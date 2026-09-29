# 파일별 설계 이유와 학습 포인트

[학습 경로](learning-guide.md)의 순서대로 읽으세요. 아래는 직접 작성·관리하는 모든 코드와 설정의 찾아보기입니다. 파일 머리의 `학습` 주석은 존재 이유와 한계, 메서드/분기 주석은 해당 결정이 필요한 실패 상황을 설명합니다. 단순 문법을 줄마다 번역하는 대신 행동을 바꾸면 어떤 보장이 깨지는지에 집중합니다.

## member-service

| 파일 | 존재 이유와 학습 포인트 |
|---|---|
| [member-service/pom.xml](../member-service/pom.xml) | member 모듈의 의존성과 독립 실행 산출물 / exec classifier는 실행 JAR와 테스트가 의존하는 일반 JAR를 분리한다. / 업무 서비스가 서로의 Java 모듈을 의존하지 않도록 HTTP/메시지 계약을 사용한다. |
| [member-service/src/main/java/dev/study/member/ApiErrors.java](../member-service/src/main/java/dev/study/member/ApiErrors.java) | 실패를 HTTP 계약으로 번역 / 예상 가능한 업무 예외와 DTO 검증 오류만 사용자 메시지로 바꾼다. SQL/스택/비밀번호를 응답으로 노출하지 않는다. / 모든 예외를 200으로 감싸면 호출자는 실패를 판별할 수 없다. 각 서비스에 두어 독립적인 오류 계약 확장을 허용한다. |
| [member-service/src/main/java/dev/study/member/MemberApplication.java](../member-service/src/main/java/dev/study/member/MemberApplication.java) | 독립 배포 단위와 Bean 탐색 범위 / Spring 컨테이너와 해당 서비스의 DB/HTTP 경계를 시작한다. 다른 서비스의 업무 Bean을 스캔하지 않는다. / EnableScheduling이 있는 서비스만 Outbox relay를 스케줄링한다. 회원 서비스는 Kafka 업무 명령을 발행하지 않는다. |
| [member-service/src/main/java/dev/study/member/MemberController.java](../member-service/src/main/java/dev/study/member/MemberController.java) | 인증의 HTTP 경계 / DTO는 입력 형식, Service는 업무 흐름을 맡는다. 회원 생성은 201, 상태 없는 완료는 204다. / 쿠키를 응답 본문과 분리하고 HttpOnly/SameSite/Secure의 서로 다른 역할을 배운다. / MemberService.current와 MemberSessions를 따라가 인증과 계좌 소유권 인가를 구분한다. |
| [member-service/src/main/java/dev/study/member/MemberService.java](../member-service/src/main/java/dev/study/member/MemberService.java) | 영속 세션과 자격 증명 수명 / 회원 생성과 세션 저장을 같은 트랜잭션으로 묶어 가입만 성공한 반쪽 상태를 줄인다. / 정규화한 이메일의 UNIQUE가 동시 가입을 최종 방어한다. 사전 SELECT만으로는 부족하다. / JWT 대신 서버 세션을 선택해 즉시 폐기를 관찰한다. 그 대가로 매 인증 시 DB/회원 서비스에 의존한다. |
| [member-service/src/main/java/dev/study/member/Passwords.java](../member-service/src/main/java/dev/study/member/Passwords.java) | 비밀번호와 세션 토큰의 위협 모델 차이 / 사람의 비밀번호에는 salt와 느린 KDF, 충분히 무작위인 세션 토큰에는 빠른 SHA-256을 사용한다. / PBKDF2는 JDK 표준 구현에 위임한다. 비밀번호를 암호화해 복호화하거나 직접 암호 알고리즘을 만들지 않는다. / 현재 저장 형식에는 반복 횟수가 없다. ITERATIONS를 바꾸기 전에 버전/비용 메타데이터와 점진 재해시가 필요하다. |
| [member-service/src/main/resources/application.yml](../member-service/src/main/resources/application.yml) | member 서비스 실행 경계 / 파일 H2는 서비스당 단일 인스턴스용이다. replica만 늘리면 서로 다른 DB로 업무 상태가 갈라질 수 있다. / 환경 변수로 주소를 주입해 로컬/Compose/Kubernetes에서 같은 코드를 실행한다. |
| [member-service/src/main/resources/member-schema.sql](../member-service/src/main/resources/member-schema.sql) | 정규화 이메일 UNIQUE는 동시 가입의 최종 방어다. 세션에는 토큰 원문 대신 해시를 저장한다. / 세션→회원 FK는 같은 DB 안에서만 사용한다. 만료 데이터 정리는 인증 검사와 별개다. |

## account-service

