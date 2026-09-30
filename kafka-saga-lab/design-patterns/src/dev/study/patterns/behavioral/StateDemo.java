package dev.study.patterns.behavioral;

/**
 * 학습: 현재 상태 객체가 동작과 다음 상태를 결정한다.
 * 실습: 취소 상태를 추가하고 배송 후 취소를 거절하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class StateDemo {

    interface State { void pay(Order order); void ship(Order order); }
    static class Pending implements State {
        public void pay(Order order) { order.state = new Paid(); }
        public void ship(Order order) { throw new IllegalStateException("결제 필요"); }
    }
    static class Paid implements State {
        public void pay(Order order) { throw new IllegalStateException("이미 결제됨"); }
        public void ship(Order order) { order.state = new Shipped(); }
    }
    static class Shipped implements State {
        public void pay(Order order) { throw new IllegalStateException("배송 완료"); }
        public void ship(Order order) { throw new IllegalStateException("배송 완료"); }
    }
    static class Order {
        private State state = new Pending();
        void pay() { state.pay(this); }
        void ship() { state.ship(this); }
    }
    public static void main(String[] args) {
        Order order = new Order();
        order.pay(); // Pending → Paid
        order.ship(); // Paid → Shipped
    }
}
