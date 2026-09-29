/**
 * 학습: 안전성 주장을 실행 가능한 증거로 만들기 실제 Embedded Kafka와 분리된 H2 DB를 사용해 원장·잔액·상태의 결과를 검증한다. 동시 주문 E2E와 DB
 * Handler 직접 경합 테스트는 서로 다른 증거다. consumer concurrency=1을 락 검증으로 오해하지 않는다. 메모리 DB 테스트는 운영 DB 격리 수준,
 * 디스크 장애, 네트워크 분단까지 입증하지 않는다.
 */
package dev.study;

import static org.assertj.core.api.Assertions.*;
import static org.awaitility.Awaitility.await;

import com.fasterxml.jackson.databind.ObjectMapper;

import dev.study.account.*;
import dev.study.common.*;
import dev.study.inventory.*;
import dev.study.member.MemberApplication;
import dev.study.order.*;
import dev.study.payment.PaymentApplication;

import org.apache.kafka.clients.consumer.Consumer;
import org.apache.kafka.common.serialization.StringDeserializer;
import org.junit.jupiter.api.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.builder.SpringApplicationBuilder;
import org.springframework.boot.web.servlet.context.ServletWebServerApplicationContext;
import org.springframework.context.ConfigurableApplicationContext;
import org.springframework.context.annotation.Configuration;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.kafka.config.KafkaListenerEndpointRegistry;
import org.springframework.kafka.core.*;
import org.springframework.kafka.test.EmbeddedKafkaBroker;
import org.springframework.kafka.test.context.EmbeddedKafka;
import org.springframework.kafka.test.utils.KafkaTestUtils;
import org.springframework.test.context.junit.jupiter.SpringJUnitConfig;

import java.net.URI;
import java.net.http.*;
import java.time.Duration;
import java.util.*;
import java.util.concurrent.TimeUnit;

// Docker 없이 실제 KRaft Kafka + 서로 분리된 H2 DB 3개 + Spring 앱 3개를 기동한다.
@SpringJUnitConfig(SagaIntegrationTest.Config.class)
@EmbeddedKafka(
        kraft = true,
        partitions = 3,
        topics = {
            Topics.PAYMENT,
            Topics.INVENTORY,
            Topics.REPLIES,
            Topics.PAYMENT + ".DLT",
            Topics.INVENTORY + ".DLT",
            Topics.REPLIES + ".DLT",
            Topics.ACCOUNT,
            Topics.ACCOUNT + ".DLT"
        })
@TestInstance(TestInstance.Lifecycle.PER_CLASS)
class SagaIntegrationTest {
    @Configuration
    static class Config {}

    @Autowired EmbeddedKafkaBroker broker;
    ConfigurableApplicationContext order, payment, inventory, member, account;
    final ObjectMapper json = new ObjectMapper();
    final HttpClient http = HttpClient.newHttpClient();

    @BeforeAll
    void start() {
        member = startApp(MemberApplication.class, "member");
        account = startApp(AccountApplication.class, "account");
        order = startApp(OrderApplication.class, "order");
        payment = startApp(PaymentApplication.class, "payment");
        inventory = startApp(InventoryApplication.class, "inventory");
    }

    ConfigurableApplicationContext startApp(Class<?> app, String name) {
        return new SpringApplicationBuilder(app)
                .run(
                        "--server.port=0",
                        "--lab.member-url="
                                + (member == null ? "http://localhost:8083" : base(member)),
                        "--lab.account-url="
                                + (account == null ? "http://localhost:8084" : base(account)),
                        "--spring.application.name=" + name + "-service",
                        "--spring.datasource.url=jdbc:h2:mem:" + name + ";DB_CLOSE_DELAY=-1",
                        "--spring.sql.init.schema-locations=classpath:common-schema.sql,classpath:"
                                + name
                                + "-schema.sql",
                        "--spring.kafka.bootstrap-servers=" + broker.getBrokersAsString(),
                        "--spring.kafka.consumer.group-id=" + name + "-service",
                        "--lab.outbox.delay=20",
                        "--spring.main.banner-mode=off",
                        "--logging.level.root=WARN");
    }

    @AfterAll
    void stop() {
        for (var context : Arrays.asList(inventory, payment, order, account, member))
            if (context != null) context.close();
    }

    @BeforeEach
    void resetStock() {
        db(inventory).update("update stock set available = 10 where product_id = 'book'");
    }

