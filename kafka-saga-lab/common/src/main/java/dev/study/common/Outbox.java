/**
 * 학습: DB와 메시지 시스템 사이의 이중 쓰기 문제 업무 변경과 발행할 메시지를 같은 로컬 DB에 저장한다. Kafka로 직접 보내면 두 시스템의 성공을 원자적으로 맞출 수
 * 없다. MANDATORY로 업무 트랜잭션 참여를 강제하고 실제 네트워크 전송은 Publisher로 분리한다. orderId를 key로 저장해 같은 주문의 파티션 배치가
 * 유지되도록 한다.
 */
package dev.study.common;

import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

@Component
public class Outbox {
    private final JdbcTemplate db;
    private final Json json;

    public Outbox(JdbcTemplate db, Json json) {
        this.db = db;
        this.json = json;
    }

    // 업무 데이터 + 발행할 메시지를 같은 로컬 트랜잭션에 저장한다.
    @Transactional(propagation = Propagation.MANDATORY)
    public void append(String topic, Message message) {
        db.update(
                "insert into outbox(topic, message_key, payload) values (?, ?, ?)",
                topic,
                message.orderId(),
                json.write(message));
    }
}
