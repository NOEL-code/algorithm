package dev.study.patterns.creational;

/**
 * 학습: 설정 관리자를 private 생성자와 정적 인스턴스로 하나만 생성한다.
 * 실습: getInstance()를 여러 곳에서 호출해도 같은 설정 객체를 사용하는 흐름을 따라가세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class SingletonDemo {

    static final class ConfigurationManager {
        private static final ConfigurationManager INSTANCE = new ConfigurationManager();
        private final String applicationName = "Design Pattern Demo";

        private ConfigurationManager() {
        }

        public static ConfigurationManager getInstance() {
            return INSTANCE;
        }

        public String getApplicationName() {
            return applicationName;
        }
    }

    public static void main(String[] args) {
        ConfigurationManager configuration = ConfigurationManager.getInstance();
        System.out.println(configuration.getApplicationName());
    }
}
