package dev.study.patterns.structural;

import java.util.List;

/**
 * 학습: 단일 항목과 항목 묶음을 같은 인터페이스로 다룬다.
 * 실습: 여러 단계로 중첩된 묶음의 합계를 검증하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class CompositeDemo {

    interface Item { int price(); }
    static class Product implements Item {
        private final int price;

        Product(int price) {
            this.price = price;
        }

        public int price() {
            return price;
        }
    }
    static class Bundle implements Item {
        private final List<Item> children;

        Bundle(List<Item> children) {
            this.children = List.copyOf(children);
        }

        public List<Item> children() {
            return children;
        }

        public int price() {
            int total = 0;
            for (Item child : children) {
                total += child.price();
            }
            return total;
        }
    }
    public static void main(String[] args) {
        Item cart = new Bundle(List.of(
                new Product(1000),
                new Bundle(List.of(new Product(2000), new Product(3000)))
        ));
        System.out.println(cart.price());
    }
}
