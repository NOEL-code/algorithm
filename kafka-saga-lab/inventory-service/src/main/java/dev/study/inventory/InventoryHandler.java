package dev.study.inventory;

import dev.study.common.*;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import static dev.study.common.Message.Type.*;

@Service
public class InventoryHandler {
    private final JdbcTemplate db;
    private final Inbox inbox;
    private final Outbox outbox;
    public InventoryHandler(JdbcTemplate db, Inbox inbox, Outbox outbox) {
        this.db = db; this.inbox = inbox; this.outbox = outbox;
    }

    @Transactional
    public void handle(Message m) {
        if (m.type() != RESERVE_STOCK) throw new IllegalArgumentException("예상하지 못한 재고 명령");
        if (inbox.seen(m.eventId())) return;
        var existing = db.queryForList("select status from reservations where order_id = ?", String.class, m.orderId());
        String status;
        if (existing.isEmpty()) {
            // 조회 후 Java에서 차감하면 경합 시 초과 판매될 수 있다. 조건부 UPDATE로 원자적으로 검사/차감한다.
            int changed = db.update("update stock set available = available - ? where product_id = ? and available >= ?",
                    m.quantity(), m.productId(), m.quantity());
            status = changed == 1 ? "RESERVED" : "REJECTED";
            db.update("insert into reservations(order_id, product_id, quantity, status) values (?, ?, ?, ?)",
                    m.orderId(), m.productId(), m.quantity(), status);
        } else { status = existing.getFirst(); }
        outbox.append(Topics.REPLIES, m.next(status.equals("RESERVED") ? STOCK_RESERVED : STOCK_REJECTED));
        inbox.record(m.eventId());
    }
}
