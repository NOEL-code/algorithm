package dev.study.patterns.structural;

/**
 * 학습: 기존 API를 클라이언트가 원하는 인터페이스로 변환한다.
 * 실습: 레거시 반환 코드가 실패인 경우를 추가하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class AdapterDemo {

    static class LegacyGateway { String payWon(int won) { return "paid:" + won; } }
    interface Payment { String charge(int amount); }
    static class GatewayAdapter implements Payment {
        private final LegacyGateway gateway;

        GatewayAdapter(LegacyGateway gateway) {
            this.gateway = gateway;
        }

        public LegacyGateway gateway() {
            return gateway;
        }

        public String charge(int amount) { return gateway.payWon(amount); }
    }
    public static void main(String[] args) {
        Payment payment = new GatewayAdapter(new LegacyGateway());
        System.out.println(payment.charge(1000));
    }
}
