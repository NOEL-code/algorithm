package dev.study.patterns.behavioral;

import java.util.List;

/**
 * 학습: 요소 구조와 작업을 분리하고 이중 디스패치로 타입별 작업을 선택한다.
 * 실습: 요소 수정 없이 이름을 수집하는 새 Visitor를 추가하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class VisitorDemo {

    interface Element { void accept(Visitor visitor); }
    static class Book implements Element {
        private final int price;

        Book(int price) {
            this.price = price;
        }

        public int price() {
            return price;
        }

        public void accept(Visitor visitor) { visitor.visit(this); }
    }
    static class Fruit implements Element {
        private final int price;
        private final int weight;

        Fruit(int price, int weight) {
            this.price = price;
            this.weight = weight;
        }

        public int price() {
            return price;
        }

        public int weight() {
            return weight;
        }

        public void accept(Visitor visitor) { visitor.visit(this); }
    }
    interface Visitor { void visit(Book book); void visit(Fruit fruit); }
    static class PriceVisitor implements Visitor {
        int total;
        public void visit(Book book) { total += book.price(); }
        public void visit(Fruit fruit) { total += fruit.price() * fruit.weight(); }
    }
    static class CountVisitor implements Visitor {
        int count;
        public void visit(Book book) { count++; }
        public void visit(Fruit fruit) { count++; }
    }
    public static void main(String[] args) {
        List<Element> cart = List.of(new Book(1000), new Fruit(500, 3));
        PriceVisitor price = new PriceVisitor();
        CountVisitor count = new CountVisitor();
        for (Element item : cart) {
            item.accept(price);
            item.accept(count);
        }
        System.out.println(price.total);
        System.out.println(count.count);
    }
}
