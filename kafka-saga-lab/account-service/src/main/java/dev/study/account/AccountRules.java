package dev.study.account;

/**
 * 학습: 업무 불변식은 HTTP DTO 밖에서도 지켜야 한다. Kafka, 배치, 다른 Bean이 서비스를 직접 호출하면 @Valid는 실행되지 않는다. 계좌의 금액 범위를 한
 * 곳에서 정의하되, DB CHECK도 마지막 방어선으로 유지한다. 이 작은 정책 클래스는 SQL/트랜잭션을 숨기는 범용 Repository 추상화가 아니다.
 */
public final class AccountRules {
    public static final long MAX_TRANSFER = 1_000_000_000L;
    public static final long MAX_BALANCE = 9_000_000_000_000L;

    private AccountRules() {}

    public enum Movement {
        DEPOSIT,
        WITHDRAW;

        long delta(long amount) {
            return this == DEPOSIT ? amount : -amount;
        }
    }

    public static Movement manualMovement(String type, long amount, String requestId) {
        // "DEPOSIT 이외에는 출금"으로 해석하면 오타/새 메시지 유형도 돈을 빼는 명령이 된다.
        if (amount < 1
                || amount > MAX_TRANSFER
                || requestId == null
                || !requestId.matches(
                        "[a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}")) {
            throw invalid();
        }
        try {
            return Movement.valueOf(type);
        } catch (IllegalArgumentException | NullPointerException e) {
            throw invalid();
        }
    }

    // 업무 규칙은 HTTP 상태를 알지 않는다. HTTP로 호출했을 때의 400 변환은 ApiErrors가 맡는다.
    public static final class InvalidMovement extends IllegalArgumentException {
        private InvalidMovement() {
            super("거래 유형·금액·요청 ID를 확인해 주세요.");
        }
    }

    private static InvalidMovement invalid() {
        return new InvalidMovement();
    }
}
