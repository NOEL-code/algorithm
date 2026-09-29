/**
 * 학습: 메시지 식별자와 업무 식별자의 분리 eventId는 특정 메시지, orderId는 Saga 상관관계와 Kafka key다. next는 전자는 바꾸고 후자는 유지한다.
 * memberId/accountId는 인증된 주문에서 파생한다. 이 필드가 존재한다고 Kafka 생산자가 인증되는 것은 아니다. 단일 record는 학습 편의다. 운영에서는
 * 버전별 명령/이벤트 계약과 호환성 정책을 나눌 수 있다.
 */
package dev.study.common;

import java.util.UUID;

// 학습용 단일 메시지 계약. 실무에서는 명령/이벤트별 스키마와 버전을 따로 관리한다.
public record Message(
        String eventId,
        String orderId,
        Type type,
        String productId,
        int quantity,
        long amount,
        boolean rejectPayment,
        String memberId,
        String accountId) {
    public enum Type {
        CHARGE_PAYMENT,
        PAYMENT_CHARGED,
        PAYMENT_REJECTED,
        RESERVE_STOCK,
        STOCK_RESERVED,
        STOCK_REJECTED,
        REFUND_PAYMENT,
        PAYMENT_REFUNDED,
        DEBIT_ACCOUNT,
        ACCOUNT_DEBITED,
        ACCOUNT_REJECTED,
        CREDIT_ACCOUNT,
        ACCOUNT_CREDITED,
        SETTLE_ACCOUNT
    }

    // Existing lab messages remain readable; new HTTP orders always require an authenticated
    // account.
    public Message(
            String eventId,
            String orderId,
            Type type,
            String productId,
            int quantity,
            long amount,
            boolean rejectPayment) {
        this(eventId, orderId, type, productId, quantity, amount, rejectPayment, null, null);
    }

    public static Message start(
            String orderId, String productId, int quantity, long amount, boolean reject) {
        return new Message(
                UUID.randomUUID().toString(),
                orderId,
                Type.CHARGE_PAYMENT,
                productId,
                quantity,
                amount,
                reject);
    }

    public Message next(Type type) {
        return new Message(
                UUID.randomUUID().toString(),
                orderId,
                type,
                productId,
                quantity,
                amount,
                rejectPayment,
                memberId,
                accountId);
    }
}
