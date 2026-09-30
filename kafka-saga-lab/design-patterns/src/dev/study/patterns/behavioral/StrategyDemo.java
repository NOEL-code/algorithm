package dev.study.patterns.behavioral;

/**
 * 학습: 알고리즘을 공통 인터페이스 뒤에 두고 호출자가 교체한다.
 * 실습: 고정 금액 할인 전략을 추가하고 음수 결제 금액을 방지하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class StrategyDemo {

    interface Discount { int apply(int price); }
    static class Regular implements Discount { public int apply(int price) { return price; } }
    static class Vip implements Discount { public int apply(int price) { return price - price / 10; } }
    static class Checkout {
        private final Discount discount;

        Checkout(Discount discount) {
            this.discount = discount;
        }

        public Discount discount() {
            return discount;
        }

        int total(int price) { return discount.apply(price); }
    }
    public static void main(String[] args) {
        Checkout regular = new Checkout(new Regular());
        Checkout vip = new Checkout(new Vip());
        System.out.println(regular.total(10000));
        System.out.println(vip.total(10000));
    }
}