| 파일 | 존재 이유와 학습 포인트 |
|---|---|
| [account-service/pom.xml](../account-service/pom.xml) | account 모듈의 의존성과 독립 실행 산출물 / exec classifier는 실행 JAR와 테스트가 의존하는 일반 JAR를 분리한다. / 업무 서비스가 서로의 Java 모듈을 의존하지 않도록 HTTP/메시지 계약을 사용한다. |
| [account-service/src/main/java/dev/study/account/AccountApplication.java](../account-service/src/main/java/dev/study/account/AccountApplication.java) | 독립 배포 단위와 Bean 탐색 범위 / Spring 컨테이너와 해당 서비스의 DB/HTTP 경계를 시작한다. 다른 서비스의 업무 Bean을 스캔하지 않는다. / EnableScheduling이 있는 서비스만 Outbox relay를 스케줄링한다. 회원 서비스는 Kafka 업무 명령을 발행하지 않는다. |
| [account-service/src/main/java/dev/study/account/AccountController.java](../account-service/src/main/java/dev/study/account/AccountController.java) | 인증된 주체에서 시작하는 API / memberId를 요청 본문으로 받지 않고 세션 확인 결과를 Service에 넘긴다. UUID를 안다고 권한이 생기지는 않는다. / 입금/출금과 별칭 변경/해지는 다른 업무이므로 명시적인 API로 분리한다. / @Valid는 HTTP 경계 검증이다. AccountRules의 서비스 내부 방어 및 DB CHECK와 비교한다. |
| [account-service/src/main/java/dev/study/account/AccountListener.java](../account-service/src/main/java/dev/study/account/AccountListener.java) | 전송 계층과 DB 트랜잭션 경계 / Listener는 JSON을 해석하고 별도 Handler Bean을 호출한다. 프록시를 통과해야 @Transactional이 적용된다. / Handler 커밋 후 정상 반환해야 offset을 진행한다. 예외를 잡아 로그만 남기면 업무 실패가 소비 성공으로 처리될 수 있다. |
| [account-service/src/main/java/dev/study/account/AccountOrderHandler.java](../account-service/src/main/java/dev/study/account/AccountOrderHandler.java) | 서비스 간 보상과 계좌 로컬 원자성 / 잔액·주문별 계좌 원장·거래 내역·Inbox·Outbox를 하나의 계좌 DB 트랜잭션으로 처리한다. / 결제/환불은 orderId 업무 원장으로 중복을 막는다. eventId가 달라도 금전 효과는 반복하지 않는다. / 환불은 이전 DB 트랜잭션의 rollback이 아니라 CREDIT_ACCOUNT라는 새 거래다. |
| [account-service/src/main/java/dev/study/account/AccountRules.java](../account-service/src/main/java/dev/study/account/AccountRules.java) | HTTP에 의존하지 않는 금액·거래 유형 불변식 |
| [account-service/src/main/java/dev/study/account/AccountService.java](../account-service/src/main/java/dev/study/account/AccountService.java) | 같은 자원에 적용되는 일관된 잠금 규약 / 수동 입출금·별칭 변경·해지는 계좌 행을 잠근다. AccountOrderHandler도 같은 행을 먼저 잠근다. / 잠금을 잡은 채 잔액, 환불 가능 금액, 거래 원장을 검사·갱신해야 읽고 쓰는 사이에 값이 바뀌지 않는다. / requestId 재사용 시 이전 결과를 반환한다. 멱등성은 락과 달리 순차적으로 반복된 요청도 방어한다. |
| [account-service/src/main/java/dev/study/account/ApiErrors.java](../account-service/src/main/java/dev/study/account/ApiErrors.java) | 실패를 HTTP 계약으로 번역 / 예상 가능한 업무 예외와 DTO 검증 오류만 사용자 메시지로 바꾼다. SQL/스택/비밀번호를 응답으로 노출하지 않는다. / 모든 예외를 200으로 감싸면 호출자는 실패를 판별할 수 없다. 각 서비스에 두어 독립적인 오류 계약 확장을 허용한다. |
| [account-service/src/main/resources/account-schema.sql](../account-service/src/main/resources/account-schema.sql) | 잔액 CHECK, 요청 유일 키, 거래 원장이 애플리케이션 버그에 대한 마지막 방어선이다. / member_id에는 다른 서비스 DB의 FK를 걸지 않는다. 소유권은 회원 API/업무 명령으로 검증한다. / account_orders의 DEBITED 금액은 환불 가능 공간이므로 잔액과 함께 상한을 계산한다. |
| [account-service/src/main/resources/application.yml](../account-service/src/main/resources/application.yml) | account 서비스 실행 경계 / 파일 H2는 서비스당 단일 인스턴스용이다. replica만 늘리면 서로 다른 DB로 업무 상태가 갈라질 수 있다. / 환경 변수로 주소를 주입해 로컬/Compose/Kubernetes에서 같은 코드를 실행한다. |

## order-service

