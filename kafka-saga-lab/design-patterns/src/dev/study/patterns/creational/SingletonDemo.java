package dev.study.patterns.creational;

/**
 * 학습: private 생성자로 외부 생성을 막고 정적 인스턴스 하나를 공유한다.
 * 실습: 가변 설정을 추가하면 병렬 실행에서 어떤 문제가 생기는지 설명하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class SingletonDemo {

    static final class Settings {
        private static final Settings INSTANCE = new Settings();
        private final String currency = "KRW";

        private Settings() {
        }

        public static Settings getInstance() {
            return INSTANCE;
        }

        public String getCurrency() {
            return currency;
        }
    }
    public static void main(String[] args) {
        Settings settings = Settings.getInstance();
        System.out.println(settings.getCurrency());
    }
}
