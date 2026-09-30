package dev.study.patterns.creational;

/**
 * 학습: 기존 도형의 상태를 복사해 같은 종류의 새 도형을 만든다.
 * 실습: Rectangle을 추가하고 Shape 타입으로 copy()를 호출해 보세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class PrototypeDemo {

    static abstract class Shape {
        private int x;
        private int y;
        private String color;

        Shape(int x, int y, String color) {
            this.x = x;
            this.y = y;
            this.color = color;
        }

        Shape(Shape source) {
            this.x = source.x;
            this.y = source.y;
            this.color = source.color;
        }

        public int getX() {
            return x;
        }

        public int getY() {
            return y;
        }

        public String getColor() {
            return color;
        }

        public void setPosition(int x, int y) {
            this.x = x;
            this.y = y;
        }

        public void setColor(String color) {
            this.color = color;
        }

        public abstract Shape copy();
    }

    static class Circle extends Shape {
        private final int radius;

        Circle(int x, int y, String color, int radius) {
            super(x, y, color);
            this.radius = radius;
        }

        private Circle(Circle source) {
            super(source);
            this.radius = source.radius;
        }

        public int getRadius() {
            return radius;
        }

        @Override
        public Circle copy() {
            return new Circle(this);
        }
    }

    public static void main(String[] args) {
        Shape original = new Circle(10, 20, "red", 5);
        // 구체 타입의 생성자를 몰라도 공통 copy()로 같은 종류의 도형을 복제한다.
        Shape copied = original.copy();
        copied.setPosition(100, 200);
        copied.setColor("blue");

        System.out.println(original.getX() + ", " + original.getY() + ", " + original.getColor());
        System.out.println(copied.getX() + ", " + copied.getY() + ", " + copied.getColor());
        // 기본형은 값 복사, String은 불변 객체를 공유한다.
        // 가변 참조 필드를 추가한다면 해당 객체도 복사할지 결정해야 한다.
    }
}