| 파일 | 존재 이유와 학습 포인트 |
|---|---|
| [order-service/pom.xml](../order-service/pom.xml) | order 모듈의 의존성과 독립 실행 산출물 / exec classifier는 실행 JAR와 테스트가 의존하는 일반 JAR를 분리한다. / 업무 서비스가 서로의 Java 모듈을 의존하지 않도록 HTTP/메시지 계약을 사용한다. |
| [order-service/src/main/java/dev/study/order/OrderAccounts.java](../order-service/src/main/java/dev/study/order/OrderAccounts.java) | 서비스 간 HTTP 계약과 TOCTOU / 계좌 DB를 직접 조회하지 않고 소유자가 인증된 API를 호출해 서비스 경계를 지킨다. / 연결/응답 시간 제한을 둔다. 상대 서비스 장애를 무기한 대기로 전파하지 않지만 이중 검증 비용은 생긴다. / 조회 직후 상태가 바뀔 수 있다. 확인 결과를 분산 락이나 결제 성공 보장으로 해석하지 않는다. |
| [order-service/src/main/java/dev/study/order/OrderApplication.java](../order-service/src/main/java/dev/study/order/OrderApplication.java) | 독립 배포 단위와 Bean 탐색 범위 / Spring 컨테이너와 해당 서비스의 DB/HTTP 경계를 시작한다. 다른 서비스의 업무 Bean을 스캔하지 않는다. / EnableScheduling이 있는 서비스만 Outbox relay를 스케줄링한다. 회원 서비스는 Kafka 업무 명령을 발행하지 않는다. |
| [order-service/src/main/java/dev/study/order/OrderController.java](../order-service/src/main/java/dev/study/order/OrderController.java) | 접수 성공과 업무 완료의 차이 / HTTP 202와 Location은 주문 접수 사실만 알린다. 프론트엔드는 주문 상태를 별도로 조회한다. / 계좌 서비스의 사전 확인은 사용자에게 빠른 오류를 주는 용도다. 이후 계좌가 닫힐 수 있어 소비 시에도 재검증한다. / 조회도 getOwned를 거쳐 소유권을 검사한다. 서버가 발급한 UUID만으로 접근 제어를 대체하지 않는다. |
| [order-service/src/main/java/dev/study/order/OrderListener.java](../order-service/src/main/java/dev/study/order/OrderListener.java) | 전송 계층과 DB 트랜잭션 경계 / Listener는 JSON을 해석하고 별도 Handler Bean을 호출한다. 프록시를 통과해야 @Transactional이 적용된다. / Handler 커밋 후 정상 반환해야 offset을 진행한다. 예외를 잡아 로그만 남기면 업무 실패가 소비 성공으로 처리될 수 있다. |
| [order-service/src/main/java/dev/study/order/OrderSaga.java](../order-service/src/main/java/dev/study/order/OrderSaga.java) | 명시적 상태 머신으로 표현하는 Orchestration Saga / 주문 서비스는 다음 명령을 결정하고 각 서비스가 자기 DB를 수정하도록 한다. 전역 트랜잭션은 없다. / 현재 상태 + 받은 이벤트로 전이를 제한하고 행 잠금으로 같은 주문의 경쟁을 직렬화한다. / 환불 요청과 환불 완료를 분리한다. COMPENSATING은 실패를 숨기지 않는 업무 상태다. |
| [order-service/src/main/resources/application.yml](../order-service/src/main/resources/application.yml) | order 서비스 실행 경계 / 파일 H2는 서비스당 단일 인스턴스용이다. replica만 늘리면 서로 다른 DB로 업무 상태가 갈라질 수 있다. / 환경 변수로 주소를 주입해 로컬/Compose/Kubernetes에서 같은 코드를 실행한다. |
| [order-service/src/main/resources/order-schema.sql](../order-service/src/main/resources/order-schema.sql) | 주문 상태는 프로세스 메모리가 아니라 DB에 남겨 재시작 후 이어간다. / ADD COLUMN IF NOT EXISTS는 기존 실습 DB를 유지하는 멱등 초기화다. 운영의 버전 관리 마이그레이션을 대체하지 않는다. |

## payment-service

| 파일 | 존재 이유와 학습 포인트 |
|---|---|
| [payment-service/pom.xml](../payment-service/pom.xml) | payment 모듈의 의존성과 독립 실행 산출물 / exec classifier는 실행 JAR와 테스트가 의존하는 일반 JAR를 분리한다. / 업무 서비스가 서로의 Java 모듈을 의존하지 않도록 HTTP/메시지 계약을 사용한다. |
| [payment-service/src/main/java/dev/study/payment/AccountPaymentHandler.java](../payment-service/src/main/java/dev/study/payment/AccountPaymentHandler.java) | 원격 작업 요청과 성공 확정의 분리 / PROCESSING에서 계좌 응답을 기다린 뒤 CHARGED/REJECTED로 바뀐다. DB에 명령을 저장했다고 차감이 끝난 것은 아니다. / REFUND_PENDING도 같은 원리다. 주문 서비스에는 계좌 환불 확인 후 PAYMENT_REFUNDED를 보낸다. / 원장과 메시지의 회원·계좌·금액 일치 검사는 같은 주문 ID에 다른 요청을 섞는 오류를 탐지한다. |
| [payment-service/src/main/java/dev/study/payment/PaymentApplication.java](../payment-service/src/main/java/dev/study/payment/PaymentApplication.java) | 독립 배포 단위와 Bean 탐색 범위 / Spring 컨테이너와 해당 서비스의 DB/HTTP 경계를 시작한다. 다른 서비스의 업무 Bean을 스캔하지 않는다. / EnableScheduling이 있는 서비스만 Outbox relay를 스케줄링한다. 회원 서비스는 Kafka 업무 명령을 발행하지 않는다. |
| [payment-service/src/main/java/dev/study/payment/PaymentController.java](../payment-service/src/main/java/dev/study/payment/PaymentController.java) | 조회 API에서도 필요한 소유권 검사 / 인증된 회원 ID와 주문 ID를 함께 WHERE에 넣는다. 다른 회원의 결제 존재 여부도 404로 감춘다. / 결제 상태는 별도 DB의 조회 결과라 주문 상태와 동시에 바뀌지 않을 수 있다. / 프론트엔드의 결제 404 대기 처리와 비교해 최종 일관성을 설명한다. |
| [payment-service/src/main/java/dev/study/payment/PaymentHandler.java](../payment-service/src/main/java/dev/study/payment/PaymentHandler.java) | 과거 메시지 호환 경로와 현재 경로의 구분 / 새 HTTP 주문은 AccountPaymentHandler로 보내 실제 가상 잔액을 사용한다. / 계좌 ID 없는 분기는 기존 데이터/기초 Saga 테스트를 위한 상태만의 결제 모형이다. 새 API에서는 생성하지 않는다. / 호환 분기를 운영 기능으로 오해하지 말 것. 제거하려면 기존 메시지·DB 마이그레이션 전략이 필요하다. |
| [payment-service/src/main/java/dev/study/payment/PaymentListener.java](../payment-service/src/main/java/dev/study/payment/PaymentListener.java) | 전송 계층과 DB 트랜잭션 경계 / Listener는 JSON을 해석하고 별도 Handler Bean을 호출한다. 프록시를 통과해야 @Transactional이 적용된다. / Handler 커밋 후 정상 반환해야 offset을 진행한다. 예외를 잡아 로그만 남기면 업무 실패가 소비 성공으로 처리될 수 있다. |
| [payment-service/src/main/resources/application.yml](../payment-service/src/main/resources/application.yml) | payment 서비스 실행 경계 / 파일 H2는 서비스당 단일 인스턴스용이다. replica만 늘리면 서로 다른 DB로 업무 상태가 갈라질 수 있다. / 환경 변수로 주소를 주입해 로컬/Compose/Kubernetes에서 같은 코드를 실행한다. |
| [payment-service/src/main/resources/payment-schema.sql](../payment-service/src/main/resources/payment-schema.sql) | order_id PK는 같은 주문 결제를 두 번 만드는 경쟁을 방어한다. / PROCESSING/REFUND_PENDING은 원격 계좌 결과가 아직 확정되지 않았음을 표현한다. |

