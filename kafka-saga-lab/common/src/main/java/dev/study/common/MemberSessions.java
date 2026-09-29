/**
 * 학습: 인증 정보의 신뢰 경계와 가용성 비용 클라이언트가 보낸 회원 ID 대신 회원 서비스가 검증한 세션의 회원 ID를 사용한다. 회원 서비스 장애는 503, 무효 세션은
 * 401로 나눈다. 장애 시 임의로 인증을 통과시키지 않는다. 매 요청 HTTP 확인은 즉시 폐기와 단순성을 얻지만 지연/가용성 의존성이 생긴다. JWT/캐시와 비교할
 * 출발점이다.
 */
package dev.study.common;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.*;
import org.springframework.http.client.JdkClientHttpRequestFactory;
import org.springframework.stereotype.Component;
import org.springframework.web.client.*;
import org.springframework.web.server.ResponseStatusException;

import java.net.http.HttpClient;
import java.time.Duration;

/** Validate opaque member sessions without trusting caller-supplied member IDs. */
@Component
public class MemberSessions {
    public record Member(String id, String email, String name) {}

    private final RestClient client;

    public MemberSessions(@Value("${lab.member-url:http://localhost:8083}") String url) {
        var factory =
                new JdkClientHttpRequestFactory(
                        HttpClient.newBuilder().connectTimeout(Duration.ofSeconds(3)).build());
        factory.setReadTimeout(Duration.ofSeconds(5));
        client = RestClient.builder().baseUrl(url).requestFactory(factory).build();
    }

    public String authenticate(String token) {
        if (token == null || !token.matches("[A-Za-z0-9_-]{43}"))
            throw new ResponseStatusException(HttpStatus.UNAUTHORIZED, "로그인이 필요합니다.");
        try {
            Member member =
                    client.get()
                            .uri("/members/me")
                            .header(HttpHeaders.COOKIE, "SAGA_SESSION=" + token)
                            .retrieve()
                            .body(Member.class);
            if (member == null || member.id() == null)
                throw new ResponseStatusException(HttpStatus.SERVICE_UNAVAILABLE, "회원 확인 실패");
            return member.id();
        } catch (HttpClientErrorException.Unauthorized e) {
            throw new ResponseStatusException(HttpStatus.UNAUTHORIZED, "로그인이 필요합니다.");
        } catch (RestClientException e) {
            throw new ResponseStatusException(HttpStatus.SERVICE_UNAVAILABLE, "회원 서비스 연결 실패");
        }
    }
}
