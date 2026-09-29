/**
 * 학습: 인증의 HTTP 경계 DTO는 입력 형식, Service는 업무 흐름을 맡는다. 회원 생성은 201, 상태 없는 완료는 204다. 쿠키를 응답 본문과 분리하고
 * HttpOnly/SameSite/Secure의 서로 다른 역할을 배운다. MemberService.current와 MemberSessions를 따라가 인증과 계좌 소유권
 * 인가를 구분한다.
 */
package dev.study.member;

import jakarta.validation.Valid;
import jakarta.validation.constraints.*;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.*;
import org.springframework.web.bind.annotation.*;

import java.time.Duration;

@RestController
@RequestMapping("/members")
public class MemberController {
    public record Register(
            @NotBlank @Email @Size(max = 254) String email,
            @NotBlank @Size(max = 60) String name,
            @NotBlank @Size(min = 10, max = 128) String password) {}

    public record Credentials(
            @NotBlank @Email @Size(max = 254) String email,
            @NotBlank @Size(max = 128) String password) {}

    public record Profile(@NotBlank @Size(max = 60) String name) {}

    public record PasswordChange(
            @NotBlank @Size(max = 128) String currentPassword,
            @NotBlank @Size(min = 10, max = 128) String newPassword) {}

    private final MemberService service;
    private final boolean secure;

    public MemberController(
            MemberService service, @Value("${lab.secure-cookie:false}") boolean secure) {
        this.service = service;
        this.secure = secure;
    }

    // 생성과 자동 로그인을 함께 반환하지만 토큰은 HttpOnly 쿠키에만 담는다.
    @PostMapping
    public ResponseEntity<MemberService.Member> register(@Valid @RequestBody Register input) {
        var login = service.register(input.email(), input.name(), input.password());
        return ResponseEntity.status(201)
                .header(HttpHeaders.SET_COOKIE, cookie(login.token(), 86400))
                .body(login.member());
    }

    @PostMapping("/sessions")
    public ResponseEntity<MemberService.Member> login(@Valid @RequestBody Credentials input) {
        var login = service.login(input.email(), input.password());
        return ResponseEntity.ok()
                .header(HttpHeaders.SET_COOKIE, cookie(login.token(), 86400))
                .body(login.member());
    }

    // 브라우저 쿠키 삭제만으로는 탈취된 토큰을 폐기할 수 없어 DB 세션도 삭제한다.
    @DeleteMapping("/sessions/current")
    public ResponseEntity<Void> logout(
            @CookieValue(name = "SAGA_SESSION", required = false) String token) {
        service.logout(token);
        return ResponseEntity.noContent().header(HttpHeaders.SET_COOKIE, cookie("", 0)).build();
    }

    // 개인정보 응답 캐시를 금지한다. 인증은 누구인지 확인하고, 이후 각 서비스가 소유권을 검사한다.
    @GetMapping("/me")
    public ResponseEntity<MemberService.Member> me(
            @CookieValue(name = "SAGA_SESSION", required = false) String token) {
        return ResponseEntity.ok()
                .cacheControl(CacheControl.noStore())
                .body(service.current(token));
    }

    @PatchMapping("/me")
    public MemberService.Member update(
            @CookieValue(name = "SAGA_SESSION", required = false) String token,
            @Valid @RequestBody Profile input) {
        return service.update(token, input.name());
    }

    // 현재 비밀번호를 재확인하고 모든 기존 세션을 폐기한다. 변경 완료 후 재로그인이 필요하다.
    @PutMapping("/me/password")
    public ResponseEntity<Void> password(
            @CookieValue(name = "SAGA_SESSION", required = false) String token,
            @Valid @RequestBody PasswordChange input) {
        service.changePassword(token, input.currentPassword(), input.newPassword());
        return ResponseEntity.noContent().header(HttpHeaders.SET_COOKIE, cookie("", 0)).build();
    }

    // HttpOnly는 JS 읽기 제한, SameSite는 교차 사이트 전송 제한, Secure는 HTTPS 전송 제한이다.
    // 이 설정 하나가 모든 XSS/CSRF를 해결하지 않는다. 이 예제는 JSON API와 동일 출처 프록시를 함께 사용한다.
    private String cookie(String value, long age) {
        return ResponseCookie.from("SAGA_SESSION", value)
                .httpOnly(true)
                .secure(secure)
                .sameSite("Strict")
                .path("/")
                .maxAge(Duration.ofSeconds(age))
                .build()
                .toString();
    }
}