## inventory-service

| 파일 | 존재 이유와 학습 포인트 |
|---|---|
| [inventory-service/pom.xml](../inventory-service/pom.xml) | inventory 모듈의 의존성과 독립 실행 산출물 / exec classifier는 실행 JAR와 테스트가 의존하는 일반 JAR를 분리한다. / 업무 서비스가 서로의 Java 모듈을 의존하지 않도록 HTTP/메시지 계약을 사용한다. |
| [inventory-service/src/main/java/dev/study/inventory/InventoryApplication.java](../inventory-service/src/main/java/dev/study/inventory/InventoryApplication.java) | 독립 배포 단위와 Bean 탐색 범위 / Spring 컨테이너와 해당 서비스의 DB/HTTP 경계를 시작한다. 다른 서비스의 업무 Bean을 스캔하지 않는다. / EnableScheduling이 있는 서비스만 Outbox relay를 스케줄링한다. 회원 서비스는 Kafka 업무 명령을 발행하지 않는다. |
| [inventory-service/src/main/java/dev/study/inventory/InventoryController.java](../inventory-service/src/main/java/dev/study/inventory/InventoryController.java) | 읽기 모델과 업무 명령의 구분 / 재고 조회는 현재 스냅샷일 뿐 예약 보장이 아니다. 변경은 Kafka 명령을 처리하는 Handler만 수행한다. / 상품이 없으면 404를 반환하고 HTTP 계층에서 임의로 재고를 생성하지 않는다. / 읽은 재고가 충분해도 실제 예약 시점에는 부족할 수 있음을 주문 실습으로 확인한다. |
| [inventory-service/src/main/java/dev/study/inventory/InventoryHandler.java](../inventory-service/src/main/java/dev/study/inventory/InventoryHandler.java) | 조건부 UPDATE로 지키는 재고 불변식 / available >= quantity 검사와 차감을 한 SQL로 묶는다. @Transactional만 붙인 조회 후 덮어쓰기는 안전하지 않다. / 상품별 DB 행 잠금은 다른 주문 ID/파티션에서 들어온 동일 상품 경쟁도 조정한다. / 재고 부족은 업무 결과 이벤트, DB 오류는 예외다. 재시도 가능한 장애와 정상 거절을 구분한다. |
| [inventory-service/src/main/java/dev/study/inventory/InventoryListener.java](../inventory-service/src/main/java/dev/study/inventory/InventoryListener.java) | 전송 계층과 DB 트랜잭션 경계 / Listener는 JSON을 해석하고 별도 Handler Bean을 호출한다. 프록시를 통과해야 @Transactional이 적용된다. / Handler 커밋 후 정상 반환해야 offset을 진행한다. 예외를 잡아 로그만 남기면 업무 실패가 소비 성공으로 처리될 수 있다. |
| [inventory-service/src/main/resources/application.yml](../inventory-service/src/main/resources/application.yml) | inventory 서비스 실행 경계 / 파일 H2는 서비스당 단일 인스턴스용이다. replica만 늘리면 서로 다른 DB로 업무 상태가 갈라질 수 있다. / 환경 변수로 주소를 주입해 로컬/Compose/Kubernetes에서 같은 코드를 실행한다. |
| [inventory-service/src/main/resources/inventory-schema.sql](../inventory-service/src/main/resources/inventory-schema.sql) | available >= 0 CHECK와 조건부 UPDATE는 각각 저장소 제약과 정상 업무 분기라는 다른 역할이다. / reservations의 order_id PK로 새 eventId의 동일 예약 명령도 두 번 차감하지 않는다. |

