package dev.study.patterns.creational;

/**
 * 학습: 물류 서비스의 배송 흐름은 유지하고 트럭과 배의 생성만 하위 클래스에 맡긴다.
 * 실습: Airplane과 AirLogistics를 추가하고 기존 물류 클래스의 변경 여부를 살펴보세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class FactoryMethodDemo {

    interface Transport {
        void deliver();
    }

    static class Truck implements Transport {
        @Override
        public void deliver() {
            System.out.println("트럭으로 도로를 따라 배송합니다.");
        }
    }

    static class Ship implements Transport {
        @Override
        public void deliver() {
            System.out.println("배로 바다를 건너 배송합니다.");
        }
    }

    static abstract class Logistics {
        // 하위 클래스가 어떤 운송 수단을 생성할지 결정한다.
        protected abstract Transport createTransport();

        public void planDelivery() {
            Transport transport = createTransport();
            transport.deliver();
        }
    }

    static class RoadLogistics extends Logistics {
        @Override
        protected Transport createTransport() {
            return new Truck();
        }
    }

    static class SeaLogistics extends Logistics {
        @Override
        protected Transport createTransport() {
            return new Ship();
        }
    }

    public static void main(String[] args) {
        Logistics logistics = new RoadLogistics();
        logistics.planDelivery();

        logistics = new SeaLogistics();
        logistics.planDelivery();
    }
}
