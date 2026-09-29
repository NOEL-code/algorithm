/**
 * 학습: 과거 메시지 호환 경로와 현재 경로의 구분 새 HTTP 주문은 AccountPaymentHandler로 보내 실제 가상 잔액을 사용한다. 계좌 ID 없는 분기는 기존
 * 데이터/기초 Saga 테스트를 위한 상태만의 결제 모형이다. 새 API에서는 생성하지 않는다. 호환 분기를 운영 기능으로 오해하지 말 것. 제거하려면 기존 메시지·DB
 * 마이그레이션 전략이 필요하다.
 */
package dev.study.payment;

import static dev.study.common.Message.Type.*;

import dev.study.common.*;

import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

@Service
public class PaymentHandler {
    private final JdbcTemplate db;
    private final Inbox inbox;
    private final Outbox outbox;
    private final AccountPaymentHandler accountPayments;

    public PaymentHandler(
            JdbcTemplate db, Inbox inbox, Outbox outbox, AccountPaymentHandler accountPayments) {
        this.db = db;
        this.inbox = inbox;
        this.outbox = outbox;
        this.accountPayments = accountPayments;
    }

    @Transactional
    public void handle(Message m) {
        if (m.accountId() != null) {
            accountPayments.handle(m);
            return;
        }
        if (inbox.seen(m.eventId())) return;
        switch (m.type()) {
            case CHARGE_PAYMENT -> {
                // order_id도 유일 키: eventId가 달라진 같은 결제 명령도 중복 청구하지 않는다.
                var states =
                        db.queryForList(
                                "select status from payments where order_id = ? for update",
                                String.class,
                                m.orderId());
                String status;
                if (states.isEmpty()) {
                    status = m.rejectPayment() ? "REJECTED" : "CHARGED";
                    db.update(
                            "insert into payments(order_id, amount, status) values (?, ?, ?)",
                            m.orderId(),
                            m.amount(),
                            status);
                } else {
                    status = states.getFirst();
                }
                // 이미 환불된 결제에 늦게 도착한 청구 명령은 무시한다.
                if (!status.equals("REFUNDED")) {
                    outbox.append(
                            Topics.REPLIES,
                            m.next(status.equals("CHARGED") ? PAYMENT_CHARGED : PAYMENT_REJECTED));
                }
            }
            case REFUND_PAYMENT -> {
                String status =
                        db.queryForObject(
                                "select status from payments where order_id = ? for update",
                                String.class,
                                m.orderId());
                if (!"CHARGED".equals(status) && !"REFUNDED".equals(status)) {
                    throw new IllegalStateException("환불 가능한 결제가 아닙니다");
                }
                db.update(
                        "update payments set status = 'REFUNDED' where order_id = ? and status ="
                            + " 'CHARGED'",
                        m.orderId());
                outbox.append(Topics.REPLIES, m.next(PAYMENT_REFUNDED));
            }
            default -> throw new IllegalArgumentException("예상하지 못한 결제 명령: " + m.type());
        }
        inbox.record(m.eventId());
    }
}