## common

| 파일 | 존재 이유와 학습 포인트 |
|---|---|
| [common/pom.xml](../common/pom.xml) | 공통 메시지/전달/인증 클라이언트 기술을 모은다. 공용 업무 DB나 공유 엔티티 모듈이 아니다. / 클라이언트 코드도 공유 배포 결합을 만든다. 서비스별 계약 버전이 필요해지면 분리할 수 있다. |
| [common/src/main/java/dev/study/common/Inbox.java](../common/src/main/java/dev/study/common/Inbox.java) | 중복 메시지와 원자적 업무 처리 / seen 조회만으로는 동시 중복을 막지 못한다. event_id PK와 동일 트랜잭션 rollback이 최종 방어다. / MANDATORY는 호출자가 연 트랜잭션을 요구한다. 별도 REQUIRES_NEW로 기록하면 업무 실패 후에도 처리 완료로 남을 수 있다. / 같은 메시지 중복은 eventId, 새 ID의 같은 업무는 orderId/requestId 원장으로 구분한다. |
| [common/src/main/java/dev/study/common/Json.java](../common/src/main/java/dev/study/common/Json.java) | 역직렬화 경계의 실패 분류 / JSON 파싱 및 필수 필드 검사를 업무 트랜잭션 앞에서 수행한다. Java record라고 입력이 자동 검증되지는 않는다. / 잘못된 메시지는 IllegalArgumentException으로 분류해 동일한 독성 메시지를 반복 재시도하지 않는다. / 형식 검증과 원장 대조는 다르다. 값이 양수여도 다른 주문의 금액이면 Handler가 추가 검증해야 한다. |
| [common/src/main/java/dev/study/common/KafkaInfrastructure.java](../common/src/main/java/dev/study/common/KafkaInfrastructure.java) | 전달 설정과 복구 정책을 업무 실패와 구분하기 / 3개 파티션은 병렬 처리 단위, 복제 계수 1은 로컬 실습 제약이다. acks=all이 복제 수를 늘려주지는 않는다. / 기술 오류는 제한적으로 재시도하고 DLT에 격리한다. DLT 발행 실패 시 원본을 성공 처리하지 않도록 설정한다. / DLT는 환불/취소 완료가 아니다. 현재 업무 상태 확인·재처리·대사는 별도 운영 절차다. |
| [common/src/main/java/dev/study/common/MemberSessions.java](../common/src/main/java/dev/study/common/MemberSessions.java) | 인증 정보의 신뢰 경계와 가용성 비용 / 클라이언트가 보낸 회원 ID 대신 회원 서비스가 검증한 세션의 회원 ID를 사용한다. / 회원 서비스 장애는 503, 무효 세션은 401로 나눈다. 장애 시 임의로 인증을 통과시키지 않는다. / 매 요청 HTTP 확인은 즉시 폐기와 단순성을 얻지만 지연/가용성 의존성이 생긴다. JWT/캐시와 비교할 출발점이다. |
| [common/src/main/java/dev/study/common/Message.java](../common/src/main/java/dev/study/common/Message.java) | 메시지 식별자와 업무 식별자의 분리 / eventId는 특정 메시지, orderId는 Saga 상관관계와 Kafka key다. next는 전자는 바꾸고 후자는 유지한다. / memberId/accountId는 인증된 주문에서 파생한다. 이 필드가 존재한다고 Kafka 생산자가 인증되는 것은 아니다. / 단일 record는 학습 편의다. 운영에서는 버전별 명령/이벤트 계약과 호환성 정책을 나눌 수 있다. |
| [common/src/main/java/dev/study/common/Outbox.java](../common/src/main/java/dev/study/common/Outbox.java) | DB와 메시지 시스템 사이의 이중 쓰기 문제 / 업무 변경과 발행할 메시지를 같은 로컬 DB에 저장한다. Kafka로 직접 보내면 두 시스템의 성공을 원자적으로 맞출 수 없다. / MANDATORY로 업무 트랜잭션 참여를 강제하고 실제 네트워크 전송은 Publisher로 분리한다. / orderId를 key로 저장해 같은 주문의 파티션 배치가 유지되도록 한다. |
| [common/src/main/java/dev/study/common/OutboxPublisher.java](../common/src/main/java/dev/study/common/OutboxPublisher.java) | 발행 보장과 정확히 한 번의 차이 / Kafka ACK를 기다린 뒤 outbox를 지운다. ACK 후 DB 커밋 전 장애라면 재발행되므로 Inbox가 필요하다. / DB 행 잠금을 보유한 채 네트워크를 기다리는 단순 구현이다. 느린 전송이 DB 자원을 점유하는 비용을 관찰한다. / 서비스당 한 Publisher 전제다. SKIP LOCKED를 붙여도 같은 주문의 다중 발행 순서가 자동 보장되지는 않는다. |
| [common/src/main/java/dev/study/common/Topics.java](../common/src/main/java/dev/study/common/Topics.java) | 명령 수신자와 결과 수신자의 경계 / 명령은 payment/inventory/account 토픽, Saga 결과는 replies로 나눠 누가 처리해야 하는지 드러낸다. / 서로 다른 consumer group은 같은 레코드를 독립적으로 읽는다. 그룹을 합치면 서비스 간 메시지가 분산될 수 있다. / 토픽 이름을 공통 계약으로 묶되 DB 테이블이나 업무 로직까지 공통 모듈에 공유하지 않는다. |
| [common/src/main/resources/common-schema.sql](../common/src/main/resources/common-schema.sql) | Inbox는 수신 중복, Outbox는 발행 의도를 저장한다. 공통 DDL이어도 각 서비스의 별도 DB에 생성한다. / Inbox event_id PK 충돌은 업무 변경까지 rollback시킨다. Outbox identity는 로컬 발행 순서이며 전역 이벤트 순서가 아니다. |

