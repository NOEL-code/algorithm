package dev.study.patterns.structural;

/**
 * 학습: 추상 기능과 구현 수단을 분리해 두 축을 독립적으로 확장한다.
 * 실습: 긴급 알림과 메신저 채널을 각각 추가하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class BridgeDemo {

    interface Channel { String deliver(String text); }
    static class Email implements Channel { public String deliver(String text) { return "email:" + text; } }
    static class Sms implements Channel { public String deliver(String text) { return "sms:" + text; } }
    static abstract class Notice {
        protected final Channel channel;
        Notice(Channel channel) { this.channel = channel; }
        abstract String send();
    }
    static class OrderNotice extends Notice {
        OrderNotice(Channel channel) { super(channel); }
        String send() { return channel.deliver("주문 완료"); }
    }
    static class RefundNotice extends Notice {
        RefundNotice(Channel channel) { super(channel); }
        String send() { return channel.deliver("환불 완료"); }
    }
    public static void main(String[] args) {
        Notice orderNotice = new OrderNotice(new Email());
        Notice refundNotice = new RefundNotice(new Sms());
        System.out.println(orderNotice.send());
        System.out.println(refundNotice.send());
    }
}
