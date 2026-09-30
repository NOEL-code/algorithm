package dev.study.patterns.behavioral;

/**
 * 학습: 요청을 처리기 사슬로 전달하고 조건에 따라 중단한다.
 * 실습: 금액 상한 처리기를 추가하고 뒤 처리기가 호출되지 않는지 확인하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class ChainOfResponsibilityDemo {

    static class Request {
        private final boolean authenticated;
        private final int amount;

        Request(boolean authenticated, int amount) {
            this.authenticated = authenticated;
            this.amount = amount;
        }

        public boolean authenticated() {
            return authenticated;
        }

        public int amount() {
            return amount;
        }
}
    static abstract class Handler {
        private final Handler next;
        Handler(Handler next) { this.next = next; }
        final String handle(Request request) {
            String error = validate(request);
            return error != null ? error : next == null ? "승인" : next.handle(request);
        }
        abstract String validate(Request request);
    }
    static class Authentication extends Handler {
        Authentication(Handler next) { super(next); }
        String validate(Request r) { return r.authenticated() ? null : "인증 필요"; }
    }
    static class PositiveAmount extends Handler {
        PositiveAmount(Handler next) { super(next); }
        String validate(Request r) { return r.amount() > 0 ? null : "금액 오류"; }
    }
    public static void main(String[] args) {
        Handler chain = new Authentication(new PositiveAmount(null));
        System.out.println(chain.handle(new Request(true, 100)));
    }
}
