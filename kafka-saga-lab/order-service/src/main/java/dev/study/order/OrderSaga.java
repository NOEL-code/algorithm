/**
 * 학습: 명시적 상태 머신으로 표현하는 Orchestration Saga 주문 서비스는 다음 명령을 결정하고 각 서비스가 자기 DB를 수정하도록 한다. 전역 트랜잭션은 없다.
 * 현재 상태 + 받은 이벤트로 전이를 제한하고 행 잠금으로 같은 주문의 경쟁을 직렬화한다. 환불 요청과 환불 완료를 분리한다. COMPENSATING은 실패를 숨기지 않는 업무
 * 상태다.
 */
package dev.study.order;

import static dev.study.common.Message.Type.*;

import dev.study.common.*;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.HttpStatus;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.server.ResponseStatusException;

import java.util.Map;
import java.util.UUID;

/** 이 클래스가 Saga orchestrator다. 각 서비스의 DB를 직접 수정하지 않고 명령을 발행한다. */
@Service
public class OrderSaga {
    private static final Logger log = LoggerFactory.getLogger(OrderSaga.class);

    public enum Status {
        PAYMENT_PENDING,
        STOCK_PENDING,
        COMPENSATING,
        COMPLETED,
        CANCELLED
    }

    private final JdbcTemplate db;
    private final Outbox outbox;
    private final Inbox inbox;

    public OrderSaga(JdbcTemplate db, Outbox outbox, Inbox inbox) {
        this.db = db;
        this.outbox = outbox;
        this.inbox = inbox;
    }

    @Transactional
    // 계좌 없는 기초 실습/기존 메시지 호환 진입점이며 HTTP Controller에서는 호출하지 않는다.
    // self-invocation으로 아래 overload를 불러도 새 프록시는 통과하지 않는다. 이 메서드 자체의 트랜잭션이 유지된다.
    public String create(String productId, int quantity, long amount, boolean rejectPayment) {
        return create(productId, quantity, amount, rejectPayment, null, null);
    }

    @Transactional
    public String create(
            String productId,
            int quantity,
            long amount,
            boolean rejectPayment,
            String memberId,
            String accountId) {
        String id = UUID.randomUUID().toString();
        // 처음부터 소유권까지 같은 INSERT에 담는다. 즉시 UPDATE하는 불필요한 DB 왕복을 줄인다.
        db.update(
                "insert into orders(id, product_id, quantity, amount, status, member_id,"
                    + " account_id) values (?, ?, ?, ?, ?, ?, ?)",
                id,
                productId,
                quantity,
                amount,
                Status.PAYMENT_PENDING.name(),
                memberId,
                accountId);
        outbox.append(
                Topics.PAYMENT,
                new Message(
                        UUID.randomUUID().toString(),
                        id,
                        CHARGE_PAYMENT,
                        productId,
                        quantity,
                        amount,
                        rejectPayment,
                        memberId,
                        accountId));
        return id;
    }

    public Map<String, Object> get(String id) {
        var rows = db.queryForList("select * from orders where id = ?", id);
        if (rows.isEmpty()) throw new ResponseStatusException(HttpStatus.NOT_FOUND, "주문 없음");
        var row = rows.getFirst();
        return Map.of(
                "orderId",
                row.get("ID"),
                "productId",
                row.get("PRODUCT_ID"),
                "quantity",
                row.get("QUANTITY"),
                "amount",
                row.get("AMOUNT"),
                "status",
                row.get("STATUS"),
                "reason",
                row.get("REASON") == null ? "" : row.get("REASON"),
                "accountId",
                row.get("ACCOUNT_ID") == null ? "" : row.get("ACCOUNT_ID"));
    }

    // 회원 ID가 불변인 이 예제에서는 소유권 확인 후 조회한다. 소유권 이전을 도입하면 같은 SELECT/스냅샷으로 합쳐야 한다.
    public Map<String, Object> getOwned(String id, String memberId) {
        Integer count =
                db.queryForObject(
                        "select count(*) from orders where id=? and member_id=?",
                        Integer.class,
                        id,
                        memberId);
        if (count == null || count == 0)
            throw new ResponseStatusException(HttpStatus.NOT_FOUND, "주문 없음");
        return get(id);
    }

    @Transactional
    public void onReply(Message m) {
        // 주문 행 잠금으로 같은 주문에 대한 동시 상태 전이를 직렬화한다.
        String current =
                db.queryForObject(
                        "select status from orders where id = ? for update",
                        String.class,
                        m.orderId());
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
                if (status == Status.PAYMENT_PENDING)
                    move(m.orderId(), Status.CANCELLED, "PAYMENT_REJECTED");
            }
            // 주문 완료와 계좌 정산 메시지 발행 의도를 함께 저장한다. COMPLETED 직후에도 계좌 해지는 정산 소비를 기다릴 수 있다.
            case STOCK_RESERVED -> {
                if (status == Status.STOCK_PENDING) {
                    move(m.orderId(), Status.COMPLETED, null);
                    if (m.accountId() != null)
                        outbox.append(Topics.ACCOUNT, m.next(SETTLE_ACCOUNT));
                }
            }
            // 이미 커밋된 계좌 차감을 되돌리는 새 명령이다. 주문 DB rollback으로 원격 계좌 잔액은 복원되지 않는다.
            case STOCK_REJECTED -> {
                if (status == Status.STOCK_PENDING) {
                    move(m.orderId(), Status.COMPENSATING, "OUT_OF_STOCK");
                    outbox.append(Topics.PAYMENT, m.next(REFUND_PAYMENT));
                }
            }
            case PAYMENT_REFUNDED -> {
                // 환불 명령을 보낸 시점이 아니라 환불 완료를 확인한 시점에 취소한다.
                if (status == Status.COMPENSATING)
                    move(m.orderId(), Status.CANCELLED, "OUT_OF_STOCK");
            }
            default -> throw new IllegalArgumentException("예상하지 못한 Saga 응답: " + m.type());
        }
        // 이미 지난 단계의 응답은 상태를 되돌리지 않는다. 미래 단계 메시지는 정상 생산자가 보내지 않는다.
        inbox.record(m.eventId());
    }

    // 이 로그는 커밋 전에 출력된다. 로그 한 줄만 보고 업무 성공을 확정하면 안 된다.
    private void move(String id, Status status, String reason) {
        db.update(
                "update orders set status = ?, reason = ? where id = ?", status.name(), reason, id);
        log.info("상태 전이 시도 orderId={} status={} reason={} (DB 커밋은 메서드 종료 후)", id, status, reason);
    }
}
