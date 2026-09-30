package dev.study.patterns.creational;

/**
 * 학습: 상위 클래스의 처리 흐름은 유지하고 하위 클래스가 생성할 제품을 결정한다.
 * 실습: SmsNotification을 추가하고 상위 클래스 변경 없이 실행하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class FactoryMethodDemo {

    interface Sender { String send(String message); }
    static class EmailSender implements Sender {
        @Override
        public String send(String message) {
            return "email:" + message;
        }
    }
    static class PushSender implements Sender {
        @Override
        public String send(String message) {
            return "push:" + message;
        }
    }
    static abstract class Notification {
        abstract Sender createSender(); // Factory Method
        final String notifyUser(String message) { return createSender().send(message); }
    }
    static class EmailNotification extends Notification {
        Sender createSender() { return new EmailSender(); }
    }
    static class PushNotification extends Notification {
        Sender createSender() { return new PushSender(); }
    }
    public static void main(String[] args) {
        Notification notification = new EmailNotification();
        notification.notifyUser("주문 완료");
        notification = new PushNotification();
        notification.notifyUser("배송 시작");
    }
}
