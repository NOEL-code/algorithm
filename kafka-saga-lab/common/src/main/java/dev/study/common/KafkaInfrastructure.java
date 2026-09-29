/**
 * 학습: 전달 설정과 복구 정책을 업무 실패와 구분하기 3개 파티션은 병렬 처리 단위, 복제 계수 1은 로컬 실습 제약이다. acks=all이 복제 수를 늘려주지는 않는다.
 * 기술 오류는 제한적으로 재시도하고 DLT에 격리한다. DLT 발행 실패 시 원본을 성공 처리하지 않도록 설정한다. DLT는 환불/취소 완료가 아니다. 현재 업무 상태
 * 확인·재처리·대사는 별도 운영 절차다.
 */
package dev.study.common;

import org.apache.kafka.common.TopicPartition;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.kafka.config.TopicBuilder;
import org.springframework.kafka.core.KafkaAdmin;
import org.springframework.kafka.core.KafkaTemplate;
import org.springframework.kafka.listener.DeadLetterPublishingRecoverer;
import org.springframework.kafka.listener.DefaultErrorHandler;
import org.springframework.util.backoff.FixedBackOff;

@Configuration
public class KafkaInfrastructure {
    @Bean
    KafkaAdmin.NewTopics topics() {
        return new KafkaAdmin.NewTopics(
                TopicBuilder.name(Topics.ACCOUNT).partitions(3).replicas(1).build(),
                TopicBuilder.name(Topics.ACCOUNT + ".DLT").partitions(3).replicas(1).build(),
                TopicBuilder.name(Topics.PAYMENT).partitions(3).replicas(1).build(),
                TopicBuilder.name(Topics.INVENTORY).partitions(3).replicas(1).build(),
                TopicBuilder.name(Topics.REPLIES).partitions(3).replicas(1).build(),
                TopicBuilder.name(Topics.PAYMENT + ".DLT").partitions(3).replicas(1).build(),
                TopicBuilder.name(Topics.INVENTORY + ".DLT").partitions(3).replicas(1).build(),
                TopicBuilder.name(Topics.REPLIES + ".DLT").partitions(3).replicas(1).build());
    }

    @Bean
    DefaultErrorHandler errorHandler(KafkaTemplate<String, String> kafka) {
        var recoverer =
                new DeadLetterPublishingRecoverer(
                        kafka,
                        (record, ex) ->
                                new TopicPartition(record.topic() + ".DLT", record.partition()));
        // DLT 발행 실패를 성공으로 간주하여 원본 offset을 넘기지 않는다.
        recoverer.setFailIfSendResultIsError(true);
        var handler = new DefaultErrorHandler(recoverer, new FixedBackOff(1000, 2));
        handler.addNotRetryableExceptions(IllegalArgumentException.class);
        return handler;
    }
}
