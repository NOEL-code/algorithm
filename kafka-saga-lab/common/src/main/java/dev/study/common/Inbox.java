package dev.study.common;

import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

@Component
public class Inbox {
    private final JdbcTemplate db;
    public Inbox(JdbcTemplate db) { this.db = db; }

    // 호출한 업무 처리의 DB 트랜잭션에 반드시 참여한다.
    @Transactional(propagation = Propagation.MANDATORY)
    public boolean seen(String eventId) {
        return db.queryForObject("select count(*) from inbox where event_id = ?", Integer.class, eventId) > 0;
    }
    @Transactional(propagation = Propagation.MANDATORY)
    public void record(String eventId) {
        db.update("insert into inbox(event_id) values (?)", eventId);
    }
}
