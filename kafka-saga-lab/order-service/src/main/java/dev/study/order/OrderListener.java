package dev.study.order;

import dev.study.common.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.kafka.annotation.KafkaListener;
import org.springframework.stereotype.Component;

@Component
public class OrderListener {
    private static final Logger log = LoggerFactory.getLogger(OrderListener.class);
    private final Json json;
    private final OrderSaga handler;
    public OrderListener(Json json, OrderSaga handler) { this.json = json; this.handler = handler; }

    @KafkaListener(topics = Topics.REPLIES)
    public void listen(String value) {
        // 별도 Bean 호출이므로 @Transactional 프록시가 적용된다.
        // 예외는 잡아 삼키지 않는다. DB 롤백 후 Kafka 재시도/DLT 정책이 처리한다.
        var message = json.read(value);
        log.info("수신 orderId={} eventId={} type={}", message.orderId(), message.eventId(), message.type());
        handler.onReply(message);
    }
}
