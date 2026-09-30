package dev.study.patterns.structural;

/**
 * 학습: 동일 인터페이스의 대리 객체가 실제 객체 접근과 생성 시점을 제어한다.
 * 실습: 권한 검사 프록시를 만들고 거절 시 실제 호출이 없는지 확인하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class ProxyDemo {

    interface Image { String display(); }
    static class RealImage implements Image {
        final String file;
        RealImage(String file) { this.file = file; }
        public String display() { return "표시:" + file; }
    }
    static class LazyImage implements Image {
        private final String file;
        private RealImage real;
        LazyImage(String file) { this.file = file; }
        public String display() {
            if (real == null) { real = new RealImage(file); }
            return real.display();
        }
    }
    public static void main(String[] args) {
        Image image = new LazyImage("cover.png");
        System.out.println(image.display()); // 첫 접근 시 실제 객체 생성
        System.out.println(image.display()); // 생성된 객체 재사용
    }
}
