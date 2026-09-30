package dev.study.patterns.structural;

import java.util.List;

/**
 * 학습: 여러 하위 시스템을 사용하는 순서를 간단한 진입점으로 제공한다.
 * 실습: 결제 실패 시 재고 예약 취소가 필요한 이유를 설명하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class FacadeDemo {

    static class Stock { String reserve() { return "재고 예약"; } }
    static class Billing { String pay() { return "결제"; } }
    static class Shipping { String ship() { return "배송 접수"; } }
    static class Checkout {
        private final Stock stock;
        private final Billing billing;
        private final Shipping shipping;

        Checkout(Stock stock, Billing billing, Shipping shipping) {
            this.stock = stock;
            this.billing = billing;
            this.shipping = shipping;
        }

        public Stock stock() {
            return stock;
        }

        public Billing billing() {
            return billing;
        }

        public Shipping shipping() {
            return shipping;
        }

        List<String> order() { return List.of(stock.reserve(), billing.pay(), shipping.ship()); }
    }
    public static void main(String[] args) {
        Checkout checkout = new Checkout(new Stock(), new Billing(), new Shipping());
        System.out.println(checkout.order());
        // Facade 자체는 트랜잭션이나 실패 보상을 보장하지 않는다.
    }
}