    JdbcTemplate db(ConfigurableApplicationContext context) {
        return context.getBean(JdbcTemplate.class);
    }

    OrderSaga saga() {
        return order.getBean(OrderSaga.class);
    }

    int available() {
        return db(inventory)
                .queryForObject(
                        "select available from stock where product_id='book'", Integer.class);
    }

    String paymentStatus(String id) {
        return db(payment)
                .queryForObject("select status from payments where order_id=?", String.class, id);
    }

    String create(int quantity, boolean reject) {
        return saga().create("book", quantity, 10000, reject);
    }

    void terminal(String id, String status) {
        await().atMost(Duration.ofSeconds(30))
                .untilAsserted(() -> assertThat(saga().get(id).get("status")).isEqualTo(status));
    }

    @SuppressWarnings("unchecked")
    void send(String topic, Message m) throws Exception {
        KafkaTemplate<String, String> template = order.getBean(KafkaTemplate.class);
        template.send(topic, m.orderId(), order.getBean(Json.class).write(m))
                .get(10, TimeUnit.SECONDS);
    }

    void processed(ConfigurableApplicationContext context, String eventId) {
        await().atMost(Duration.ofSeconds(20))
                .untilAsserted(
                        () ->
                                assertThat(
                                                db(context)
                                                        .queryForObject(
                                                                "select count(*) from inbox where"
                                                                        + " event_id=?",
                                                                Integer.class,
                                                                eventId))
                                        .isEqualTo(1));
    }

    // 학습 검증: 기초 Saga의 정상 경로와 재고 최종값을 검증한다.
    @Test
    void normalOrderCompletesAndDeductsStock() {
        String id = create(2, false);
        terminal(id, "COMPLETED");
        assertThat(paymentStatus(id)).isEqualTo("CHARGED");
        assertThat(available()).isEqualTo(8);
    }

    // 학습 검증: 성공 호출 횟수보다 상태/부수 효과를 검사한다. 아래 실패·중복 시나리오가 어떤 불변식을 지키는지 읽는다.
    @Test
    void paymentRejectionCancelsWithoutStockChange() {
        String id = create(2, true);
        terminal(id, "CANCELLED");
        assertThat(paymentStatus(id)).isEqualTo("REJECTED");
        assertThat(available()).isEqualTo(10);
        assertThat(saga().get(id).get("reason")).isEqualTo("PAYMENT_REJECTED");
    }

    // 학습 검증: 성공 호출 횟수보다 상태/부수 효과를 검사한다. 아래 실패·중복 시나리오가 어떤 불변식을 지키는지 읽는다.
    @Test
    void insufficientStockRefundsBeforeCancellation() {
        String id = create(11, false);
        terminal(id, "CANCELLED");
        assertThat(paymentStatus(id)).isEqualTo("REFUNDED");
        assertThat(available()).isEqualTo(10);
        assertThat(saga().get(id).get("reason")).isEqualTo("OUT_OF_STOCK");
    }

