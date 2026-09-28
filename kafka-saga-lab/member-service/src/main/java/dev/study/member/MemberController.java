package dev.study.member;

import java.time.Duration;
import jakarta.validation.Valid;
import jakarta.validation.constraints.*;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.*;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/members")
public class MemberController {
    public record Register(@NotBlank @Email @Size(max=254) String email, @NotBlank @Size(max=60) String name, @NotBlank @Size(min=10,max=128) String password) {}
    public record Credentials(@NotBlank @Email @Size(max=254) String email, @NotBlank @Size(max=128) String password) {}
    public record Profile(@NotBlank @Size(max=60) String name) {}
    public record PasswordChange(@NotBlank @Size(max=128) String currentPassword, @NotBlank @Size(min=10,max=128) String newPassword) {}
    private final MemberService service;
    private final boolean secure;
    public MemberController(MemberService service, @Value("${lab.secure-cookie:false}") boolean secure) { this.service=service; this.secure=secure; }
    @PostMapping
    public ResponseEntity<MemberService.Member> register(@Valid @RequestBody Register input) {
        var login = service.register(input.email(), input.name(), input.password());
        return ResponseEntity.status(201).header(HttpHeaders.SET_COOKIE, cookie(login.token(), 86400)).body(login.member());
    }
    @PostMapping("/sessions")
    public ResponseEntity<MemberService.Member> login(@Valid @RequestBody Credentials input) {
        var login = service.login(input.email(), input.password());
        return ResponseEntity.ok().header(HttpHeaders.SET_COOKIE, cookie(login.token(), 86400)).body(login.member());
    }
    @DeleteMapping("/sessions/current")
    public ResponseEntity<Void> logout(@CookieValue(name="SAGA_SESSION",required=false) String token) {
        service.logout(token);
        return ResponseEntity.noContent().header(HttpHeaders.SET_COOKIE, cookie("",0)).build();
    }
    @GetMapping("/me")
    public ResponseEntity<MemberService.Member> me(@CookieValue(name="SAGA_SESSION",required=false) String token) {
        return ResponseEntity.ok().cacheControl(CacheControl.noStore()).body(service.current(token));
    }
    @PatchMapping("/me")
    public MemberService.Member update(@CookieValue(name="SAGA_SESSION",required=false) String token, @Valid @RequestBody Profile input) { return service.update(token,input.name()); }
    @PutMapping("/me/password")
    public ResponseEntity<Void> password(@CookieValue(name="SAGA_SESSION",required=false) String token, @Valid @RequestBody PasswordChange input) {
        service.changePassword(token,input.currentPassword(),input.newPassword());
        return ResponseEntity.noContent().header(HttpHeaders.SET_COOKIE,cookie("",0)).build();
    }
    private String cookie(String value, long age) {
        return ResponseCookie.from("SAGA_SESSION",value).httpOnly(true).secure(secure).sameSite("Strict").path("/").maxAge(Duration.ofSeconds(age)).build().toString();
    }
}
