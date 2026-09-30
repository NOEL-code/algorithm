package dev.study.patterns.creational;

/**
 * 학습: 필수 값과 선택 값을 나누고 완성 시점에 유효한 객체를 만든다.
 * 실습: 배송 메모 옵션과 수량 검증 실패 시나리오를 추가하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class BuilderDemo {

    static class Order {
        private final String product;
        private final int quantity;
        private final boolean gift;

        Order(String product, int quantity, boolean gift) {
            this.product = product;
            this.quantity = quantity;
            this.gift = gift;
        }

        public String product() {
            return product;
        }

        public int quantity() {
            return quantity;
        }

        public boolean gift() {
            return gift;
        }
    }
    static class OrderBuilder {
        private final String product;
        private int quantity = 1;
        private boolean gift;
        OrderBuilder(String product) { this.product = product; }
        OrderBuilder quantity(int value) { quantity = value; return this; }
        OrderBuilder gift(boolean value) { gift = value; return this; }
        Order build() {
            if (product == null || product.isBlank() || quantity < 1)
                throw new IllegalArgumentException("상품과 양수 수량이 필요합니다");
            return new Order(product, quantity, gift);
        }
    }
    public static void main(String[] args) {
        Order order = new OrderBuilder("책")
                .quantity(3)
                .gift(true)
                .build();
        System.out.println(order.product());
        System.out.println(order.quantity());
        System.out.println(order.gift());
    }
}
