/**
 * 학습: 전송 계층과 DB 트랜잭션 경계 Listener는 JSON을 해석하고 별도 Handler Bean을 호출한다. 프록시를 통과해야 @Transactional이 적용된다.
 * Handler 커밋 후 정상 반환해야 offset을 진행한다. 예외를 잡아 로그만 남기면 업무 실패가 소비 성공으로 처리될 수 있다.
 */
package dev.study.inventory;

import dev.study.common.*;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.kafka.annotation.KafkaListener;
import org.springframework.stereotype.Component;

@Component
public class InventoryListener {
    private static final Logger log = LoggerFactory.getLogger(InventoryListener.class);
    private final Json json;
    private final InventoryHandler handler;

    public InventoryListener(Json json, InventoryHandler handler) {
        this.json = json;
        this.handler = handler;
    }

    @KafkaListener(topics = Topics.INVENTORY)
    public void listen(String value) {
        // 별도 Bean 호출이므로 @Transactional 프록시가 적용된다.
        // 예외는 잡아 삼키지 않는다. DB 롤백 후 Kafka 재시도/DLT 정책이 처리한다.
        var message = json.read(value);
        log.info(
                "수신 orderId={} eventId={} type={}",
                message.orderId(),
                message.eventId(),
                message.type());
        handler.handle(message);
    }
}
