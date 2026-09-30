package dev.study.patterns.creational;

/**
 * 학습: 컴퓨터의 필수 부품과 선택 부품을 구분하고 build()로 완성된 객체를 만든다.
 * 실습: 선택 부품을 생략한 기본 컴퓨터와 부품을 지정한 컴퓨터를 구성하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class BuilderDemo {

    static class Computer {
        private final String cpu;
        private final int ram;
        private final int storage;
        private final String graphicsCard;

        private Computer(Builder builder) {
            cpu = builder.cpu;
            ram = builder.ram;
            storage = builder.storage;
            graphicsCard = builder.graphicsCard;
        }

        public String getCpu() {
            return cpu;
        }

        public int getRam() {
            return ram;
        }

        public int getStorage() {
            return storage;
        }

        public String getGraphicsCard() {
            return graphicsCard;
        }

        static class Builder {
            private final String cpu;
            private final int ram;
            private int storage = 256;
            private String graphicsCard = "내장 그래픽";

            Builder(String cpu, int ram) {
                this.cpu = cpu;
                this.ram = ram;
            }

            public Builder setStorage(int storage) {
                this.storage = storage;
                return this;
            }

            public Builder setGraphicsCard(String graphicsCard) {
                this.graphicsCard = graphicsCard;
                return this;
            }

            public Computer build() {
                if (cpu == null || cpu.isBlank() || ram <= 0 || storage <= 0) {
                    throw new IllegalArgumentException("CPU와 양수 용량이 필요합니다.");
                }
                return new Computer(this);
            }
        }
    }

    public static void main(String[] args) {
        // CPU와 RAM은 필수, 저장 공간과 그래픽 카드는 선택 사항이다. 용량 단위는 GB다.
        Computer computer = new Computer.Builder("Intel Core i5", 16)
                .setStorage(512)
                .setGraphicsCard("외장 그래픽")
                .build();

        System.out.println(computer.getCpu());
        System.out.println(computer.getRam());
        System.out.println(computer.getStorage());
        System.out.println(computer.getGraphicsCard());
    }
}