## frontend

| 파일 | 존재 이유와 학습 포인트 |
|---|---|
| [frontend/.dockerignore](../frontend/.dockerignore) | 호스트 의존성과 테스트 산출물의 이미지 혼입 방지 / 이미지 안에서 npm ci/build를 실행하므로 호스트 node_modules/dist를 복사하지 않는다. |
| [frontend/.env.example](../frontend/.env.example) | 설정과 코드 분리 / 주소는 Vite 서버가 읽는다. VITE_ 접두사로 브라우저에 노출하는 비밀 값과 혼동하지 않는다. |
| [frontend/Dockerfile](../frontend/Dockerfile) | 빌드 도구와 실행 환경의 분리 / Node 단계에서 번들만 만들고 Nginx 이미지에는 정적 산출물을 복사한다. npm ci는 lockfile 계약을 사용한다. |
| [frontend/docker-start.sh](../frontend/docker-start.sh) | 환경 의존 주소의 런타임 주입 / Docker와 Kubernetes의 DNS 주소가 달라 resolv.conf에서 읽고 Nginx 템플릿에 주입한다. exec로 종료 신호를 전달한다. |
| [frontend/index.html](../frontend/index.html) | React 마운트와 모바일/접근성 기본값 / lang은 문서 언어, viewport는 작은 화면의 레이아웃 기준이다. 실제 화면 생성은 main.jsx에 위임한다. |
| [frontend/nginx.conf](../frontend/nginx.conf) | 정적 웹과 API 라우팅의 배포 경계 / /api를 제거해 기존 Spring 경로로 전달하고 서비스 이름을 재해석해 컨테이너 교체를 따른다. / 프록시는 Saga 조정자나 인증 서버가 아니다. 각 서비스가 세션/소유권을 확인한다. |
| [frontend/playwright.config.js](../frontend/playwright.config.js) | 격리된 종단 간 검증 / 전용 포트와 메모리 DB를 사용하고 기존 서버 재사용을 금지해 실습 데이터를 변경하지 않는다. / 스크린샷과 trace는 실패 재현 자료다. worker=1은 공유 테스트 재고 간섭을 피하는 테스트 환경 선택이다. |
| [frontend/src/App.jsx](../frontend/src/App.jsx) | 주문 화면의 조합과 관찰 상태 / 회원 변경 시 이전 회원의 선택·최근 주문을 지워 UI의 데이터 혼선을 막는다. API 인가는 서버에서 별도로 수행한다. / 계좌/재고는 조회 시점 값이다. 버튼 활성화나 화면 검증이 서버의 잔액/재고 검증을 대신하지 않는다. |
| [frontend/src/MemberArea.jsx](../frontend/src/MemberArea.jsx) | 회원/계좌 API의 사용자 흐름 / 회원 상태, 요청 중 상태, 입력값은 수명이 달라 별도로 관리한다. ref 잠금은 재렌더 전의 중복 클릭도 막는다. / 같은 화면에서 실패한 동일 입출금에는 requestId를 유지한다. 서버 원장이 실제 멱등성을 보장한다. / 주의: ref는 탭 이동/새로고침에 영속적이지 않다. 불확실한 거래를 복구하려면 키 영속화/결과 조회가 추가로 필요하다. |
| [frontend/src/api.js](../frontend/src/api.js) | HTTP 공통 경계와 실패 의미 / 쿠키는 브라우저가 전송한다. HttpOnly 토큰을 JavaScript로 읽거나 localStorage에 보관하지 않는다. / 요청 취소와 8초 타임아웃은 서버 작업 rollback이 아니다. 쓰기 요청을 자동 재시도하지 않는다. / 204는 본문 없음, 선택적 404는 아직 없음이다. 그 밖의 실패를 정상 데이터로 숨기지 않는다. |
| [frontend/src/hooks/useOrderObservation.js](../frontend/src/hooks/useOrderObservation.js) | 진입점 분리 / effect 취소 / 겹치지 않는 폴링 / 최종 일관성 관찰 |
| [frontend/src/main.jsx](../frontend/src/main.jsx) | 진입점 분리 / effect 취소 / 겹치지 않는 폴링 / 최종 일관성 관찰 |
| [frontend/src/styles.css](../frontend/src/styles.css) | 학습 화면의 정보 계층과 접근성 / 상태는 색뿐 아니라 텍스트로 표현하고 키보드 focus와 작은 화면의 입력/버튼 접근을 유지한다. / 스타일은 표시 책임만 가진다. disabled나 숨김은 접근 제어가 아니며 서버가 권한을 검사한다. |
| [frontend/tests/app.spec.js](../frontend/tests/app.spec.js) | 사용자 시나리오로 API 계약 검증 / 가입→입금→결제→환불을 실제 백엔드로 확인한다. 성공 토스트만 보지 않고 잔액/거래 내역도 확인한다. / 실패 전용 테스트만 네트워크를 차단해 응답 유실을 재현한다. 서버 실패와 응답 유실을 구분하는 UX를 확인한다. |
| [frontend/vite.config.js](../frontend/vite.config.js) | 개발 환경의 동일 출처 프록시 / 브라우저는 /api만 호출하고 Vite가 실제 서비스로 전달한다. CORS 전체 허용으로 쿠키 문제를 우회하지 않는다. / 이 설정은 개발/preview용이다. 배포 환경의 동등한 규칙은 nginx.conf에 있다. |

