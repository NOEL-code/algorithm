/**
 * 학습: 명령 수신자와 결과 수신자의 경계 명령은 payment/inventory/account 토픽, Saga 결과는 replies로 나눠 누가 처리해야 하는지 드러낸다.
 * 서로 다른 consumer group은 같은 레코드를 독립적으로 읽는다. 그룹을 합치면 서비스 간 메시지가 분산될 수 있다. 토픽 이름을 공통 계약으로 묶되 DB 테이블이나
 * 업무 로직까지 공통 모듈에 공유하지 않는다.
 */
package dev.study.common;

public final class Topics {
    private Topics() {}

    public static final String PAYMENT = "payment.commands";
    public static final String INVENTORY = "inventory.commands";
    public static final String REPLIES = "saga.replies";
    public static final String ACCOUNT = "account.commands";
}
