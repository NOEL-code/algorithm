package dev.study.patterns.behavioral;

import java.util.ArrayList;
import java.util.List;

/**
 * 학습: 발행자가 구독자의 구체 타입을 몰라도 변경을 통지한다.
 * 실습: 통지 중 구독 해제와 구독자 예외를 어떤 정책으로 처리할지 정하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class ObserverDemo {

    interface Listener { void onEvent(String event); }
    static class EmailListener implements Listener {
        @Override
        public void onEvent(String event) {
            System.out.println("이메일: " + event);
        }
    }
    static class StockListener implements Listener {
        @Override
        public void onEvent(String event) {
            System.out.println("재고: " + event);
        }
    }
    static class OrderEvents {
        private final List<Listener> listeners = new ArrayList<>();
        void subscribe(Listener listener) { listeners.add(listener); }
        void unsubscribe(Listener listener) { listeners.remove(listener); }
        void publish(String event) {
            for (Listener listener : List.copyOf(listeners)) listener.onEvent(event);
        }
    }
    public static void main(String[] args) {
        OrderEvents events = new OrderEvents();
        Listener email = new EmailListener();
        Listener stock = new StockListener();
        events.subscribe(email);
        events.subscribe(stock);
        events.publish("주문 생성");
        events.unsubscribe(email);
        events.publish("주문 취소");
        // 동기 메모리 통지다. Kafka의 영속성이나 재시도 보장은 없다.
    }
}
