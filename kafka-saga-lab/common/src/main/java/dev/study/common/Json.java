/**
 * 학습: 역직렬화 경계의 실패 분류 JSON 파싱 및 필수 필드 검사를 업무 트랜잭션 앞에서 수행한다. Java record라고 입력이 자동 검증되지는 않는다. 잘못된 메시지는
 * IllegalArgumentException으로 분류해 동일한 독성 메시지를 반복 재시도하지 않는다. 형식 검증과 원장 대조는 다르다. 값이 양수여도 다른 주문의 금액이면
 * Handler가 추가 검증해야 한다.
 */
package dev.study.common;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

import org.springframework.stereotype.Component;

@Component
public class Json {
    private final ObjectMapper mapper = new ObjectMapper();

    public String write(Message message) {
        try {
            return mapper.writeValueAsString(message);
        } catch (JsonProcessingException e) {
            throw new IllegalArgumentException("메시지 직렬화 실패", e);
        }
    }

    public Message read(String value) {
        try {
            Message m = mapper.readValue(value, Message.class);
            if (m.eventId() == null
                    || m.orderId() == null
                    || m.type() == null
                    || m.productId() == null
                    || m.quantity() <= 0
                    || m.amount() <= 0) {
                throw new IllegalArgumentException("필수 메시지 필드가 잘못되었습니다");
            }
            return m;
        } catch (JsonProcessingException e) {
            throw new IllegalArgumentException("메시지 역직렬화 실패", e);
        }
    }
}
