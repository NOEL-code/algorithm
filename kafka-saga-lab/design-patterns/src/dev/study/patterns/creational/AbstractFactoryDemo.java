package dev.study.patterns.creational;

/**
 * 학습: Windows 또는 Mac 팩토리 하나로 같은 운영체제의 버튼과 체크박스를 생성한다.
 * 실습: LinuxFactory를 추가한 뒤 Application 수정 없이 UI 제품군을 교체하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class AbstractFactoryDemo {

    interface Button {
        void render();
    }

    interface Checkbox {
        void render();
    }

    interface GUIFactory {
        Button createButton();
        Checkbox createCheckbox();
    }

    static class WindowsButton implements Button {
        @Override
        public void render() {
            System.out.println("Windows 버튼");
        }
    }

    static class WindowsCheckbox implements Checkbox {
        @Override
        public void render() {
            System.out.println("Windows 체크박스");
        }
    }

    static class MacButton implements Button {
        @Override
        public void render() {
            System.out.println("Mac 버튼");
        }
    }

    static class MacCheckbox implements Checkbox {
        @Override
        public void render() {
            System.out.println("Mac 체크박스");
        }
    }

    static class WindowsFactory implements GUIFactory {
        @Override
        public Button createButton() {
            return new WindowsButton();
        }

        @Override
        public Checkbox createCheckbox() {
            return new WindowsCheckbox();
        }
    }

    static class MacFactory implements GUIFactory {
        @Override
        public Button createButton() {
            return new MacButton();
        }

        @Override
        public Checkbox createCheckbox() {
            return new MacCheckbox();
        }
    }

    static class Application {
        private final Button button;
        private final Checkbox checkbox;

        Application(GUIFactory factory) {
            button = factory.createButton();
            checkbox = factory.createCheckbox();
        }

        public void render() {
            button.render();
            checkbox.render();
        }
    }

    public static void main(String[] args) {
        Application windowsApp = new Application(new WindowsFactory());
        windowsApp.render();

        Application macApp = new Application(new MacFactory());
        macApp.render();
    }
}
