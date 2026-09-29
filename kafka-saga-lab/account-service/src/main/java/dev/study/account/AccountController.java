/**
 * 학습: 인증된 주체에서 시작하는 API memberId를 요청 본문으로 받지 않고 세션 확인 결과를 Service에 넘긴다. UUID를 안다고 권한이 생기지는 않는다.
 * 입금/출금과 별칭 변경/해지는 다른 업무이므로 명시적인 API로 분리한다. @Valid는 HTTP 경계 검증이다. AccountRules의 서비스 내부 방어 및 DB
 * CHECK와 비교한다.
 */
package dev.study.account;

import dev.study.common.MemberSessions;

import jakarta.validation.Valid;
import jakarta.validation.constraints.*;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/accounts")
public class AccountController {
    public record Name(@NotBlank @Size(max = 60) String name) {}

    public record Transaction(
            @NotNull @Pattern(regexp = "DEPOSIT|WITHDRAW") String type,
            @Min(1) @Max(1_000_000_000) long amount,
            @NotNull
                    @Pattern(
                            regexp =
                                    "[a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}")
                    String requestId) {}

    private final MemberSessions members;
    private final AccountService service;

    public AccountController(MemberSessions members, AccountService service) {
        this.members = members;
        this.service = service;
    }

    @GetMapping("/{id}")
    public AccountService.Account get(
            @CookieValue(name = "SAGA_SESSION", required = false) String token,
            @PathVariable String id) {
        return service.get(members.authenticate(token), id);
    }

    @GetMapping
    public List<AccountService.Account> list(
            @CookieValue(name = "SAGA_SESSION", required = false) String token) {
        return service.list(members.authenticate(token));
    }

    @PostMapping
    @ResponseStatus(HttpStatus.CREATED)
    public AccountService.Account create(
            @CookieValue(name = "SAGA_SESSION", required = false) String token,
            @Valid @RequestBody Name body) {
        return service.create(members.authenticate(token), body.name());
    }

    @PatchMapping("/{id}")
    public AccountService.Account rename(
            @CookieValue(name = "SAGA_SESSION", required = false) String token,
            @PathVariable String id,
            @Valid @RequestBody Name body) {
        return service.rename(members.authenticate(token), id, body.name());
    }

    @DeleteMapping("/{id}")
    @ResponseStatus(HttpStatus.NO_CONTENT)
    public void close(
            @CookieValue(name = "SAGA_SESSION", required = false) String token,
            @PathVariable String id) {
        service.close(members.authenticate(token), id);
    }

    @PostMapping("/{id}/transactions")
    public AccountService.Entry transact(
            @CookieValue(name = "SAGA_SESSION", required = false) String token,
            @PathVariable String id,
            @Valid @RequestBody Transaction body) {
        return service.transact(
                members.authenticate(token), id, body.type(), body.amount(), body.requestId());
    }

    @GetMapping("/{id}/transactions")
    public List<AccountService.Entry> history(
            @CookieValue(name = "SAGA_SESSION", required = false) String token,
            @PathVariable String id) {
        return service.history(members.authenticate(token), id);
    }
}
