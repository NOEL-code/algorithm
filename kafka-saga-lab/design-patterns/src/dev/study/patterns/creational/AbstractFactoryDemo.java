package dev.study.patterns.creational;

/**
 * 학습: 서로 어울리는 여러 제품을 한 팩토리에서 생성한다.
 * 실습: Checkbox 제품을 추가할 때 수정해야 할 클래스를 찾아보세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class AbstractFactoryDemo {

    interface Button { String render(); }
    interface Menu { String render(); }
    interface ThemeFactory { Button button(); Menu menu(); }
    static class LightButton implements Button {
        @Override
        public String render() {
            return "light button";
        }
    }
    static class LightMenu implements Menu {
        @Override
        public String render() {
            return "light menu";
        }
    }
    static class DarkButton implements Button {
        @Override
        public String render() {
            return "dark button";
        }
    }
    static class DarkMenu implements Menu {
        @Override
        public String render() {
            return "dark menu";
        }
    }
    static class LightTheme implements ThemeFactory {
        public Button button() { return new LightButton(); }
        public Menu menu() { return new LightMenu(); }
    }
    static class DarkTheme implements ThemeFactory {
        public Button button() { return new DarkButton(); }
        public Menu menu() { return new DarkMenu(); }
    }
    static String screen(ThemeFactory factory) {
        return factory.button().render() + ", " + factory.menu().render();
    }
    public static void main(String[] args) {
        ThemeFactory theme = new LightTheme();
        System.out.println(screen(theme));
        theme = new DarkTheme();
        System.out.println(screen(theme));
    }
}
