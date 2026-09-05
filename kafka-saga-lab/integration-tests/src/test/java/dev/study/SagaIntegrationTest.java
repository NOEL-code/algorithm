package dev.study;

import java.net.URI;
import java.net.http.*;
import java.time.Duration;
import java.util.*;
import java.util.concurrent.TimeUnit;
import com.fasterxml.jackson.databind.ObjectMapper;
import dev.study.common.*;
import dev.study.order.*;
import dev.study.payment.PaymentApplication;
import dev.study.inventory.*;
import org.apache.kafka.clients.consumer.Consumer;
import org.apache.kafka.common.serialization.StringDeserializer;
import org.junit.jupiter.api.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.builder.SpringApplicationBuilder;
import org.springframework.boot.web.servlet.context.ServletWebServerApplicationContext;
import org.springframework.context.ConfigurableApplicationContext;
import org.springframework.context.annotation.Configuration;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.kafka.core.*;
import org.springframework.kafka.config.KafkaListenerEndpointRegistry;
import org.springframework.kafka.test.EmbeddedKafkaBroker;
import org.springframework.kafka.test.context.EmbeddedKafka;
import org.springframework.kafka.test.utils.KafkaTestUtils;
import org.springframework.test.context.junit.jupiter.SpringJUnitConfig;
import static org.assertj.core.api.Assertions.*;
import static org.awaitility.Awaitility.await;

// Docker 없이 실제 KRaft Kafka + 서로 분리된 H2 DB 3개 + Spring 앱 3개를 기동한다.
@SpringJUnitConfig(SagaIntegrationTest.Config.class)
@EmbeddedKafka(kraft = true, partitions = 3, topics = {Topics.PAYMENT, Topics.INVENTORY, Topics.REPLIES,
        Topics.PAYMENT + ".DLT", Topics.INVENTORY + ".DLT", Topics.REPLIES + ".DLT"})
@TestInstance(TestInstance.Lifecycle.PER_CLASS)
class SagaIntegrationTest {
    @Configuration static class Config {}
    @Autowired EmbeddedKafkaBroker broker;
    ConfigurableApplicationContext order, payment, inventory;
    final ObjectMapper json = new ObjectMapper();
    final HttpClient http = HttpClient.newHttpClient();

    @BeforeAll void start() {
        order = startApp(OrderApplication.class, "order");
        payment = startApp(PaymentApplication.class, "payment");
        inventory = startApp(InventoryApplication.class, "inventory");
    }
    ConfigurableApplicationContext startApp(Class<?> app, String name) {
        return new SpringApplicationBuilder(app).run(
            "--server.port=0", "--spring.application.name=" + name + "-service",
            "--spring.datasource.url=jdbc:h2:mem:" + name + ";DB_CLOSE_DELAY=-1",
            "--spring.sql.init.schema-locations=classpath:common-schema.sql,classpath:" + name + "-schema.sql",
            "--spring.kafka.bootstrap-servers=" + broker.getBrokersAsString(),
            "--spring.kafka.consumer.group-id=" + name + "-service",
            "--lab.outbox.delay=20", "--spring.main.banner-mode=off", "--logging.level.root=WARN");
    }
    @AfterAll void stop() {
        for (var context : Arrays.asList(inventory, payment, order)) if (context != null) context.close();
    }
    @BeforeEach void resetStock() {
        db(inventory).update("update stock set available = 10 where product_id = 'book'");
    }
    JdbcTemplate db(ConfigurableApplicationContext context) { return context.getBean(JdbcTemplate.class); }
    OrderSaga saga() { return order.getBean(OrderSaga.class); }
    int available() { return db(inventory).queryForObject("select available from stock where product_id='book'", Integer.class); }
    String paymentStatus(String id) {
        return db(payment).queryForObject("select status from payments where order_id=?", String.class, id);
    }
    String create(int quantity, boolean reject) { return saga().create("book", quantity, 10000, reject); }
    void terminal(String id, String status) {
        await().atMost(Duration.ofSeconds(30)).untilAsserted(() -> assertThat(saga().get(id).get("status")).isEqualTo(status));
    }
    @SuppressWarnings("unchecked")
    void send(String topic, Message m) throws Exception {
        KafkaTemplate<String, String> template = order.getBean(KafkaTemplate.class);
        template.send(topic, m.orderId(), order.getBean(Json.class).write(m)).get(10, TimeUnit.SECONDS);
    }
    void processed(ConfigurableApplicationContext context, String eventId) {
        await().atMost(Duration.ofSeconds(20)).untilAsserted(() -> assertThat(db(context)
            .queryForObject("select count(*) from inbox where event_id=?", Integer.class, eventId)).isEqualTo(1));
    }

