/**
 * 학습: 독립 배포 단위와 Bean 탐색 범위 Spring 컨테이너와 해당 서비스의 DB/HTTP 경계를 시작한다. 다른 서비스의 업무 Bean을 스캔하지 않는다.
 * EnableScheduling이 있는 서비스만 Outbox relay를 스케줄링한다. 회원 서비스는 Kafka 업무 명령을 발행하지 않는다.
 */
package dev.study.account;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@org.springframework.scheduling.annotation.EnableScheduling
@SpringBootApplication(scanBasePackages = {"dev.study.account", "dev.study.common"})
public class AccountApplication {
    public static void main(String[] args) {
        SpringApplication.run(AccountApplication.class, args);
    }
}
