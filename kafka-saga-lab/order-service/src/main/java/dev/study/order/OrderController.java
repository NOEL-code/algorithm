/**
 * 학습: 접수 성공과 업무 완료의 차이 HTTP 202와 Location은 주문 접수 사실만 알린다. 프론트엔드는 주문 상태를 별도로 조회한다. 계좌 서비스의 사전 확인은
 * 사용자에게 빠른 오류를 주는 용도다. 이후 계좌가 닫힐 수 있어 소비 시에도 재검증한다. 조회도 getOwned를 거쳐 소유권을 검사한다. 서버가 발급한 UUID만으로 접근
 * 제어를 대체하지 않는다.
 */
package dev.study.order;

import dev.study.common.MemberSessions;

import jakarta.validation.Valid;
import jakarta.validation.constraints.*;

import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.net.URI;
import java.util.Map;

@RestController
@RequestMapping("/orders")
public class OrderController {
    public record CreateOrder(
            @NotBlank @Size(max = 100) String productId,
            @Min(1) int quantity,
            @Min(1) @Max(1_000_000_000) long amount,
            boolean rejectPayment,
            @NotBlank @Pattern(regexp = "[a-fA-F0-9-]{36}") String accountId) {}

    private final OrderSaga saga;
    private final MemberSessions sessions;
    private final OrderAccounts accounts;

    public OrderController(OrderSaga saga, MemberSessions sessions, OrderAccounts accounts) {
        this.saga = saga;
        this.sessions = sessions;
        this.accounts = accounts;
    }

    @PostMapping
    public ResponseEntity<Map<String, String>> create(
            @CookieValue(name = "SAGA_SESSION", required = false) String token,
            @Valid @RequestBody CreateOrder request) {
        String member = sessions.authenticate(token);
        accounts.requireOwnedOpen(token, request.accountId());
        String id =
                saga.create(
                        request.productId(),
                        request.quantity(),
                        request.amount(),
                        request.rejectPayment(),
                        member,
                        request.accountId());
        return ResponseEntity.accepted()
                .location(URI.create("/orders/" + id))
                .body(Map.of("orderId", id, "status", "PAYMENT_PENDING"));
    }

    @GetMapping("/{id}")
    public Map<String, Object> get(
            @CookieValue(name = "SAGA_SESSION", required = false) String token,
            @PathVariable String id) {
        return saga.getOwned(id, sessions.authenticate(token));
    }
}
