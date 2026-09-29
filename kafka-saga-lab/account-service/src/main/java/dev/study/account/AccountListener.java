/**
 * 학습: 전송 계층과 DB 트랜잭션 경계 Listener는 JSON을 해석하고 별도 Handler Bean을 호출한다. 프록시를 통과해야 @Transactional이 적용된다.
 * Handler 커밋 후 정상 반환해야 offset을 진행한다. 예외를 잡아 로그만 남기면 업무 실패가 소비 성공으로 처리될 수 있다.
 */
package dev.study.account;

import dev.study.common.*;

import org.springframework.kafka.annotation.KafkaListener;
import org.springframework.stereotype.Component;

@Component
public class AccountListener {
    private final Json json;
    private final AccountOrderHandler handler;

    public AccountListener(Json json, AccountOrderHandler handler) {
        this.json = json;
        this.handler = handler;
    }

    @KafkaListener(topics = Topics.ACCOUNT)
    public void listen(String value) {
        handler.handle(json.read(value));
    }
}