    @Test void normalOrderCompletesAndDeductsStock() {
        String id = create(2, false);
        terminal(id, "COMPLETED");
        assertThat(paymentStatus(id)).isEqualTo("CHARGED");
        assertThat(available()).isEqualTo(8);
    }
    @Test void paymentRejectionCancelsWithoutStockChange() {
        String id = create(2, true);
        terminal(id, "CANCELLED");
        assertThat(paymentStatus(id)).isEqualTo("REJECTED");
        assertThat(available()).isEqualTo(10);
        assertThat(saga().get(id).get("reason")).isEqualTo("PAYMENT_REJECTED");
    }
    @Test void insufficientStockRefundsBeforeCancellation() {
        String id = create(11, false);
        terminal(id, "CANCELLED");
        assertThat(paymentStatus(id)).isEqualTo("REFUNDED");
        assertThat(available()).isEqualTo(10);
        assertThat(saga().get(id).get("reason")).isEqualTo("OUT_OF_STOCK");
    }
    @Test void cancellationWaitsForRefundCompletion() {
        var inventoryListeners = inventory.getBean(KafkaListenerEndpointRegistry.class);
        var paymentListeners = payment.getBean(KafkaListenerEndpointRegistry.class);
        inventoryListeners.stop();
        try {
            String id = create(11, false);
            terminal(id, "STOCK_PENDING");
            paymentListeners.stop();
            inventoryListeners.start();
            terminal(id, "COMPENSATING");
            assertThat(paymentStatus(id)).isEqualTo("CHARGED");
            paymentListeners.start();
            terminal(id, "CANCELLED");
            assertThat(paymentStatus(id)).isEqualTo("REFUNDED");
        } finally {
            inventoryListeners.start();
            paymentListeners.start();
        }
    }
    @Test void duplicateMessagesAndBusinessCommandsDoNotDeductTwice() throws Exception {
        String id = create(2, false);
        terminal(id, "COMPLETED");
        Message reserve = Message.start(id, "book", 2, 10000, false).next(Message.Type.RESERVE_STOCK);
        send(Topics.INVENTORY, reserve);
        send(Topics.INVENTORY, reserve);
        processed(inventory, reserve.eventId());
        Message charge = Message.start(id, "book", 2, 10000, false);
        send(Topics.PAYMENT, charge);
        send(Topics.PAYMENT, charge);
        processed(payment, charge.eventId());
        // 같은 파티션에 후속 메시지를 보내 앞의 중복 메시지까지 소비했음을 확인한다.
        Message barrier = reserve.next(Message.Type.RESERVE_STOCK);
        send(Topics.INVENTORY, barrier);
        processed(inventory, barrier.eventId());
        assertThat(available()).isEqualTo(8);
        assertThat(db(payment).queryForObject("select count(*) from payments where order_id=?", Integer.class, id)).isEqualTo(1);
        assertThat(saga().get(id).get("status")).isEqualTo("COMPLETED");
    }
    @Test void duplicateRefundAndLateChargeCannotChargeAgain() throws Exception {
        String id = create(11, false);
        terminal(id, "CANCELLED");
        Message refund = Message.start(id, "book", 11, 10000, false).next(Message.Type.REFUND_PAYMENT);
        send(Topics.PAYMENT, refund);
        send(Topics.PAYMENT, refund);
        Message lateCharge = refund.next(Message.Type.CHARGE_PAYMENT);
        send(Topics.PAYMENT, lateCharge);
        processed(payment, lateCharge.eventId());
        assertThat(paymentStatus(id)).isEqualTo("REFUNDED");
        assertThat(saga().get(id).get("status")).isEqualTo("CANCELLED");
    }
    @Test void concurrentOrdersNeverOversell() {
        var ids = java.util.stream.IntStream.range(0, 6).parallel().mapToObj(i -> create(3, false)).toList();
        await().atMost(Duration.ofSeconds(40)).untilAsserted(() -> {
            assertThat(ids.stream().map(id -> saga().get(id).get("status")).toList())
                    .allMatch(s -> s.equals("COMPLETED") || s.equals("CANCELLED"));
        });
        assertThat(ids.stream().filter(id -> saga().get(id).get("status").equals("COMPLETED")).count()).isEqualTo(3);
        assertThat(available()).isEqualTo(1);
        ids.stream().filter(id -> saga().get(id).get("status").equals("CANCELLED"))
                .forEach(id -> assertThat(paymentStatus(id)).isEqualTo("REFUNDED"));
    }
    @Test void inboxFailureRollsBackStockReservationAndOutbox() {
        // 마지막 inbox 저장을 실패시켜 앞의 재고 차감/예약/outbox도 함께 롤백되는지 검증한다.
        String id = UUID.randomUUID().toString();
        Message m = new Message("x".repeat(100), id, Message.Type.RESERVE_STOCK, "book", 2, 10000, false);
        assertThatThrownBy(() -> inventory.getBean(InventoryHandler.class).handle(m)).isInstanceOf(RuntimeException.class);
        assertThat(available()).isEqualTo(10);
        assertThat(db(inventory).queryForObject("select count(*) from reservations where order_id=?", Integer.class, id)).isZero();
        assertThat(db(inventory).queryForObject("select count(*) from outbox where message_key=?", Integer.class, id)).isZero();
    }
    @Test void exhaustedTechnicalFailureGoesToDlt() throws Exception {
        Map<String, Object> props = KafkaTestUtils.consumerProps("dlt-test-" + UUID.randomUUID(), "false", broker);
        try (Consumer<String, String> consumer = new DefaultKafkaConsumerFactory<>(props,
                new StringDeserializer(), new StringDeserializer()).createConsumer()) {
            broker.consumeFromAnEmbeddedTopic(consumer, Topics.PAYMENT + ".DLT");
            Message m = Message.start(UUID.randomUUID().toString(), "book", 1, 10000, false).next(Message.Type.REFUND_PAYMENT);
            send(Topics.PAYMENT, m); // 결제가 없는 환불 → DB 예외 → 재시도 2회 → DLT
            var record = KafkaTestUtils.getSingleRecord(consumer, Topics.PAYMENT + ".DLT", Duration.ofSeconds(25));
            assertThat(order.getBean(Json.class).read(record.value()).eventId()).isEqualTo(m.eventId());
            assertThat(db(payment).queryForObject("select count(*) from inbox where event_id=?", Integer.class, m.eventId())).isZero();
        }
    }
    @Test void httpAcceptsAsyncOrderAndRejectsInvalidInput() throws Exception {
        int port = ((ServletWebServerApplicationContext) order).getWebServer().getPort();
        URI uri = URI.create("http://localhost:" + port + "/orders");
        var request = HttpRequest.newBuilder(uri).header("Content-Type", "application/json")
            .POST(HttpRequest.BodyPublishers.ofString("{\"productId\":\"book\",\"quantity\":1,\"amount\":10000}")).build();
        var response = http.send(request, HttpResponse.BodyHandlers.ofString());
        assertThat(response.statusCode()).isEqualTo(202);
        String id = json.readTree(response.body()).get("orderId").asText();
        terminal(id, "COMPLETED");
        var get = http.send(HttpRequest.newBuilder(URI.create(uri + "/" + id)).GET().build(), HttpResponse.BodyHandlers.ofString());
        assertThat(json.readTree(get.body()).get("status").asText()).isEqualTo("COMPLETED");
        var bad = HttpRequest.newBuilder(uri).header("Content-Type", "application/json")
            .POST(HttpRequest.BodyPublishers.ofString("{\"productId\":\"book\",\"quantity\":0,\"amount\":-1}")).build();
        assertThat(http.send(bad, HttpResponse.BodyHandlers.ofString()).statusCode()).isEqualTo(400);
    }
}
