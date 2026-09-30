# Java 디자인 패턴 실습 — GoF 23개

생성 5개, 구조 7개, 행위 11개를 각각 독립적인 Java 예제로 제공합니다.
Java 21 JDK만 필요하며 Spring, Maven, Kafka, DB 없이 실행합니다.
기존 서비스와 독립된 소스 폴더이며 Maven reactor 모듈은 아닙니다.

## 읽는 방법

각 `*Demo.java`는 표준 Java만 사용하는 패턴 구현과 짧은 `main()` 사용 예시로 구성됩니다.
별도 검증 도우미나 실행 스크립트 없이 코드를 읽으며 객체 사이의 관계를 살펴보세요.

1. 파일의 학습 주석을 읽고 인터페이스·구현체·클라이언트를 찾습니다.
2. `main()`에서 객체 생성과 호출 흐름을 따라갑니다.
3. 아래 변경 과제를 구현하며 패턴의 확장 지점을 살펴봅니다.
4. 패턴을 쓰지 않는 코드와 비교해 어떤 변경이 쉬워지고 타입이 얼마나 늘어나는지 설명합니다.

각 예제는 패턴의 협력 구조를 작게 보여주는 교육용 코드입니다.
금액 오버플로, 모든 입력 검증, 동시성, 영속성, 네트워크 실패 처리는 다루지 않습니다.
특히 Singleton 이외의 캐시·프록시·상태 객체는 스레드 안전성을 보장하지 않습니다.

## 생성 패턴 — 객체를 어떻게 만들까?

| 예제 | 핵심 | 변경 과제 |
|---|---|---|
| [Singleton](src/dev/study/patterns/creational/SingletonDemo.java) | 설정 관리자를 private 생성자와 정적 인스턴스로 하나만 생성한다. | getInstance()를 여러 곳에서 호출해도 같은 설정 객체를 사용하는 흐름을 따라가세요. |
| [FactoryMethod](src/dev/study/patterns/creational/FactoryMethodDemo.java) | 물류 서비스의 배송 흐름은 유지하고 트럭과 배의 생성만 하위 클래스에 맡긴다. | Airplane과 AirLogistics를 추가하고 기존 물류 클래스의 변경 여부를 살펴보세요. |
| [AbstractFactory](src/dev/study/patterns/creational/AbstractFactoryDemo.java) | Windows 또는 Mac 팩토리 하나로 같은 운영체제의 버튼과 체크박스를 생성한다. | LinuxFactory를 추가한 뒤 Application 수정 없이 UI 제품군을 교체하세요. |
| [Builder](src/dev/study/patterns/creational/BuilderDemo.java) | 컴퓨터의 필수 부품과 선택 부품을 구분하고 build()로 완성된 객체를 만든다. | 선택 부품을 생략한 기본 컴퓨터와 부품을 지정한 컴퓨터를 구성하세요. |
| [Prototype](src/dev/study/patterns/creational/PrototypeDemo.java) | 기존 도형의 상태를 복사해 같은 종류의 새 도형을 만든다. | Rectangle을 추가하고 Shape 타입으로 copy()를 호출해 보세요. |

## 구조 패턴 — 객체를 어떻게 조합할까?

| 예제 | 핵심 | 변경 과제 |
|---|---|---|
| [Adapter](src/dev/study/patterns/structural/AdapterDemo.java) | 기존 API를 클라이언트가 원하는 인터페이스로 변환한다. | 레거시 반환 코드가 실패인 경우를 추가하세요. |
| [Bridge](src/dev/study/patterns/structural/BridgeDemo.java) | 추상 기능과 구현 수단을 분리해 두 축을 독립적으로 확장한다. | 긴급 알림과 메신저 채널을 각각 추가하세요. |
| [Composite](src/dev/study/patterns/structural/CompositeDemo.java) | 단일 항목과 항목 묶음을 같은 인터페이스로 다룬다. | 여러 단계로 중첩된 묶음의 합계를 검증하세요. |
| [Decorator](src/dev/study/patterns/structural/DecoratorDemo.java) | 같은 인터페이스의 객체를 감싸 기능을 누적한다. | 우유 데코레이터를 두 번 감쌌을 때 결과를 예측하세요. |
| [Facade](src/dev/study/patterns/structural/FacadeDemo.java) | 여러 하위 시스템을 사용하는 순서를 간단한 진입점으로 제공한다. | 결제 실패 시 재고 예약 취소가 필요한 이유를 설명하세요. |
| [Flyweight](src/dev/study/patterns/structural/FlyweightDemo.java) | 공통의 불변 상태를 공유하고 개별 상태는 외부에서 전달한다. | 공유 객체에 좌표를 저장하면 왜 문제가 되는지 설명하세요. |
| [Proxy](src/dev/study/patterns/structural/ProxyDemo.java) | 동일 인터페이스의 대리 객체가 실제 객체 접근과 생성 시점을 제어한다. | 권한 검사 프록시를 만들고 거절 시 실제 호출이 없는지 확인하세요. |

