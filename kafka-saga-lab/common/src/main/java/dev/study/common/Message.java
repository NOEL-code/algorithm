package dev.study.common;

import java.util.UUID;

// 학습용 단일 메시지 계약. 실무에서는 명령/이벤트별 스키마와 버전을 따로 관리한다.
public record Message(String eventId, String orderId, Type type, String productId,
                      int quantity, long amount, boolean rejectPayment) {
    public enum Type { CHARGE_PAYMENT, PAYMENT_CHARGED, PAYMENT_REJECTED,
        RESERVE_STOCK, STOCK_RESERVED, STOCK_REJECTED, REFUND_PAYMENT, PAYMENT_REFUNDED }

    public static Message start(String orderId, String productId, int quantity, long amount, boolean reject) {
        return new Message(UUID.randomUUID().toString(), orderId, Type.CHARGE_PAYMENT,
                productId, quantity, amount, reject);
    }

    public Message next(Type type) {
        return new Message(UUID.randomUUID().toString(), orderId, type, productId, quantity, amount, rejectPayment);
    }
}
