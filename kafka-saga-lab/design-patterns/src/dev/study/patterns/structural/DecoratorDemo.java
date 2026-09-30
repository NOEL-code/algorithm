package dev.study.patterns.structural;

/**
 * 학습: 같은 인터페이스의 객체를 감싸 기능을 누적한다.
 * 실습: 우유 데코레이터를 두 번 감쌌을 때 결과를 예측하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class DecoratorDemo {

    interface Beverage { int price(); String description(); }
    static class Coffee implements Beverage {
        public int price() { return 3000; }
        public String description() { return "커피"; }
    }
    static class Milk implements Beverage {
        private final Beverage base;

        Milk(Beverage base) {
            this.base = base;
        }

        public Beverage base() {
            return base;
        }

        public int price() { return base.price() + 500; }
        public String description() { return base.description() + "+우유"; }
    }
    static class Shot implements Beverage {
        private final Beverage base;

        Shot(Beverage base) {
            this.base = base;
        }

        public Beverage base() {
            return base;
        }

        public int price() { return base.price() + 700; }
        public String description() { return base.description() + "+샷"; }
    }
    public static void main(String[] args) {
        Beverage drink = new Shot(new Milk(new Coffee()));
        System.out.println(drink.description());
        System.out.println(drink.price());
    }
}
