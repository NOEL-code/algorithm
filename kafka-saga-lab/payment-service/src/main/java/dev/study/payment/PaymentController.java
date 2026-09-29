/**
 * 학습: 조회 API에서도 필요한 소유권 검사 인증된 회원 ID와 주문 ID를 함께 WHERE에 넣는다. 다른 회원의 결제 존재 여부도 404로 감춘다. 결제 상태는 별도
 * DB의 조회 결과라 주문 상태와 동시에 바뀌지 않을 수 있다. 프론트엔드의 결제 404 대기 처리와 비교해 최종 일관성을 설명한다.
 */
package dev.study.payment;

import org.springframework.http.HttpStatus;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;

import java.util.Map;

@RestController
public class PaymentController {
    private final JdbcTemplate db;
    private final dev.study.common.MemberSessions sessions;

    public PaymentController(JdbcTemplate db, dev.study.common.MemberSessions sessions) {
        this.db = db;
        this.sessions = sessions;
    }

    @GetMapping("/payments/{orderId}")
    public Map<String, Object> get(
            @CookieValue(name = "SAGA_SESSION", required = false) String token,
            @PathVariable String orderId) {
        String member = sessions.authenticate(token);
        var rows =
                db.queryForList(
                        "select * from payments where order_id = ? and member_id=?",
                        orderId,
                        member);
        if (rows.isEmpty()) throw new ResponseStatusException(HttpStatus.NOT_FOUND);
        var row = rows.getFirst();
        return Map.of(
                "orderId",
                row.get("ORDER_ID"),
                "amount",
                row.get("AMOUNT"),
                "status",
                row.get("STATUS"));
    }
}
