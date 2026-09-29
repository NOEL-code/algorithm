/**
 * 학습: 중복 메시지와 원자적 업무 처리 seen 조회만으로는 동시 중복을 막지 못한다. event_id PK와 동일 트랜잭션 rollback이 최종 방어다.
 * MANDATORY는 호출자가 연 트랜잭션을 요구한다. 별도 REQUIRES_NEW로 기록하면 업무 실패 후에도 처리 완료로 남을 수 있다. 같은 메시지 중복은 eventId,
 * 새 ID의 같은 업무는 orderId/requestId 원장으로 구분한다.
 */
package dev.study.common;

import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

@Component
public class Inbox {
    private final JdbcTemplate db;

    public Inbox(JdbcTemplate db) {
        this.db = db;
    }

    // 호출한 업무 처리의 DB 트랜잭션에 반드시 참여한다.
    @Transactional(propagation = Propagation.MANDATORY)
    public boolean seen(String eventId) {
        return db.queryForObject(
                        "select count(*) from inbox where event_id = ?", Integer.class, eventId)
                > 0;
    }

    @Transactional(propagation = Propagation.MANDATORY)
    public void record(String eventId) {
        db.update("insert into inbox(event_id) values (?)", eventId);
    }
}
