# 코드와 연결하는 Kafka + MSA 면접 질문

답을 외우기보다 “어느 파일의 어떤 장애 구간을 해결했는가”를 설명해 보세요.

## 1. 왜 MSA에서 하나의 @Transactional로 주문·결제·재고를 묶지 않았나요?

이 예제는 서비스별로 DataSource가 다릅니다. 주문 서비스의 DB 트랜잭션은 결제·재고 DB에 전파되지 않습니다. `OrderSaga.create()`는 주문과 주문 outbox만 커밋하고, 다른 서비스는 메시지를 받아 자신의 로컬 트랜잭션을 수행합니다.

꼬리 질문: “서비스를 나눴는데 같은 DB 테이블을 함께 수정하면요?” → 배포와 스키마 변경, 장애가 다시 결합됩니다. 데이터 소유권을 정하고 다른 서비스 데이터는 API나 이벤트로 접근합니다.

## 2. Saga란 무엇인가요? rollback과 어떻게 다른가요?

여러 로컬 트랜잭션을 업무 순서로 연결하는 방식입니다. 앞 단계가 이미 커밋된 뒤 후속 단계가 실패하면 새 트랜잭션으로 업무상 반대 동작을 실행합니다. 여기서는 재고 부족 시 `REFUND_PAYMENT`를 보내 결제 상태를 `REFUNDED`로 바꿉니다. 기존 결제 기록을 지우는 DB rollback이 아닙니다. [Saga 패턴](https://microservices.io/patterns/data/saga.html)

꼬리 질문: “이메일 발송도 되돌릴 수 있나요?” → 항상 원상복구할 수는 없습니다. 취소 안내처럼 업무적으로 수용 가능한 보상을 설계해야 합니다.

## 3. Orchestration과 Choreography 중 무엇을 선택했나요?

이 예제는 `OrderSaga`가 명령과 상태 전이를 관리하는 Orchestration입니다. 흐름과 보상 경로를 한 곳에서 읽기 쉽습니다. 대신 조정자가 업무 흐름을 알아야 합니다. Choreography는 서비스들이 이벤트에 반응하므로 중앙 흐름 관리가 줄어들지만 단계가 많아지면 전체 경로 추적이 어려워질 수 있습니다. [두 방식 비교](https://microservices.io/patterns/data/saga.html)

## 4. 재고 부족이면 왜 즉시 CANCELLED로 바꾸지 않나요?

결제가 아직 청구된 상태이기 때문입니다. `STOCK_REJECTED` 수신 시 `COMPENSATING`으로 이동하고 환불 명령을 저장합니다. `PAYMENT_REFUNDED`를 받은 뒤에만 `CANCELLED`로 전이합니다. 환불 장애가 있으면 보상이 미완료라는 사실을 상태로 드러냅니다.

## 5. DB 저장 후 kafkaTemplate.send() 하면 충분하지 않나요?

DB 커밋 직후 프로세스가 죽으면 Kafka에 보내지 못할 수 있습니다. 먼저 Kafka에 보내면 DB rollback에도 메시지만 남을 수 있습니다. 여기서는 업무 데이터와 메시지를 DB에 함께 커밋하고 `OutboxPublisher`가 나중에 전달합니다. [Transactional Outbox](https://microservices.io/patterns/data/transactional-outbox.html)

꼬리 질문: “AFTER_COMMIT 이벤트 리스너면요?” → 커밋 뒤 프로세스 종료 시 실행 보장이 별도로 필요합니다. 메모리 콜백만으로 발행 의도가 영속화되지는 않습니다.

## 6. Outbox를 쓰면 메시지가 한 번만 전달되나요?

아닙니다. Kafka ACK 이후 outbox 삭제 커밋 전 종료되면 같은 메시지를 다시 발행합니다. 이 예제는 기존 payload를 그대로 재발행하여 eventId를 유지하고, 소비자가 inbox로 중복을 처리합니다. [Outbox relay의 중복 가능성](https://microservices.io/patterns/data/transactional-outbox.html)

## 7. eventId와 orderId는 왜 둘 다 있나요?

`eventId`는 한 메시지의 식별자이고 `orderId`는 전체 Saga의 상관관계 식별자이자 Kafka key입니다. 새로운 단계는 새로운 eventId를 만들고 같은 주문 ID를 유지합니다. 같은 메시지의 재전달은 inbox로, 다른 eventId를 가진 동일 업무 명령은 주문별 결제·예약 상태로 방어합니다.

꼬리 질문: “POST 재요청도 방어하나요?” → 현재는 새 주문 ID를 생성하므로 방어하지 않습니다. API idempotency key와 요청 결과 저장이 별도로 필요합니다.

## 8. Kafka의 exactly-once면 결제 중복도 막히나요?

Kafka 트랜잭션의 EOS 범위와 외부 DB/PG의 부수 효과를 구분해야 합니다. 이 구현은 Kafka 트랜잭션을 사용하지 않고 DB outbox/inbox를 사용합니다. producer idempotence는 producer 재전송 중복을 줄이지만, outbox relay가 새 send로 재발행하는 모든 업무 중복을 제거하지는 않습니다. [Spring Kafka EOS](https://docs.spring.io/spring-kafka/reference/3.3/kafka/exactly-once.html), [트랜잭션 연동](https://docs.spring.io/spring-kafka/reference/kafka/transactions.html)

## 9. offset은 언제 커밋하나요?

`enable-auto-commit=false`, `ack-mode=record`입니다. 별도 서비스 Bean의 DB 트랜잭션이 커밋되고 listener가 정상 반환하면 컨테이너가 offset을 커밋합니다. DB 커밋과 offset 커밋은 원자적이지 않으므로 그 사이 장애에는 inbox가 필요합니다. 예외를 잡아 로그만 남기고 반환하면 실패 메시지를 성공으로 처리할 수 있어 예외를 전파합니다. [Spring Kafka 컨테이너](https://docs.spring.io/spring-kafka/reference/kafka/receiving-messages/message-listener-container.html)

## 10. 순서는 어떻게 보장하나요?

모든 메시지에 `orderId`를 key로 넣습니다. 같은 토픽에서 파티션 수와 파티셔닝 전략이 유지되는 동안 같은 주문은 같은 파티션으로 갑니다. 서로 다른 토픽 사이에는 전역 순서가 없습니다. 이 예제는 이전 단계 응답을 받아야 다음 명령을 만들고 주문의 현재 상태도 검사합니다. [Kafka 설계](https://kafka.apache.org/41/design/design/)

꼬리 질문: “파티션을 늘리면요?” → 키의 파티션 매핑이 바뀔 수 있으므로 진행 중인 데이터의 순서 요구를 고려한 전환이 필요합니다. 단순 증설만으로 기존 키 순서가 계속 보장된다고 말하면 안 됩니다.

## 11. consumer group은 어떻게 나눴나요?

order-service, payment-service, inventory-service, account-service로 분리했습니다. member-service는 Kafka 소비자가 아닙니다. 동일 서비스의 replica들은 같은 group을 사용해 파티션을 나누어 처리합니다. 서로 독립적인 구독자는 별도 group이 필요합니다. 현재 토픽은 3개 파티션이며 listener concurrency는 1이라, 한 서비스 인스턴스가 자기 토픽의 여러 파티션을 처리합니다.

꼬리 질문: “바로 replica 3개로 늘릴 수 있나요?” → 현재 파일 H2와 단일 publisher 전제가 있어 그대로는 안 됩니다. 서비스별 공유 운영 DB와 outbox 선점·순서 보장을 먼저 설계해야 합니다.

## 12. 재고 부족을 왜 예외 대신 이벤트로 보내나요?

재고 부족은 예상 가능한 업무 결과입니다. 반복해서 같은 요청을 실행한다고 해결된다고 가정할 수 없습니다. `InventoryHandler`는 `STOCK_REJECTED`를 발행해 Saga가 보상하도록 합니다. DB 접속 장애 같은 기술 실패는 예외를 던져 재시도합니다.

## 13. DLT에 보내면 장애 처리가 끝난 건가요?

DLT는 처리하지 못한 메시지를 격리한 곳입니다. 이 코드에서 기본 기술 오류는 최초 1회와 재시도 2회 후 DLT로 가며, 잘못된 메시지는 바로 DLT로 갑니다. 원인 해결, 업무 상태 확인, 재처리/대사와 알림이 운영 절차로 남습니다. DLT 자체는 Saga를 취소하지 않습니다. [오류 처리와 DLT](https://docs.spring.io/spring-kafka/reference/kafka/annotation-error-handling.html)

꼬리 질문: “DLT에 보내는 것도 실패하면요?” → `setFailIfSendResultIsError(true)`로 실패를 전파하여 원본 레코드를 성공으로 처리하지 않습니다.

## 14. 동시 주문에서 초과 판매를 어떻게 막나요?

`update stock set available = available - ? where product_id = ? and available >= ?`로 수량 검사와 차감을 DB에서 원자적으로 수행합니다. Java에서 조회한 수량으로 나중에 덮어쓰지 않습니다. 예약 저장과 inbox/outbox도 같은 트랜잭션입니다. 통합 테스트는 재고 10개에 3개씩 6개 주문을 넣고 3개만 완료되며 재고가 1개 남는지 검사합니다.

## 15. 보상도 실패하면 어떻게 하나요?

현재는 재시도 후 DLT로 이동하며 주문은 `COMPENSATING`으로 남습니다. 실무에서는 보상 진행 시간 감시, 영속적 재시도 정책, 운영자 개입, 실제 PG 결제 결과 대사가 필요합니다. 무조건 취소 완료로 바꾸면 환불되지 않은 주문을 취소 완료로 표시하게 됩니다.

## 16. 타임아웃이면 바로 환불하면 되나요?

응답이 없다는 사실만으로 작업 실패를 확정할 수 없습니다. 결제는 성공했지만 응답만 지연되었을 수 있습니다. 이 예제는 자동 타임아웃을 구현하지 않았습니다. 확장 시 단계별 deadline, 요청 멱등 키, 결과 조회, 늦은 성공 이벤트와 보상 명령의 경합 정책을 함께 설계해야 합니다.

## 17. 2PC보다 Saga가 항상 좋은가요?

요구사항에 따라 다릅니다. Saga는 중간 상태 노출과 명시적인 보상 설계가 필요하며 전체 업무에 대한 ACID 격리를 제공하지 않습니다. 현재 API가 `202`를 반환하고 진행 상태를 조회하게 한 이유입니다. [Saga의 격리와 보상 한계](https://microservices.io/patterns/data/saga.html)

## 18. 실제 결제 API를 붙이면 무엇을 바꿔야 하나요?

새 주문은 `AccountPaymentHandler`가 계좌 서비스에 Kafka 명령을 보내 실제 가상 잔액을 차감·환불합니다. 계좌 ID 없는 `PaymentHandler` 분기는 과거 데이터/기초 실습 호환용 DB 상태 모형입니다. 외부 PG 호출은 로컬 DB 트랜잭션 rollback으로 취소되지 않습니다. 주문/결제 시도별 멱등 키, PG 결과 조회, webhook 중복 처리, 재조정 작업을 설계해야 합니다. `@Transactional`만 붙여 외부 결제와 DB가 함께 원자적으로 처리된다고 설명하지 않습니다.

## 19. 어떤 지표를 보겠나요?

이 예제 확장의 우선 지표는 상태별 Saga 체류 시간, `COMPENSATING` 장기 대기 건수, outbox 건수와 가장 오래된 행의 나이, consumer lag, DLT 유입량, 결제 대사 불일치 건수입니다. orderId를 로그/트레이스 상관관계 키로 사용하면 관련 서비스의 흐름을 추적하기 쉽습니다. 현재 코드는 이 지표의 수집·대시보드를 구현하지 않았습니다.

## 20. 이 프로젝트를 1분 안에 설명해 보세요

> 주문 서비스가 Orchestration Saga로 결제와 재고 예약 순서를 관리합니다. 결제 후 재고가 부족하면 환불 명령을 발행하고, 환불 완료 이벤트를 받은 뒤 주문을 취소합니다. 서비스마다 별도 DB를 사용하며 로컬 변경과 메시지는 Outbox에 함께 저장합니다. Kafka 전달은 중복될 수 있으므로 Inbox와 주문별 결제·예약 상태로 멱등성을 확보했습니다. 업무 실패는 이벤트로 처리하고 기술 실패는 재시도 후 DLT로 격리합니다. 정상 흐름, 보상, 중복, 초과 판매 방지와 DB rollback을 실제 Kafka 통합 테스트로 확인했습니다. 운영으로 확장하려면 보상 실패 복구, PG 대사, API 멱등 키와 outbox 다중 인스턴스 처리가 더 필요합니다.

## 직접 해볼 확장 과제

1. POST `/orders`에 `Idempotency-Key`를 추가하고 같은 키의 재요청이 같은 주문 ID를 반환하게 만들기.
2. `COMPENSATING`이 일정 시간 이상 지속되는 주문을 조회하는 운영 API 만들기.
3. 발송 단계를 추가하여 배송 거절 시 재고 해제 → 결제 환불 순서의 다단계 보상 구현하기.
4. DLT 재처리 시 기존 eventId를 유지하고 현재 업무 상태를 확인하는 절차 설계하기.
5. H2를 서비스별 PostgreSQL로 바꾸고 동시성 테스트를 다시 실행하기.


## 21. DTO에 @Valid가 있는데 서비스에서도 검사하는 이유는?

HTTP가 유일한 호출 경로가 아니기 때문입니다. `AccountRules`는 Bean 직접 호출에서도 음수/초과 금액과 모르는 거래 유형을 거절합니다. DTO는 API 계약, 서비스는 업무 불변식, DB CHECK/UNIQUE는 저장소 제약이라는 서로 다른 경계입니다. `accountRulesProtectNonHttpCallers`가 이를 검증합니다.

## 22. 로그인과 비밀번호 변경도 동시성 문제가 있나요?

옛 비밀번호 확인 후 세션 생성 전에 변경 트랜잭션이 세션들을 삭제하면, 늦게 생성된 세션이 살아남을 수 있습니다. 현재는 같은 회원 행을 잠가 직렬화합니다. 비용은 느린 KDF 동안의 잠금 점유이며 대안은 자격 증명 버전을 확인하는 조건부 세션 발급입니다. 관련 경합 테스트를 설명하세요.

## 23. 잔액이 0원인데 계좌 해지를 막는 이유는?

`DEBITED` 주문은 재고 예약 실패 시 환불받을 수 있습니다. 계좌를 닫아 환불할 수 없게 만들지 않도록 진행 중 주문을 확인합니다. 성공 주문의 SETTLE_ACCOUNT를 처리한 뒤 해지할 수 있습니다. 이 상태는 분산 트랜잭션의 중간 상태가 사용자 기능에도 영향을 준다는 사례입니다.

## 24. 왜 계좌와 결제 서비스를 따로 두었나요?

계좌는 잔액/원장/소유권, 결제는 원격 차감·환불의 진행 상태를 소유합니다. PROCESSING과 CHARGED를 구분하는 학습 가치가 있습니다. 가상 계좌만 지원하는 작은 제품이라면 합치는 편이 단순할 수 있으므로 분리는 무조건적인 정답이 아닙니다.

## 25. 동시 주문 테스트가 통과하면 DB 락이 검증되나요?

항상 그렇지는 않습니다. 이 설정은 단일 consumer여서 주문 입력이 병렬이어도 재고 처리는 순차일 수 있습니다. 별도의 `conditionalStockUpdateSurvivesActualDatabaseContention`이 DB Handler를 독립 트랜잭션 6개에서 동시에 호출해 재고 불변식을 확인합니다. 테스트가 만드는 실제 경합 지점을 설명해야 합니다.

## 26. 프론트엔드에서 요청을 취소하면 주문도 취소되나요?

아닙니다. AbortController는 클라이언트가 응답을 기다리는 동작을 취소합니다. 서버의 이미 커밋된 주문이나 Kafka 명령은 남을 수 있습니다. useOrderObservation은 이전 조회를 취소해 화면 경쟁을 줄이고, 주문 POST 실패는 불확실한 결과로 안내하며 자동 재전송하지 않습니다.

## 27. 거래 내역이 있으면 이벤트 소싱인가요?

아닙니다. 현재 계좌는 잔액 테이블을 직접 갱신하고 거래 내역을 같은 트랜잭션으로 추가합니다. 모든 상태를 이벤트 재생으로 구성하는 모델, 이벤트 버전/스냅샷/프로젝션 재구축을 구현하지 않았습니다.

## 28. 왜 주석을 모든 줄에 달지 않았나요?

문법을 한국어로 반복하면 설계 의도가 묻힙니다. 파일의 존재 이유와 핵심 분기의 불변식·실패 조건·대안을 설명하고, 실행 가능한 테스트를 근거로 연결했습니다. JSON/생성 코드에는 잘못된 주석을 넣지 않고 파일 안내에 역할을 기록했습니다.

더 진행할 과제는 [학습 경로의 우선순위·완료 기준](learning-guide.md#5-추가하면-면접에서-가치가-큰-기능--우선순위와-완료-기준)을 참고하세요.