    // 학습 검증: 성공 호출 횟수보다 상태/부수 효과를 검사한다. 아래 실패·중복 시나리오가 어떤 불변식을 지키는지 읽는다.
    @Test
    void cancellationWaitsForRefundCompletion() {
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

    // 학습 검증: 성공 호출 횟수보다 상태/부수 효과를 검사한다. 아래 실패·중복 시나리오가 어떤 불변식을 지키는지 읽는다.
    @Test
    void duplicateMessagesAndBusinessCommandsDoNotDeductTwice() throws Exception {
        String id = create(2, false);
        terminal(id, "COMPLETED");
        Message reserve =
                Message.start(id, "book", 2, 10000, false).next(Message.Type.RESERVE_STOCK);
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
        assertThat(
                        db(payment)
                                .queryForObject(
                                        "select count(*) from payments where order_id=?",
                                        Integer.class,
                                        id))
                .isEqualTo(1);
        assertThat(saga().get(id).get("status")).isEqualTo("COMPLETED");
    }

    // 학습 검증: 성공 호출 횟수보다 상태/부수 효과를 검사한다. 아래 실패·중복 시나리오가 어떤 불변식을 지키는지 읽는다.
    @Test
    void duplicateRefundAndLateChargeCannotChargeAgain() throws Exception {
        String id = create(11, false);
        terminal(id, "CANCELLED");
        Message refund =
                Message.start(id, "book", 11, 10000, false).next(Message.Type.REFUND_PAYMENT);
        send(Topics.PAYMENT, refund);
        send(Topics.PAYMENT, refund);
        Message lateCharge = refund.next(Message.Type.CHARGE_PAYMENT);
        send(Topics.PAYMENT, lateCharge);
        processed(payment, lateCharge.eventId());
        assertThat(paymentStatus(id)).isEqualTo("REFUNDED");
        assertThat(saga().get(id).get("status")).isEqualTo("CANCELLED");
    }

    // 학습 검증: 여러 주문의 종단 간 결과 검증이다. 단일 consumer이므로 DB 행 경합의 단독 증거는 아니다.
    @Test
    void concurrentOrdersNeverOversell() {
        var ids =
                java.util.stream.IntStream.range(0, 6)
                        .parallel()
                        .mapToObj(i -> create(3, false))
                        .toList();
        await().atMost(Duration.ofSeconds(40))
                .untilAsserted(
                        () -> {
                            assertThat(
                                            ids.stream()
                                                    .map(id -> saga().get(id).get("status"))
                                                    .toList())
                                    .allMatch(s -> s.equals("COMPLETED") || s.equals("CANCELLED"));
                        });
        assertThat(
                        ids.stream()
                                .filter(id -> saga().get(id).get("status").equals("COMPLETED"))
                                .count())
                .isEqualTo(3);
        assertThat(available()).isEqualTo(1);
        ids.stream()
                .filter(id -> saga().get(id).get("status").equals("CANCELLED"))
                .forEach(id -> assertThat(paymentStatus(id)).isEqualTo("REFUNDED"));
    }

    // 학습 검증: 트랜잭션의 마지막 INSERT를 실패시켜 앞선 모든 업무 변경의 rollback을 증명한다.
    @Test
    void inboxFailureRollsBackStockReservationAndOutbox() {
        // 마지막 inbox 저장을 실패시켜 앞의 재고 차감/예약/outbox도 함께 롤백되는지 검증한다.
        String id = UUID.randomUUID().toString();
        Message m =
                new Message(
                        "x".repeat(100), id, Message.Type.RESERVE_STOCK, "book", 2, 10000, false);
        assertThatThrownBy(() -> inventory.getBean(InventoryHandler.class).handle(m))
                .isInstanceOf(RuntimeException.class);
        assertThat(available()).isEqualTo(10);
        assertThat(
                        db(inventory)
                                .queryForObject(
                                        "select count(*) from reservations where order_id=?",
                                        Integer.class,
                                        id))
                .isZero();
        assertThat(
                        db(inventory)
                                .queryForObject(
                                        "select count(*) from outbox where message_key=?",
                                        Integer.class,
                                        id))
                .isZero();
    }

    // 학습 검증: 업무 실패가 아닌 기술 실패의 재시도 소진 경로를 실제 DLT 소비로 확인한다.
    @Test
    void exhaustedTechnicalFailureGoesToDlt() throws Exception {
        Map<String, Object> props =
                KafkaTestUtils.consumerProps("dlt-test-" + UUID.randomUUID(), "false", broker);
        try (Consumer<String, String> consumer =
                new DefaultKafkaConsumerFactory<>(
                                props, new StringDeserializer(), new StringDeserializer())
                        .createConsumer()) {
            broker.consumeFromAnEmbeddedTopic(consumer, Topics.PAYMENT + ".DLT");
            Message m =
                    Message.start(UUID.randomUUID().toString(), "book", 1, 10000, false)
                            .next(Message.Type.REFUND_PAYMENT);
            send(Topics.PAYMENT, m); // 결제가 없는 환불 → DB 예외 → 재시도 2회 → DLT
            var record =
                    KafkaTestUtils.getSingleRecord(
                            consumer, Topics.PAYMENT + ".DLT", Duration.ofSeconds(25));
            assertThat(order.getBean(Json.class).read(record.value()).eventId())
                    .isEqualTo(m.eventId());
            assertThat(
                            db(payment)
                                    .queryForObject(
                                            "select count(*) from inbox where event_id=?",
                                            Integer.class,
                                            m.eventId()))
                    .isZero();
        }
    }

    // 학습 검증: HTTP 접수 202와 비동기 완료, 입력 오류 400이 서로 다른 의미임을 확인한다.
    @Test
    void httpAcceptsAsyncOrderAndRejectsInvalidInput() throws Exception {
        int port = ((ServletWebServerApplicationContext) order).getWebServer().getPort();
        URI uri = URI.create("http://localhost:" + port + "/orders");
        String cookie = register();
        String accountId = openAccount(cookie, 10000);
        var request =
                HttpRequest.newBuilder(uri)
                        .header("Content-Type", "application/json")
                        .header("Cookie", cookie)
                        .POST(
                                HttpRequest.BodyPublishers.ofString(
                                        json.writeValueAsString(
                                                Map.of(
                                                        "productId",
                                                        "book",
                                                        "quantity",
                                                        1,
                                                        "amount",
                                                        10000,
                                                        "accountId",
                                                        accountId))))
                        .build();
        var response = http.send(request, HttpResponse.BodyHandlers.ofString());
        assertThat(response.statusCode()).isEqualTo(202);
        String id = json.readTree(response.body()).get("orderId").asText();
        terminal(id, "COMPLETED");
        var get =
                http.send(
                        HttpRequest.newBuilder(URI.create(uri + "/" + id))
                                .header("Cookie", cookie)
                                .GET()
                                .build(),
                        HttpResponse.BodyHandlers.ofString());
        assertThat(json.readTree(get.body()).get("status").asText()).isEqualTo("COMPLETED");
        var bad =
                HttpRequest.newBuilder(uri)
                        .header("Content-Type", "application/json")
                        .POST(
                                HttpRequest.BodyPublishers.ofString(
                                        "{\"productId\":\"book\",\"quantity\":0,\"amount\":-1}"))
                        .build();
        assertThat(http.send(bad, HttpResponse.BodyHandlers.ofString()).statusCode())
                .isEqualTo(400);
    }

    String base(ConfigurableApplicationContext app) {
        return "http://localhost:"
                + ((ServletWebServerApplicationContext) app).getWebServer().getPort();
    }

    HttpResponse<String> api(
            ConfigurableApplicationContext app,
            String method,
            String path,
            String cookie,
            Object body)
            throws Exception {
        var builder =
                HttpRequest.newBuilder(URI.create(base(app) + path))
                        .header("Content-Type", "application/json");
        if (cookie != null) builder.header("Cookie", cookie);
        return http.send(
                builder.method(
                                method,
                                body == null
                                        ? HttpRequest.BodyPublishers.noBody()
                                        : HttpRequest.BodyPublishers.ofString(
                                                json.writeValueAsString(body)))
                        .build(),
                HttpResponse.BodyHandlers.ofString());
    }

    String register() throws Exception {
        var response =
                api(
                        member,
                        "POST",
                        "/members",
                        null,
                        Map.of(
                                "email",
                                UUID.randomUUID() + "@example.test",
                                "name",
                                "테스트 회원",
                                "password",
                                "test-password-123"));
        assertThat(response.statusCode()).isEqualTo(201);
        return response.headers().firstValue("Set-Cookie").orElseThrow().split(";", 2)[0];
    }

    String openAccount(String cookie, long balance) throws Exception {
        var response = api(account, "POST", "/accounts", cookie, Map.of("name", "주문 계좌"));
        assertThat(response.statusCode()).isEqualTo(201);
        String id = json.readTree(response.body()).get("id").asText();
        if (balance > 0)
            assertThat(
                            api(
                                            account,
                                            "POST",
                                            "/accounts/" + id + "/transactions",
                                            cookie,
                                            Map.of(
                                                    "type",
                                                    "DEPOSIT",
                                                    "amount",
                                                    balance,
                                                    "requestId",
                                                    UUID.randomUUID().toString()))
                                    .statusCode())
                    .isEqualTo(200);
        return id;
    }

    String accountOrder(String cookie, String accountId, int quantity, long amount, boolean reject)
            throws Exception {
        var response =
                api(
                        order,
                        "POST",
                        "/orders",
                        cookie,
                        Map.of(
                                "productId",
                                "book",
                                "quantity",
                                quantity,
                                "amount",
                                amount,
                                "rejectPayment",
                                reject,
                                "accountId",
                                accountId));
        assertThat(response.statusCode()).isEqualTo(202);
        return json.readTree(response.body()).get("orderId").asText();
    }

    long balance(String id) {
        return db(account)
                .queryForObject("select balance from accounts where id=?", Long.class, id);
    }

    // 학습 검증: 가입/인증/수정/비밀번호 변경의 세션 수명과 토큰 저장 정책을 검증한다.
    @Test
    void membersAuthenticateUpdateAndRevokeSessions() throws Exception {
        String email = UUID.randomUUID() + "@example.test";
        var body = Map.of("email", email, "name", "회원", "password", "test-password-123");
        var response = api(member, "POST", "/members", null, body);
        assertThat(response.statusCode()).isEqualTo(201);
        String cookie = response.headers().firstValue("Set-Cookie").orElseThrow().split(";", 2)[0];
        assertThat(response.headers().firstValue("Set-Cookie").orElseThrow())
                .contains("HttpOnly", "SameSite=Strict");
        assertThat(
                        api(
                                        member,
                                        "POST",
                                        "/members",
                                        null,
                                        Map.of(
                                                "email",
                                                email.toUpperCase(Locale.ROOT),
                                                "name",
                                                "중복",
                                                "password",
                                                "test-password-123"))
                                .statusCode())
                .isEqualTo(409);
        assertThat(
                        api(
                                        member,
                                        "POST",
                                        "/members/sessions",
                                        null,
                                        Map.of("email", email, "password", "incorrect-password"))
                                .statusCode())
                .isEqualTo(401);
        assertThat(api(member, "GET", "/members/me", null, null).statusCode()).isEqualTo(401);
        assertThat(api(member, "PATCH", "/members/me", cookie, Map.of("name", "변경된 이름")).body())
                .contains("변경된 이름");
        assertThat(
                        api(
                                        member,
                                        "PUT",
                                        "/members/me/password",
                                        cookie,
                                        Map.of(
                                                "currentPassword",
                                                "wrong-password",
                                                "newPassword",
                                                "new-password-123"))
                                .statusCode())
                .isEqualTo(400);
        assertThat(
                        api(
                                        member,
                                        "PUT",
                                        "/members/me/password",
                                        cookie,
                                        Map.of(
                                                "currentPassword",
                                                "test-password-123",
                                                "newPassword",
                                                "new-password-123"))
                                .statusCode())
                .isEqualTo(204);
        assertThat(api(member, "GET", "/members/me", cookie, null).statusCode()).isEqualTo(401);
        var login =
                api(
                        member,
                        "POST",
                        "/members/sessions",
                        null,
                        Map.of("email", email, "password", "new-password-123"));
        assertThat(login.statusCode()).isEqualTo(200);
        String second = login.headers().firstValue("Set-Cookie").orElseThrow().split(";", 2)[0];
        assertThat(api(member, "DELETE", "/members/sessions/current", second, null).statusCode())
                .isEqualTo(204);
        assertThat(api(member, "GET", "/members/me", second, null).statusCode()).isEqualTo(401);
        assertThat(
                        db(member)
                                .queryForObject(
                                        "select password_hash from members where email=?",
                                        String.class,
                                        email))
                .doesNotContain("password");
    }

    // 학습 검증: 다른 회원의 접근 차단, 같은 요청 중복, 실제 병렬 HTTP 출금의 잔액 불변식을 함께 검증한다.
    @Test
    void accountOwnershipIdempotencyAndConcurrentWithdrawals() throws Exception {
        String owner = register(), stranger = register(), id = openAccount(owner, 100);
        assertThat(api(account, "GET", "/accounts", null, null).statusCode()).isEqualTo(401);
        assertThat(api(account, "GET", "/accounts/" + id, stranger, null).statusCode())
                .isEqualTo(404);
        assertThat(
                        api(account, "PATCH", "/accounts/" + id, stranger, Map.of("name", "탈취"))
                                .statusCode())
                .isEqualTo(404);
        assertThat(
                        api(
                                        account,
                                        "POST",
                                        "/accounts/" + id + "/transactions",
                                        stranger,
                                        Map.of(
                                                "type",
                                                "WITHDRAW",
                                                "amount",
                                                1,
                                                "requestId",
                                                UUID.randomUUID().toString()))
                                .statusCode())
                .isEqualTo(404);
        assertThat(api(account, "DELETE", "/accounts/" + id, owner, null).statusCode())
                .isEqualTo(409);
        String requestId = UUID.randomUUID().toString();
        var deposit = Map.of("type", "DEPOSIT", "amount", 10, "requestId", requestId);
        assertThat(
                        api(account, "POST", "/accounts/" + id + "/transactions", owner, deposit)
                                .statusCode())
                .isEqualTo(200);
        assertThat(
                        api(account, "POST", "/accounts/" + id + "/transactions", owner, deposit)
                                .statusCode())
                .isEqualTo(200);
        assertThat(balance(id)).isEqualTo(110);
        assertThat(
                        api(
                                        account,
                                        "POST",
                                        "/accounts/" + id + "/transactions",
                                        owner,
                                        Map.of(
                                                "type",
                                                "DEPOSIT",
                                                "amount",
                                                20,
                                                "requestId",
                                                requestId))
                                .statusCode())
                .isEqualTo(409);
        assertThat(
                        api(
                                        account,
                                        "POST",
                                        "/accounts/" + id + "/transactions",
                                        owner,
                                        Map.of(
                                                "type",
                                                "DEPOSIT",
                                                "amount",
                                                -1,
                                                "requestId",
                                                UUID.randomUUID().toString()))
                                .statusCode())
                .isEqualTo(400);
        try (var pool = java.util.concurrent.Executors.newFixedThreadPool(12)) {
            var futures = new ArrayList<java.util.concurrent.Future<Integer>>();
            for (int i = 0; i < 20; i++)
                futures.add(
                        pool.submit(
                                () ->
                                        api(
                                                        account,
                                                        "POST",
                                                        "/accounts/" + id + "/transactions",
                                                        owner,
                                                        Map.of(
                                                                "type",
                                                                "WITHDRAW",
                                                                "amount",
                                                                10,
                                                                "requestId",
                                                                UUID.randomUUID().toString()))
                                                .statusCode()));
            int success = 0;
            for (var future : futures) {
                int status = future.get(20, TimeUnit.SECONDS);
                assertThat(status).isIn(200, 409);
                if (status == 200) success++;
            }
            assertThat(success).isEqualTo(11);
        }
        assertThat(balance(id)).isZero();
        assertThat(api(account, "DELETE", "/accounts/" + id, owner, null).statusCode())
                .isEqualTo(204);
        assertThat(
                        api(
                                        account,
                                        "POST",
                                        "/accounts/" + id + "/transactions",
                                        owner,
                                        Map.of(
                                                "type",
                                                "DEPOSIT",
                                                "amount",
                                                1,
                                                "requestId",
                                                UUID.randomUUID().toString()))
                                .statusCode())
                .isEqualTo(409);
    }

    // 학습 검증: 서비스 경계를 통과한 잔액 변화와 중복 환불 방지를 최종 DB 값으로 검증한다.
    @Test
    void ordersDebitRefundAndRejectWithoutLeakingOtherMembersData() throws Exception {
        String owner = register(), stranger = register(), accountId = openAccount(owner, 30000);
        String normal = accountOrder(owner, accountId, 1, 10000, false);
        terminal(normal, "COMPLETED");
        assertThat(balance(accountId)).isEqualTo(20000);
        assertThat(api(order, "GET", "/orders/" + normal, stranger, null).statusCode())
                .isEqualTo(404);
        assertThat(api(payment, "GET", "/payments/" + normal, stranger, null).statusCode())
                .isEqualTo(404);
        assertThat(
                        api(
                                        order,
                                        "POST",
                                        "/orders",
                                        stranger,
                                        Map.of(
                                                "productId",
                                                "book",
                                                "quantity",
                                                1,
                                                "amount",
                                                10000,
                                                "accountId",
                                                accountId))
                                .statusCode())
                .isEqualTo(404);
        String refund = accountOrder(owner, accountId, 999, 10000, false);
        terminal(refund, "CANCELLED");
        assertThat(paymentStatus(refund)).isEqualTo("REFUNDED");
        assertThat(balance(accountId)).isEqualTo(20000);
        String memberId =
                db(account)
                        .queryForObject(
                                "select member_id from accounts where id=?",
                                String.class,
                                accountId);
        Message duplicate =
                new Message(
                        UUID.randomUUID().toString(),
                        refund,
                        Message.Type.CREDIT_ACCOUNT,
                        "book",
                        999,
                        10000,
                        false,
                        memberId,
                        accountId);
        send(Topics.ACCOUNT, duplicate);
        processed(account, duplicate.eventId());
        assertThat(balance(accountId)).isEqualTo(20000);
        assertThat(
                        db(account)
                                .queryForObject(
                                        "select count(*) from account_transactions where"
                                                + " account_id=? and type='REFUND'",
                                        Integer.class,
                                        accountId))
                .isEqualTo(1);
        String insufficient = accountOrder(owner, accountId, 1, 30000, false);
        terminal(insufficient, "CANCELLED");
        assertThat(paymentStatus(insufficient)).isEqualTo("REJECTED");
        assertThat(balance(accountId)).isEqualTo(20000);
        String rejected = accountOrder(owner, accountId, 1, 10000, true);
        terminal(rejected, "CANCELLED");
        assertThat(balance(accountId)).isEqualTo(20000);
    }

    // 학습 검증: 재고 소비를 멈춰 중간 상태를 만들고 해지 금지/정산 후 해지 가능을 관찰한다.
    @Test
    void accountCannotCloseWhileOrderNeedsPossibleRefund() throws Exception {
        String cookie = register(), id = openAccount(cookie, 10000);
        var registry = inventory.getBean(KafkaListenerEndpointRegistry.class);
        registry.stop();
        String orderId;
        try {
            orderId = accountOrder(cookie, id, 1, 10000, false);
            await().atMost(Duration.ofSeconds(30))
                    .untilAsserted(
                            () ->
                                    assertThat(saga().get(orderId).get("status"))
                                            .isEqualTo("STOCK_PENDING"));
            assertThat(balance(id)).isZero();
            assertThat(api(account, "DELETE", "/accounts/" + id, cookie, null).statusCode())
                    .isEqualTo(409);
        } finally {
            registry.start();
        }
        terminal(orderId, "COMPLETED");
        await().atMost(Duration.ofSeconds(30))
                .untilAsserted(
                        () ->
                                assertThat(
                                                db(account)
                                                        .queryForObject(
                                                                "select status from account_orders"
                                                                        + " where order_id=?",
                                                                String.class,
                                                                orderId))
                                        .isEqualTo("SETTLED"));
        assertThat(api(account, "DELETE", "/accounts/" + id, cookie, null).statusCode())
                .isEqualTo(204);
    }

    // 학습 검증: HTTP @Valid를 우회한 Bean 직접 호출도 잔액을 훼손하지 않아야 한다.
    @Test
    void accountRulesProtectNonHttpCallers() throws Exception {
        String cookie = register(), id = openAccount(cookie, 100);
        String memberId =
                db(account)
                        .queryForObject(
                                "select member_id from accounts where id=?", String.class, id);
        AccountService service = account.getBean(AccountService.class);
        for (long amount : new long[] {-10, 0, AccountRules.MAX_TRANSFER + 1}) {
            assertThatThrownBy(
                            () ->
                                    service.transact(
                                            memberId,
                                            id,
                                            "WITHDRAW",
                                            amount,
                                            UUID.randomUUID().toString()))
                    .isInstanceOf(AccountRules.InvalidMovement.class);
        }
        assertThatThrownBy(
                        () ->
                                service.transact(
                                        memberId, id, "TYPO", 10, UUID.randomUUID().toString()))
                .isInstanceOf(AccountRules.InvalidMovement.class);
        assertThatThrownBy(() -> service.transact(memberId, id, "DEPOSIT", 10, "not-a-uuid"))
                .isInstanceOf(AccountRules.InvalidMovement.class);
        assertThat(balance(id)).isEqualTo(100);
        assertThat(
                        db(account)
                                .queryForObject(
                                        "select count(*) from account_transactions where"
                                                + " account_id=?",
                                        Integer.class,
                                        id))
                .isEqualTo(1);
    }

    // 학습 검증: Kafka의 순차 소비를 제외하고 여러 DB 트랜잭션이 재고 행에서 직접 경쟁하도록 만든다.
    @Test
    void conditionalStockUpdateSurvivesActualDatabaseContention() throws Exception {
        try (var isolated =
                        new SpringApplicationBuilder(InventoryApplication.class)
                                .web(org.springframework.boot.WebApplicationType.NONE)
                                .run(
                                        "--spring.datasource.url=jdbc:h2:mem:inventory_race;DB_CLOSE_DELAY=-1",
                                        "--spring.sql.init.schema-locations=classpath:common-schema.sql,classpath:inventory-schema.sql",
                                        "--spring.kafka.bootstrap-servers="
                                                + broker.getBrokersAsString(),
                                        "--spring.kafka.listener.auto-startup=false",
                                        "--lab.outbox.enabled=false",
                                        "--spring.main.banner-mode=off",
                                        "--logging.level.root=WARN");
                var pool = java.util.concurrent.Executors.newFixedThreadPool(6)) {
            var gate = new java.util.concurrent.CyclicBarrier(6);
            var futures = new ArrayList<java.util.concurrent.Future<?>>();
            for (int i = 0; i < 6; i++)
                futures.add(
                        pool.submit(
                                () -> {
                                    gate.await(5, TimeUnit.SECONDS);
                                    isolated.getBean(InventoryHandler.class)
                                            .handle(
                                                    Message.start(
                                                                    UUID.randomUUID().toString(),
                                                                    "book",
                                                                    3,
                                                                    10000,
                                                                    false)
                                                            .next(Message.Type.RESERVE_STOCK));
                                    return null;
                                }));
            for (var future : futures) future.get(15, TimeUnit.SECONDS);
            assertThat(
                            db(isolated)
                                    .queryForObject(
                                            "select available from stock where product_id='book'",
                                            Integer.class))
                    .isEqualTo(1);
            assertThat(
                            db(isolated)
                                    .queryForObject(
                                            "select count(*) from reservations where"
                                                    + " status='RESERVED'",
                                            Integer.class))
                    .isEqualTo(3);
            assertThat(
                            db(isolated)
                                    .queryForObject(
                                            "select count(*) from reservations where"
                                                    + " status='REJECTED'",
                                            Integer.class))
                    .isEqualTo(3);
            assertThat(db(isolated).queryForObject("select count(*) from inbox", Integer.class))
                    .isEqualTo(6);
            assertThat(db(isolated).queryForObject("select count(*) from outbox", Integer.class))
                    .isEqualTo(6);
        }
    }

    // 학습 검증: 비밀번호 변경 중 로그인은 기다려야 하며, 변경 후 옛 비밀번호로 새 세션을 만들면 안 된다.
    @Test
    void loginWaitsForCredentialChangeAndRejectsOldPassword() throws Exception {
        String email = UUID.randomUUID() + "@example.test";
        var registered =
                api(
                        member,
                        "POST",
                        "/members",
                        null,
                        Map.of("email", email, "name", "경합 회원", "password", "old-password-123"));
        assertThat(registered.statusCode()).isEqualTo(201);
        String memberId = json.readTree(registered.body()).get("id").asText();
        String donorEmail = UUID.randomUUID() + "@example.test";
        assertThat(
                        api(
                                        member,
                                        "POST",
                                        "/members",
                                        null,
                                        Map.of(
                                                "email",
                                                donorEmail,
                                                "name",
                                                "해시 준비",
                                                "password",
                                                "new-password-456"))
                                .statusCode())
                .isEqualTo(201);
        String replacement =
                db(member)
                        .queryForObject(
                                "select password_hash from members where email=?",
                                String.class,
                                donorEmail);
        try (var pool = java.util.concurrent.Executors.newSingleThreadExecutor()) {
            java.util.concurrent.Future<HttpResponse<String>> response;
            // 외부 트랜잭션이 비밀번호 변경의 행 잠금을 보유하는 상황을 결정적으로 만든다.
            try (var connection = member.getBean(javax.sql.DataSource.class).getConnection()) {
                connection.setAutoCommit(false);
                try (var lock =
                        connection.prepareStatement(
                                "select id from members where id=? for update")) {
                    lock.setString(1, memberId);
                    try (var rows = lock.executeQuery()) {
                        assertThat(rows.next()).isTrue();
                    }
                }
                response =
                        pool.submit(
                                () ->
                                        api(
                                                member,
                                                "POST",
                                                "/members/sessions",
                                                null,
                                                Map.of(
                                                        "email",
                                                        email,
                                                        "password",
                                                        "old-password-123")));
                assertThatThrownBy(() -> response.get(1, TimeUnit.SECONDS))
                        .isInstanceOf(java.util.concurrent.TimeoutException.class);
                try (var change =
                        connection.prepareStatement(
                                "update members set password_hash=? where id=?")) {
                    change.setString(1, replacement);
                    change.setString(2, memberId);
                    change.executeUpdate();
                }
                try (var revoke =
                        connection.prepareStatement(
                                "delete from member_sessions where member_id=?")) {
                    revoke.setString(1, memberId);
                    revoke.executeUpdate();
                }
                connection.commit();
            }
            assertThat(response.get(10, TimeUnit.SECONDS).statusCode()).isEqualTo(401);
        }
        assertThat(
                        db(member)
                                .queryForObject(
                                        "select count(*) from member_sessions where member_id=?",
                                        Integer.class,
                                        memberId))
                .isZero();
    }
}
