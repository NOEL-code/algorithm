package dev.study.account;

import java.util.List;
import jakarta.validation.Valid;
import jakarta.validation.constraints.*;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/accounts")
public class AccountController {
    public record Name(@NotBlank @Size(max=60) String name) {}
    public record Transaction(@NotNull @Pattern(regexp="DEPOSIT|WITHDRAW") String type, @Min(1) @Max(1_000_000_000) long amount, @NotNull @Pattern(regexp="[a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}") String requestId) {}
    private final MemberClient members;
    private final AccountService service;
    public AccountController(MemberClient members, AccountService service) { this.members=members; this.service=service; }
    @GetMapping
    public List<AccountService.Account> list(@CookieValue(name="SAGA_SESSION",required=false) String token) { return service.list(members.authenticate(token)); }
    @PostMapping @ResponseStatus(HttpStatus.CREATED)
    public AccountService.Account create(@CookieValue(name="SAGA_SESSION",required=false) String token, @Valid @RequestBody Name body) { return service.create(members.authenticate(token),body.name()); }
    @PatchMapping("/{id}")
    public AccountService.Account rename(@CookieValue(name="SAGA_SESSION",required=false) String token, @PathVariable String id, @Valid @RequestBody Name body) { return service.rename(members.authenticate(token),id,body.name()); }
    @DeleteMapping("/{id}") @ResponseStatus(HttpStatus.NO_CONTENT)
    public void close(@CookieValue(name="SAGA_SESSION",required=false) String token, @PathVariable String id) { service.close(members.authenticate(token),id); }
    @PostMapping("/{id}/transactions")
    public AccountService.Entry transact(@CookieValue(name="SAGA_SESSION",required=false) String token, @PathVariable String id, @Valid @RequestBody Transaction body) { return service.transact(members.authenticate(token),id,body.type(),body.amount(),body.requestId()); }
    @GetMapping("/{id}/transactions")
    public List<AccountService.Entry> history(@CookieValue(name="SAGA_SESSION",required=false) String token, @PathVariable String id) { return service.history(members.authenticate(token),id); }
}
