/**
 * 학습: 서비스 간 보상과 계좌 로컬 원자성 잔액·주문별 계좌 원장·거래 내역·Inbox·Outbox를 하나의 계좌 DB 트랜잭션으로 처리한다. 결제/환불은 orderId 업무
 * 원장으로 중복을 막는다. eventId가 달라도 금전 효과는 반복하지 않는다. 환불은 이전 DB 트랜잭션의 rollback이 아니라 CREDIT_ACCOUNT라는 새 거래다.
 */
package dev.study.account;

import static dev.study.common.Message.Type.*;

import dev.study.common.*;

import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.*;

@Service
public class AccountOrderHandler {
    private final JdbcTemplate db;
    private final Inbox inbox;
    private final Outbox outbox;

    public AccountOrderHandler(JdbcTemplate db, Inbox inbox, Outbox outbox) {
        this.db = db;
        this.inbox = inbox;
        this.outbox = outbox;
    }

    @Transactional
    public void handle(Message m) {
        if (m.accountId() == null
                || m.memberId() == null
                || m.amount() < 1
                || m.amount() > 1_000_000_000L) throw new IllegalArgumentException("유효하지 않은 계좌 명령");
        // 모든 변경 경로가 계좌 행부터 잠그는 규약을 공유한다. 서로 다른 orderId도 같은 계좌에서는 직렬화된다.
        var accounts =
                db.queryForList("select * from accounts where id=? for update", m.accountId());
        if (inbox.seen(m.eventId())) return;
        // eventId 중복 검사와 별개로 주문별 원장을 확인한다. 같은 orderId에 다른 계좌/금액을 끼우면 계약 오류다.
        var ledger = db.queryForList("select * from account_orders where order_id=?", m.orderId());
        String status = null;
        if (!ledger.isEmpty()) {
            var row = ledger.getFirst();
            if (!Objects.equals(row.get("ACCOUNT_ID"), m.accountId())
                    || !Objects.equals(row.get("MEMBER_ID"), m.memberId())
                    || ((Number) row.get("AMOUNT")).longValue() != m.amount())
                throw new IllegalArgumentException("계좌 원장과 명령 불일치");
            status = (String) row.get("STATUS");
        }
        switch (m.type()) {
            case DEBIT_ACCOUNT -> {
                // 최초 차감만 잔액을 바꾼다. 잔액 부족/해지/소유권 불일치는 재시도 예외 대신 거절 결과로 남긴다.
                if (status == null) {
                    boolean allowed =
                            !accounts.isEmpty()
                                    && Objects.equals(
                                            accounts.getFirst().get("MEMBER_ID"), m.memberId())
                                    && "OPEN".equals(accounts.getFirst().get("STATUS"))
                                    && ((Number) accounts.getFirst().get("BALANCE")).longValue()
                                            >= m.amount();
                    status = allowed ? "DEBITED" : "REJECTED";
                    db.update(
                            "insert into"
                                + " account_orders(order_id,account_id,member_id,amount,status)"
                                + " values(?,?,?,?,?)",
                            m.orderId(),
                            m.accountId(),
                            m.memberId(),
                            m.amount(),
                            status);
                    if (allowed) change(m, -m.amount(), "PAYMENT");
                }
                if (status.equals("DEBITED") || status.equals("SETTLED"))
                    outbox.append(Topics.PAYMENT, m.next(ACCOUNT_DEBITED));
                if (status.equals("REJECTED"))
                    outbox.append(Topics.PAYMENT, m.next(ACCOUNT_REJECTED));
            }
            // 중복 환불 명령에는 완료 응답을 다시 보내되 잔액은 DEBITED→REFUNDED 한 번만 복원한다.
            case CREDIT_ACCOUNT -> {
                if ("DEBITED".equals(status)) {
                    change(m, m.amount(), "REFUND");
                    db.update(
                            "update account_orders set status='REFUNDED' where order_id=?",
                            m.orderId());
                } else if (!"REFUNDED".equals(status))
                    throw new IllegalStateException("환불할 계좌 거래가 없습니다");
                outbox.append(Topics.PAYMENT, m.next(ACCOUNT_CREDITED));
            }
            // 재고 예약 성공 후 차감을 확정하여 환불 예약 공간을 해제한다. 이 예제에는 완료 후 취소 정책이 없다.
            case SETTLE_ACCOUNT -> {
                if ("DEBITED".equals(status))
                    db.update(
                            "update account_orders set status='SETTLED' where order_id=?",
                            m.orderId());
                else if (!"SETTLED".equals(status))
                    throw new IllegalStateException("확정할 계좌 거래가 없습니다");
            }
            default -> throw new IllegalArgumentException("예상하지 못한 계좌 명령");
        }
        // Inbox까지 같은 트랜잭션이다. 실패하면 금전 효과/Outbox도 함께 rollback한다.
        inbox.record(m.eventId());
    }

    private void change(Message m, long delta, String type) {
        int changed =
                db.update(
                        "update accounts set balance=balance+? where id=? and member_id=? and"
                            + " status='OPEN'",
                        delta,
                        m.accountId(),
                        m.memberId());
        if (changed != 1) throw new IllegalStateException("계좌 상태 확인 필요");
        long balance =
                db.queryForObject(
                        "select balance from accounts where id=?", Long.class, m.accountId());
        // 주문 ID + 작업 종류로 원장 키를 고정한다. 차감과 환불은 서로 다른 거래이며 개인정보 인증 토큰이 아니다.
        String key =
                UUID.nameUUIDFromBytes(
                                (m.orderId() + ":" + type)
                                        .getBytes(java.nio.charset.StandardCharsets.UTF_8))
                        .toString();
        db.update(
                "insert into"
                    + " account_transactions(id,account_id,request_id,type,amount,balance_after)"
                    + " values(?,?,?,?,?,?)",
                UUID.randomUUID().toString(),
                m.accountId(),
                key,
                type,
                m.amount(),
                balance);
    }
}
