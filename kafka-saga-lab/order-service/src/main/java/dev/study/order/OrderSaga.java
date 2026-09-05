package dev.study.order;

import java.util.Map;
import java.util.UUID;
import dev.study.common.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.server.ResponseStatusException;
import org.springframework.http.HttpStatus;
import static dev.study.common.Message.Type.*;

/** 이 클래스가 Saga orchestrator다. 각 서비스의 DB를 직접 수정하지 않고 명령을 발행한다. */
@Service
public class OrderSaga {
    private static final Logger log = LoggerFactory.getLogger(OrderSaga.class);
    public enum Status { PAYMENT_PENDING, STOCK_PENDING, COMPENSATING, COMPLETED, CANCELLED }
    private final JdbcTemplate db;
    private final Outbox outbox;
    private final Inbox inbox;
    public OrderSaga(JdbcTemplate db, Outbox outbox, Inbox inbox) {
        this.db = db; this.outbox = outbox; this.inbox = inbox;
    }

    @Transactional
    public String create(String productId, int quantity, long amount, boolean rejectPayment) {
        String id = UUID.randomUUID().toString();
        db.update("insert into orders(id, product_id, quantity, amount, status) values (?, ?, ?, ?, ?)",
                id, productId, quantity, amount, Status.PAYMENT_PENDING.name());
        outbox.append(Topics.PAYMENT, Message.start(id, productId, quantity, amount, rejectPayment));
        return id;
    }

    public Map<String, Object> get(String id) {
        var rows = db.queryForList("select * from orders where id = ?", id);
        if (rows.isEmpty()) throw new ResponseStatusException(HttpStatus.NOT_FOUND, "주문 없음");
        var row = rows.getFirst();
        return Map.of("orderId", row.get("ID"), "productId", row.get("PRODUCT_ID"),
                "quantity", row.get("QUANTITY"), "amount", row.get("AMOUNT"),
                "status", row.get("STATUS"), "reason", row.get("REASON") == null ? "" : row.get("REASON"));
    }

    @Transactional
    public void onReply(Message m) {
        // 주문 행 잠금으로 같은 주문에 대한 동시 상태 전이를 직렬화한다.
        String current = db.queryForObject("select status from orders where id = ? for update", String.class, m.orderId());
        if (inbox.seen(m.eventId())) return;
        Status status = Status.valueOf(current);
        switch (m.type()) {
            case PAYMENT_CHARGED -> {
                if (status == Status.PAYMENT_PENDING) {
                    move(m.orderId(), Status.STOCK_PENDING, null);
                    outbox.append(Topics.INVENTORY, m.next(RESERVE_STOCK));
                }
            }
            case PAYMENT_REJECTED -> {
                if (status == Status.PAYMENT_PENDING) move(m.orderId(), Status.CANCELLED, "PAYMENT_REJECTED");
            }
            case STOCK_RESERVED -> {
                if (status == Status.STOCK_PENDING) move(m.orderId(), Status.COMPLETED, null);
            }
            case STOCK_REJECTED -> {
                if (status == Status.STOCK_PENDING) {
                    move(m.orderId(), Status.COMPENSATING, "OUT_OF_STOCK");
                    outbox.append(Topics.PAYMENT, m.next(REFUND_PAYMENT));
                }
            }
            case PAYMENT_REFUNDED -> {
                // 환불 명령을 보낸 시점이 아니라 환불 완료를 확인한 시점에 취소한다.
                if (status == Status.COMPENSATING) move(m.orderId(), Status.CANCELLED, "OUT_OF_STOCK");
            }
            default -> throw new IllegalArgumentException("예상하지 못한 Saga 응답: " + m.type());
        }
        // 이미 지난 단계의 응답은 상태를 되돌리지 않는다. 미래 단계 메시지는 정상 생산자가 보내지 않는다.
        inbox.record(m.eventId());
    }

    private void move(String id, Status status, String reason) {
        db.update("update orders set status = ?, reason = ? where id = ?", status.name(), reason, id);
        log.info("상태 전이 시도 orderId={} status={} reason={} (DB 커밋은 메서드 종료 후)", id, status, reason);
    }
}
