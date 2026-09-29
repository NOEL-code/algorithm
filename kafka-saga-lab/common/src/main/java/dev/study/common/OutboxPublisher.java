/**
 * 학습: 발행 보장과 정확히 한 번의 차이 Kafka ACK를 기다린 뒤 outbox를 지운다. ACK 후 DB 커밋 전 장애라면 재발행되므로 Inbox가 필요하다. DB 행
 * 잠금을 보유한 채 네트워크를 기다리는 단순 구현이다. 느린 전송이 DB 자원을 점유하는 비용을 관찰한다. 서비스당 한 Publisher 전제다. SKIP LOCKED를 붙여도
 * 같은 주문의 다중 발행 순서가 자동 보장되지는 않는다.
 */
package dev.study.common;

import org.springframework.boot.autoconfigure.condition.ConditionalOnProperty;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.kafka.core.KafkaTemplate;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Transactional;

import java.util.concurrent.TimeUnit;

@Component
@ConditionalOnProperty(name = "lab.outbox.enabled", havingValue = "true", matchIfMissing = true)
public class OutboxPublisher {
    private final JdbcTemplate db;
    private final KafkaTemplate<String, String> kafka;

    public OutboxPublisher(JdbcTemplate db, KafkaTemplate<String, String> kafka) {
        this.db = db;
        this.kafka = kafka;
    }

    // 학습용: 서비스당 단일 인스턴스/단일 publisher. 한 건씩 순서대로 전달한다.
    @Scheduled(fixedDelayString = "${lab.outbox.delay:100}")
    @Transactional(rollbackFor = Exception.class)
    public void publish() throws Exception {
        var rows = db.queryForList("select * from outbox order by id limit 1 for update");
        if (rows.isEmpty()) return;
        var row = rows.getFirst();
        kafka.send(
                        (String) row.get("TOPIC"),
                        (String) row.get("MESSAGE_KEY"),
                        (String) row.get("PAYLOAD"))
                .get(10, TimeUnit.SECONDS);
        // ACK 이후 DB 커밋 전에 죽으면 재발행된다. 따라서 소비자 멱등성이 필요하다.
        db.update("delete from outbox where id = ?", row.get("ID"));
    }
}