## 행위 패턴 — 책임과 상호작용을 어떻게 나눌까?

| 예제 | 핵심 | 변경 과제 |
|---|---|---|
| [ChainOfResponsibility](src/dev/study/patterns/behavioral/ChainOfResponsibilityDemo.java) | 요청을 처리기 사슬로 전달하고 조건에 따라 중단한다. | 금액 상한 처리기를 추가하고 뒤 처리기가 호출되지 않는지 확인하세요. |
| [Command](src/dev/study/patterns/behavioral/CommandDemo.java) | 요청을 객체로 만들어 실행과 취소를 호출자에서 분리한다. | 여러 명령을 실행하고 역순으로 취소하는 이력을 추가하세요. |
| [Interpreter](src/dev/study/patterns/behavioral/InterpreterDemo.java) | 작은 언어의 문법을 표현식 객체로 구성하고 해석한다. | Or 표현식을 추가해 복합 조건을 만드세요. |
| [Iterator](src/dev/study/patterns/behavioral/IteratorDemo.java) | 내부 저장 구조를 노출하지 않고 순차 접근을 제공한다. | 서로 다른 두 iterator의 진행 위치가 독립적인지 확인하세요. |
| [Mediator](src/dev/study/patterns/behavioral/MediatorDemo.java) | 객체 사이의 직접 참조를 중재자로 모아 상호작용을 조정한다. | 퇴장 기능을 추가하고 퇴장한 사용자에게 메시지가 오지 않는지 확인하세요. |
| [Memento](src/dev/study/patterns/behavioral/MementoDemo.java) | 객체 내부 상태를 외부에 공개하지 않고 저장하고 복원한다. | 여러 스냅샷을 쌓아 다단계 되돌리기를 구현하세요. |
| [Observer](src/dev/study/patterns/behavioral/ObserverDemo.java) | 발행자가 구독자의 구체 타입을 몰라도 변경을 통지한다. | 통지 중 구독 해제와 구독자 예외를 어떤 정책으로 처리할지 정하세요. |
| [State](src/dev/study/patterns/behavioral/StateDemo.java) | 현재 상태 객체가 동작과 다음 상태를 결정한다. | 취소 상태를 추가하고 배송 후 취소를 거절하세요. |
| [Strategy](src/dev/study/patterns/behavioral/StrategyDemo.java) | 알고리즘을 공통 인터페이스 뒤에 두고 호출자가 교체한다. | 고정 금액 할인 전략을 추가하고 음수 결제 금액을 방지하세요. |
| [TemplateMethod](src/dev/study/patterns/behavioral/TemplateMethodDemo.java) | 상위 클래스가 처리 순서를 고정하고 일부 단계를 하위 클래스에 맡긴다. | 검증 단계를 추가하고 하위 클래스가 순서를 바꿀 수 없는지 확인하세요. |
| [Visitor](src/dev/study/patterns/behavioral/VisitorDemo.java) | 요소 구조와 작업을 분리하고 이중 디스패치로 타입별 작업을 선택한다. | 요소 수정 없이 이름을 수집하는 새 Visitor를 추가하세요. |

## 헷갈리는 패턴 비교

- Factory Method는 상속으로 제품 생성 지점을 바꾸고, Abstract Factory는 관련 제품군을 함께 생성합니다.
- Adapter는 인터페이스를 맞추고, Bridge는 기능과 구현의 두 확장 축을 분리합니다.
- Decorator는 기능을 누적하고, Proxy는 실제 객체에 대한 접근을 제어합니다.
- Strategy는 호출자가 알고리즘을 선택하고, State는 상태에 따라 동작과 전이가 달라집니다.
- Template Method는 상속으로 일부 단계를 바꾸고, Strategy는 객체 합성으로 알고리즘을 교체합니다.
- Command는 실행 요청과 취소 동작을 캡슐화하고, Memento는 복원할 상태를 저장합니다.
- Observer는 구독자에게 변화를 통지하고, Mediator는 참여 객체 사이의 상호작용 규칙을 모읍니다.
- Visitor는 연산 추가가 쉽지만 요소 타입 추가 시 방문자들을 수정해야 합니다.

## 기존 프로젝트와 연결해서 생각하기

이 실습은 서비스에 패턴을 강제로 적용하지 않습니다. 주문 상태와 State 예제를 비교할 때,
분산 Saga에는 상태 저장·중복 처리·보상이 별도로 필요함을 설명해 보세요.
Observer 예제의 동기 통지와 Kafka 전달 보장도 구분해 보세요.
Facade의 호출 묶음만으로 분산 트랜잭션이 생기지는 않습니다.
