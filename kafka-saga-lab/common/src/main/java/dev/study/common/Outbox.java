package dev.study.common;

import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

@Component
public class Outbox {
    private final JdbcTemplate db;
    private final Json json;
    public Outbox(JdbcTemplate db, Json json) { this.db = db; this.json = json; }

    // 업무 데이터 + 발행할 메시지를 같은 로컬 트랜잭션에 저장한다.
    @Transactional(propagation = Propagation.MANDATORY)
    public void append(String topic, Message message) {
        db.update("insert into outbox(topic, message_key, payload) values (?, ?, ?)",
                topic, message.orderId(), json.write(message));
    }
}
