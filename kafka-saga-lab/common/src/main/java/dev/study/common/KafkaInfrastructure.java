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
            TopicBuilder.name(Topics.PAYMENT).partitions(3).replicas(1).build(),
            TopicBuilder.name(Topics.INVENTORY).partitions(3).replicas(1).build(),
            TopicBuilder.name(Topics.REPLIES).partitions(3).replicas(1).build(),
            TopicBuilder.name(Topics.PAYMENT + ".DLT").partitions(3).replicas(1).build(),
            TopicBuilder.name(Topics.INVENTORY + ".DLT").partitions(3).replicas(1).build(),
            TopicBuilder.name(Topics.REPLIES + ".DLT").partitions(3).replicas(1).build());
    }

    @Bean
    DefaultErrorHandler errorHandler(KafkaTemplate<String, String> kafka) {
        var recoverer = new DeadLetterPublishingRecoverer(kafka,
                (record, ex) -> new TopicPartition(record.topic() + ".DLT", record.partition()));
        // DLT 발행 실패를 성공으로 간주하여 원본 offset을 넘기지 않는다.
        recoverer.setFailIfSendResultIsError(true);
        var handler = new DefaultErrorHandler(recoverer, new FixedBackOff(1000, 2));
        handler.addNotRetryableExceptions(IllegalArgumentException.class);
        return handler;
    }
}