## integration-tests

| 파일 | 존재 이유와 학습 포인트 |
|---|---|
| [integration-tests/pom.xml](../integration-tests/pom.xml) | 테스트에서만 서비스 모듈들을 한 JVM에 조립한다. 운영 서비스 간 직접 코드 호출을 권장하는 구조가 아니다. / Embedded Kafka로 메시지 송신을 mock하지 않고 완료 조건을 검증한다. |
| [integration-tests/src/test/java/dev/study/BrowserLab.java](../integration-tests/src/test/java/dev/study/BrowserLab.java) | 재현 가능한 E2E 환경과 운영 데이터 격리 / 브라우저 테스트 전용 Kafka/메모리 DB/포트로 HTTP→Kafka→DB→UI 전체 경로를 실행한다. / mock 응답만 통과하는 UI 테스트와 달리 DTO·프록시·쿠키·비동기 계약의 불일치를 발견한다. / 프로세스 종료 훅은 테스트 자원을 정리한다. 강제 종료 복구 보장을 제공하는 운영 장치는 아니다. |
| [integration-tests/src/test/java/dev/study/SagaIntegrationTest.java](../integration-tests/src/test/java/dev/study/SagaIntegrationTest.java) | 안전성 주장을 실행 가능한 증거로 만들기 / 실제 Embedded Kafka와 분리된 H2 DB를 사용해 원장·잔액·상태의 결과를 검증한다. / 동시 주문 E2E와 DB Handler 직접 경합 테스트는 서로 다른 증거다. consumer concurrency=1을 락 검증으로 오해하지 않는다. / 메모리 DB 테스트는 운영 DB 격리 수준, 디스크 장애, 네트워크 분단까지 입증하지 않는다. |
| [integration-tests/src/test/resources/logback-test.xml](../integration-tests/src/test/resources/logback-test.xml) | 테스트 로그는 실패 원인을 남기되 반복적인 내부 로그를 줄인다. / 로그 출력 성공은 트랜잭션 커밋 증거가 아니므로 테스트는 DB 결과도 확인한다. |

## scripts

| 파일 | 존재 이유와 학습 포인트 |
|---|---|
| [scripts/browser-lab.py](../scripts/browser-lab.py) | 검증 환경을 코드로 재현 / Maven 검증 리포트의 클래스패스를 재사용해 실제 라이브러리 버전으로 테스트 서버를 실행한다. / 먼저 verify가 필요하다는 의존성을 명시하고 shell 문자열 대신 인자 배열로 JVM을 실행한다. |
| [scripts/check-learning-docs.py](../scripts/check-learning-docs.py) | 학습 주석·문서 링크 누락 검사 / 정확성 검증과 문서 존재 검사의 차이 |

## k8s

