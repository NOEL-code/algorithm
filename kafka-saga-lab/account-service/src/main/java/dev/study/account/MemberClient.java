package dev.study.account;

import java.net.http.HttpClient;
import java.time.Duration;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.*;
import org.springframework.http.client.JdkClientHttpRequestFactory;
import org.springframework.stereotype.Component;
import org.springframework.web.client.*;
import org.springframework.web.server.ResponseStatusException;

@Component
public class MemberClient {
    record Member(String id, String email, String name) {}
    private final RestClient client;
    public MemberClient(@Value("${lab.member-url:http://localhost:8083}") String url) {
        var factory = new JdkClientHttpRequestFactory(HttpClient.newBuilder().connectTimeout(Duration.ofSeconds(3)).build());
        factory.setReadTimeout(Duration.ofSeconds(5));
        client = RestClient.builder().baseUrl(url).requestFactory(factory).build();
    }
    public String authenticate(String token) {
        if (token == null || !token.matches("[A-Za-z0-9_-]{43}")) throw new ResponseStatusException(HttpStatus.UNAUTHORIZED,"로그인이 필요합니다.");
        try {
            Member member = client.get().uri("/members/me").header(HttpHeaders.COOKIE,"SAGA_SESSION=" + token).retrieve().body(Member.class);
            if (member == null || member.id() == null) throw new ResponseStatusException(HttpStatus.SERVICE_UNAVAILABLE,"회원 확인에 실패했습니다.");
            return member.id();
        } catch (HttpClientErrorException.Unauthorized e) {
            throw new ResponseStatusException(HttpStatus.UNAUTHORIZED,"세션이 만료되었습니다. 다시 로그인해 주세요.");
        } catch (RestClientException e) {
            throw new ResponseStatusException(HttpStatus.SERVICE_UNAVAILABLE,"회원 서비스 연결을 확인해 주세요.");
        }
    }
}
