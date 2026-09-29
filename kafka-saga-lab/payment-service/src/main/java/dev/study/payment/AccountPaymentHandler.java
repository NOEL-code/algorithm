/**
 * 학습: 원격 작업 요청과 성공 확정의 분리 PROCESSING에서 계좌 응답을 기다린 뒤 CHARGED/REJECTED로 바뀐다. DB에 명령을 저장했다고 차감이 끝난 것은
 * 아니다. REFUND_PENDING도 같은 원리다. 주문 서비스에는 계좌 환불 확인 후 PAYMENT_REFUNDED를 보낸다. 원장과 메시지의 회원·계좌·금액 일치 검사는
 * 같은 주문 ID에 다른 요청을 섞는 오류를 탐지한다.
 */
package dev.study.payment;

import static dev.study.common.Message.Type.*;

import dev.study.common.*;

import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.Objects;

@Service
public class AccountPaymentHandler {
    private final JdbcTemplate db;
    private final Inbox inbox;
    private final Outbox outbox;

    public AccountPaymentHandler(JdbcTemplate db, Inbox inbox, Outbox outbox) {
        this.db = db;
        this.inbox = inbox;
        this.outbox = outbox;
    }

    @Transactional
    public void handle(Message m) {
        if (inbox.seen(m.eventId())) return;
        // 존재하는 결제 행은 잠글 수 있다. 첫 INSERT 경쟁은 order_id PK 위반과 전체 rollback/재시도로 처리한다.
        var rows =
                db.queryForList("select * from payments where order_id=? for update", m.orderId());
        // 최초 명령에서만 PROCESSING 원장과 계좌 차감 명령을 만든다. 같은 DB 트랜잭션이라 한쪽만 남지 않는다.
        if (rows.isEmpty()) {
            if (m.type() != CHARGE_PAYMENT) throw new IllegalStateException("결제 원장이 없습니다");
            db.update(
                    "insert into payments(order_id,amount,status,member_id,account_id)"
                        + " values(?,?,?,?,?)",
                    m.orderId(),
                    m.amount(),
                    m.rejectPayment() ? "REJECTED" : "PROCESSING",
                    m.memberId(),
                    m.accountId());
            outbox.append(
                    m.rejectPayment() ? Topics.REPLIES : Topics.ACCOUNT,
                    m.next(m.rejectPayment() ? PAYMENT_REJECTED : DEBIT_ACCOUNT));
        } else {
            var row = rows.getFirst();
            if (!Objects.equals(row.get("ACCOUNT_ID"), m.accountId())
                    || !Objects.equals(row.get("MEMBER_ID"), m.memberId())
                    || ((Number) row.get("AMOUNT")).longValue() != m.amount())
                throw new IllegalArgumentException("결제 원장과 명령 불일치");
            String status = (String) row.get("STATUS");
            // 현재 상태의 허용 전이만 실행한다. 정상 생산자의 인과 순서가 전제이며 임의 미래 이벤트 복구까지 제공하지 않는다.
            switch (m.type()) {
                case CHARGE_PAYMENT -> {
                    if (status.equals("CHARGED"))
                        outbox.append(Topics.REPLIES, m.next(PAYMENT_CHARGED));
                    if (status.equals("REJECTED"))
                        outbox.append(Topics.REPLIES, m.next(PAYMENT_REJECTED));
                }
                case ACCOUNT_DEBITED -> {
                    if (status.equals("PROCESSING")) result(m, "CHARGED", PAYMENT_CHARGED);
                }
                case ACCOUNT_REJECTED -> {
                    if (status.equals("PROCESSING")) result(m, "REJECTED", PAYMENT_REJECTED);
                }
                // 계좌의 환불 완료 전에는 REFUNDED로 표시하지 않는다. 원격 작업은 이 DB의 잠금으로 원자화할 수 없다.
                case REFUND_PAYMENT -> {
                    if (status.equals("CHARGED")) {
                        db.update(
                                "update payments set status='REFUND_PENDING' where order_id=?",
                                m.orderId());
                        outbox.append(Topics.ACCOUNT, m.next(CREDIT_ACCOUNT));
                    } else if (status.equals("REFUNDED"))
                        outbox.append(Topics.REPLIES, m.next(PAYMENT_REFUNDED));
                    else if (!status.equals("REFUND_PENDING"))
                        throw new IllegalStateException("환불 불가능 상태");
                }
                case ACCOUNT_CREDITED -> {
                    if (status.equals("REFUND_PENDING")) result(m, "REFUNDED", PAYMENT_REFUNDED);
                }
                default -> throw new IllegalArgumentException("지원하지 않는 결제 메시지");
            }
        }
        inbox.record(m.eventId());
    }

    // 결제 상태 변경과 Saga 응답 발행 의도를 같은 트랜잭션으로 묶는 작은 반복 동작이다.
    private void result(Message m, String status, Message.Type reply) {
        db.update("update payments set status=? where order_id=?", status, m.orderId());
        outbox.append(Topics.REPLIES, m.next(reply));
    }
}
