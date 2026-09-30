package dev.study.patterns.behavioral;

/**
 * 학습: 상위 클래스가 처리 순서를 고정하고 일부 단계를 하위 클래스에 맡긴다.
 * 실습: 검증 단계를 추가하고 하위 클래스가 순서를 바꿀 수 없는지 확인하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class TemplateMethodDemo {

    static abstract class Report {
        final String generate() { return "HEADER\n" + body() + "\nFOOTER"; }
        abstract String body();
    }
    static class SalesReport extends Report { String body() { return "매출:1000"; } }
    static class StockReport extends Report { String body() { return "재고:3"; } }
    public static void main(String[] args) {
        Report sales = new SalesReport();
        Report stock = new StockReport();
        System.out.println(sales.generate());
        System.out.println(stock.generate());
    }
}