| 파일 | 존재 이유와 학습 포인트 |
|---|---|
| [k8s/account-service.yaml](../k8s/account-service.yaml) | 서비스 주소, 실행 인스턴스, 데이터 수명의 분리 / Service는 안정적 주소, Deployment는 교체 가능한 프로세스, PVC는 DB 저장 공간이다. / 파일 H2를 위해 replicas=1/Recreate를 사용한다. TCP probe가 Saga 완료나 DB 정상까지 증명하지는 않는다. |
| [k8s/frontend.yaml](../k8s/frontend.yaml) | 정적 프론트엔드 배포 / 브라우저가 아니라 Nginx Pod가 내부 서비스 DNS를 사용한다. 외부 노출은 port-forward로 제한한 로컬 실습이다. |
| [k8s/inventory-service.yaml](../k8s/inventory-service.yaml) | 서비스 주소, 실행 인스턴스, 데이터 수명의 분리 / Service는 안정적 주소, Deployment는 교체 가능한 프로세스, PVC는 DB 저장 공간이다. / 파일 H2를 위해 replicas=1/Recreate를 사용한다. TCP probe가 Saga 완료나 DB 정상까지 증명하지는 않는다. |
| [k8s/kafka.yaml](../k8s/kafka.yaml) | 메시지 로그의 영속성과 로컬 가용성 한계 / StatefulSet/PVC가 재시작 후 로그를 유지한다. 단일 브로커이므로 복제 기반 장애 내성은 실습하지 않는다. |
| [k8s/kustomization.yaml](../k8s/kustomization.yaml) | 배포 설정의 조합과 버전 추적 / 공통 환경 값/이미지를 한 곳에서 선언한다. Secret 대신 ConfigMap에 비밀번호를 넣는 패턴으로 확장하지 않는다. |
| [k8s/member-service.yaml](../k8s/member-service.yaml) | 서비스 주소, 실행 인스턴스, 데이터 수명의 분리 / Service는 안정적 주소, Deployment는 교체 가능한 프로세스, PVC는 DB 저장 공간이다. / 파일 H2를 위해 replicas=1/Recreate를 사용한다. TCP probe가 Saga 완료나 DB 정상까지 증명하지는 않는다. |
| [k8s/namespace.yaml](../k8s/namespace.yaml) | 리소스 이름과 정리 범위의 격리 / Namespace는 논리적 묶음이다. 네트워크 보안 경계는 별도 정책이 필요하다. |
| [k8s/order-service.yaml](../k8s/order-service.yaml) | 서비스 주소, 실행 인스턴스, 데이터 수명의 분리 / Service는 안정적 주소, Deployment는 교체 가능한 프로세스, PVC는 DB 저장 공간이다. / 파일 H2를 위해 replicas=1/Recreate를 사용한다. TCP probe가 Saga 완료나 DB 정상까지 증명하지는 않는다. |
| [k8s/payment-service.yaml](../k8s/payment-service.yaml) | 서비스 주소, 실행 인스턴스, 데이터 수명의 분리 / Service는 안정적 주소, Deployment는 교체 가능한 프로세스, PVC는 DB 저장 공간이다. / 파일 H2를 위해 replicas=1/Recreate를 사용한다. TCP probe가 Saga 완료나 DB 정상까지 증명하지는 않는다. |

## 공통 실행 설정

| 파일 | 존재 이유와 학습 포인트 |
|---|---|
| [.dockerignore](../.dockerignore) | 백엔드 이미지 빌드 컨텍스트 최소화 / 프론트엔드는 별도 컨텍스트다. 로컬 DB와 로그를 이미지로 보내지 않는다. |
| [.gitignore](../.gitignore) | 소스와 재생성 가능한 실행 산출물의 구분 / DB/로그/의존성/브라우저 테스트 결과는 커밋 대상에서 제외한다. lockfile은 재현성을 위해 포함한다. |
| [Dockerfile](../Dockerfile) | Java 멀티 스테이지 빌드 / Maven/JDK 빌드 단계와 JRE 실행 단계를 분리한다. SERVICE 인자로 같은 빌드에서 서비스별 산출물을 선택한다. |
| [compose.yml](../compose.yml) | 실행 가능한 로컬 서비스 토폴로지 / depends_on의 시작 순서와 애플리케이션의 실제 준비 완료는 다르다. Kafka는 healthcheck를 기다린다. / DB/Kafka 볼륨은 프로세스와 별개다. down -v는 이 실습의 영속 데이터를 삭제한다. |
| [pom.xml](../pom.xml) | Maven reactor는 빌드 순서를 관리한다. 하나의 빌드가 하나의 런타임/DB를 뜻하지 않는다. / Java/Spring 버전을 부모에서 맞춰 모듈 간 의존성 충돌을 줄인다. |

## 주석을 직접 넣지 않는 파일

| 파일 | 이유와 배울 내용 |
|---|---|
| [frontend/package.json](../frontend/package.json) | JSON은 주석을 허용하지 않는다. scripts는 개발/빌드/검증 진입점, engines는 실행 요구사항, devDependencies는 빌드 도구 경계다. |
| [frontend/package-lock.json](../frontend/package-lock.json) | npm 생성물이다. 의존성 그래프/무결성을 고정해 npm ci로 재현한다. 직접 편집하거나 교육 주석을 넣지 않는다. |
| [mvnw](../mvnw), [mvnw.cmd](../mvnw.cmd) | Maven Wrapper 배포 원본이다. OS별 Maven 기동/다운로드를 제공하며 애플리케이션 로직이 아니다. 원본 유지가 임의 주석 삽입보다 낫다. |
| [.mvn/wrapper/maven-wrapper.properties](../.mvn/wrapper/maven-wrapper.properties) | Wrapper가 사용할 Maven 배포 버전/주소다. 실행 환경 재현성을 배우며 런타임 설정과 구분한다. |
| [README.md](../README.md), [k8s/README.md](../k8s/README.md) | 실행 절차·API·데이터 초기화·배포 한계의 기준 문서다. 코드 주석과 문서는 같은 계약을 설명해야 한다. |
| [docs/interview.md](interview.md), [학습 경로](learning-guide.md), 이 파일 | 답변 연습, 실험 순서, 파일별 찾아보기로 목적을 나눴다. 소스 주석을 대체하지 않는다. |

`target/`, `dist/`, `node_modules/`, 테스트 스크린샷/로그/리포트, 파일 DB는 생성 산출물입니다. 주석 대상이 아니며 소스/테스트로 다시 생성합니다. 개인 IDE·에이전트 설정 및 `.github/modernize/` 업그레이드 도구 생성 파일은 프로젝트 업무 코드가 아니므로 수정하지 않습니다.
