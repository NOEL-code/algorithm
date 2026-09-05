package dev.study.common;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.stereotype.Component;

@Component
public class Json {
    private final ObjectMapper mapper = new ObjectMapper();
    public String write(Message message) {
        try { return mapper.writeValueAsString(message); }
        catch (JsonProcessingException e) { throw new IllegalArgumentException("메시지 직렬화 실패", e); }
    }
    public Message read(String value) {
        try {
            Message m = mapper.readValue(value, Message.class);
            if (m.eventId() == null || m.orderId() == null || m.type() == null ||
                m.productId() == null || m.quantity() <= 0 || m.amount() <= 0) {
                throw new IllegalArgumentException("필수 메시지 필드가 잘못되었습니다");
            }
            return m;
        } catch (JsonProcessingException e) { throw new IllegalArgumentException("메시지 역직렬화 실패", e); }
    }
}
