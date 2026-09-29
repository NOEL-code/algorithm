/**
 * 학습: 재현 가능한 E2E 환경과 운영 데이터 격리 브라우저 테스트 전용 Kafka/메모리 DB/포트로 HTTP→Kafka→DB→UI 전체 경로를 실행한다. mock 응답만
 * 통과하는 UI 테스트와 달리 DTO·프록시·쿠키·비동기 계약의 불일치를 발견한다. 프로세스 종료 훅은 테스트 자원을 정리한다. 강제 종료 복구 보장을 제공하는 운영 장치는
 * 아니다.
 */
package dev.study;

import dev.study.account.AccountApplication;
import dev.study.common.Topics;
import dev.study.inventory.InventoryApplication;
import dev.study.member.MemberApplication;
import dev.study.order.OrderApplication;
import dev.study.payment.PaymentApplication;

import org.springframework.boot.builder.SpringApplicationBuilder;
import org.springframework.context.ConfigurableApplicationContext;
import org.springframework.kafka.test.EmbeddedKafkaKraftBroker;

import java.util.*;
import java.util.concurrent.CountDownLatch;

/** Isolated real backend for Playwright: in-memory DBs and an ephemeral Kafka broker. */
public class BrowserLab {
    public static void main(String[] args) throws Exception {
        var broker =
                new EmbeddedKafkaKraftBroker(
                        1,
                        3,
                        Topics.PAYMENT,
                        Topics.INVENTORY,
                        Topics.REPLIES,
                        Topics.ACCOUNT,
                        Topics.PAYMENT + ".DLT",
                        Topics.INVENTORY + ".DLT",
                        Topics.REPLIES + ".DLT",
                        Topics.ACCOUNT + ".DLT");
        var contexts = new ArrayList<ConfigurableApplicationContext>();
        Runtime.getRuntime()
                .addShutdownHook(
                        new Thread(
                                () -> {
                                    Collections.reverse(contexts);
                                    contexts.forEach(ConfigurableApplicationContext::close);
                                    broker.destroy();
                                }));
        broker.afterPropertiesSet();
        Class<?>[] apps = {
            MemberApplication.class,
            AccountApplication.class,
            OrderApplication.class,
            PaymentApplication.class,
            InventoryApplication.class
        };
        String[] names = {"member", "account", "order", "payment", "inventory"};
        int[] ports = {18083, 18084, 18080, 18081, 18082};
        for (int i = 0; i < apps.length; i++) {
            contexts.add(
                    new SpringApplicationBuilder(apps[i])
                            .run(
                                    "--server.address=127.0.0.1",
                                    "--server.port=" + ports[i],
                                    "--spring.datasource.url=jdbc:h2:mem:browser_"
                                            + names[i]
                                            + ";DB_CLOSE_DELAY=-1",
                                    "--spring.sql.init.schema-locations=classpath:common-schema.sql,classpath:"
                                            + names[i]
                                            + "-schema.sql",
                                    "--spring.kafka.bootstrap-servers="
                                            + broker.getBrokersAsString(),
                                    "--spring.kafka.consumer.group-id=browser-" + names[i],
                                    "--lab.member-url=http://127.0.0.1:18083",
                                    "--lab.account-url=http://127.0.0.1:18084",
                                    "--lab.outbox.delay=30",
                                    "--spring.main.banner-mode=off",
                                    "--logging.level.root=WARN"));
        }
        new CountDownLatch(1).await();
    }
}
