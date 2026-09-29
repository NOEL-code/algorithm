/**
 * 학습: 서비스 간 HTTP 계약과 TOCTOU 계좌 DB를 직접 조회하지 않고 소유자가 인증된 API를 호출해 서비스 경계를 지킨다. 연결/응답 시간 제한을 둔다. 상대
 * 서비스 장애를 무기한 대기로 전파하지 않지만 이중 검증 비용은 생긴다. 조회 직후 상태가 바뀔 수 있다. 확인 결과를 분산 락이나 결제 성공 보장으로 해석하지 않는다.
 */
package dev.study.order;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.*;
import org.springframework.http.client.JdkClientHttpRequestFactory;
import org.springframework.stereotype.Component;
import org.springframework.web.client.*;
import org.springframework.web.server.ResponseStatusException;

import java.net.http.HttpClient;
import java.time.Duration;
import java.util.Map;

@Component
public class OrderAccounts {
    private final RestClient client;

    public OrderAccounts(@Value("${lab.account-url:http://localhost:8084}") String url) {
        var factory =
                new JdkClientHttpRequestFactory(
                        HttpClient.newBuilder().connectTimeout(Duration.ofSeconds(3)).build());
        factory.setReadTimeout(Duration.ofSeconds(5));
        client = RestClient.builder().baseUrl(url).requestFactory(factory).build();
    }

    public void requireOwnedOpen(String token, String accountId) {
        try {
            var account =
                    client.get()
                            .uri("/accounts/{id}", accountId)
                            .header(HttpHeaders.COOKIE, "SAGA_SESSION=" + token)
                            .retrieve()
                            .body(Map.class);
            if (account == null || !"OPEN".equals(account.get("status")))
                throw new ResponseStatusException(HttpStatus.CONFLICT, "사용 가능한 계좌가 아닙니다.");
        } catch (HttpClientErrorException e) {
            throw new ResponseStatusException(e.getStatusCode(), "본인의 사용 가능한 계좌를 선택해 주세요.");
        } catch (RestClientException e) {
            throw new ResponseStatusException(HttpStatus.SERVICE_UNAVAILABLE, "계좌 서비스 연결 실패");
        }
    }
}
