package dev.study.payment;

import dev.study.common.*;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import static dev.study.common.Message.Type.*;

@Service
public class PaymentHandler {
    private final JdbcTemplate db;
    private final Inbox inbox;
    private final Outbox outbox;
    public PaymentHandler(JdbcTemplate db, Inbox inbox, Outbox outbox) {
        this.db = db; this.inbox = inbox; this.outbox = outbox;
    }

    @Transactional
    public void handle(Message m) {
        if (inbox.seen(m.eventId())) return;
        switch (m.type()) {
            case CHARGE_PAYMENT -> {
                // order_id도 유일 키: eventId가 달라진 같은 결제 명령도 중복 청구하지 않는다.
                var states = db.queryForList("select status from payments where order_id = ? for update", String.class, m.orderId());
                String status;
                if (states.isEmpty()) {
                    status = m.rejectPayment() ? "REJECTED" : "CHARGED";
                    db.update("insert into payments(order_id, amount, status) values (?, ?, ?)", m.orderId(), m.amount(), status);
                } else { status = states.getFirst(); }
                // 이미 환불된 결제에 늦게 도착한 청구 명령은 무시한다.
                if (!status.equals("REFUNDED")) {
                    outbox.append(Topics.REPLIES, m.next(status.equals("CHARGED") ? PAYMENT_CHARGED : PAYMENT_REJECTED));
                }
            }
            case REFUND_PAYMENT -> {
                String status = db.queryForObject("select status from payments where order_id = ? for update", String.class, m.orderId());
                if (!"CHARGED".equals(status) && !"REFUNDED".equals(status)) {
                    throw new IllegalStateException("환불 가능한 결제가 아닙니다");
                }
                db.update("update payments set status = 'REFUNDED' where order_id = ? and status = 'CHARGED'", m.orderId());
                outbox.append(Topics.REPLIES, m.next(PAYMENT_REFUNDED));
            }
            default -> throw new IllegalArgumentException("예상하지 못한 결제 명령: " + m.type());
        }
        inbox.record(m.eventId());
    }
}
