package dev.study.patterns.behavioral;

import java.util.ArrayDeque;
import java.util.Deque;

/**
 * 학습: 요청을 객체로 만들어 실행과 취소를 호출자에서 분리한다.
 * 실습: 여러 명령을 실행하고 역순으로 취소하는 이력을 추가하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class CommandDemo {

    static class Lamp { boolean on; }
    interface Command { void execute(); void undo(); }
    static class TurnOn implements Command {
        private final Lamp lamp;
        private boolean previous;
        TurnOn(Lamp lamp) { this.lamp = lamp; }
        public void execute() { previous = lamp.on; lamp.on = true; }
        public void undo() { lamp.on = previous; }
    }
    static class Remote {
        private final Deque<Command> history = new ArrayDeque<>();
        void press(Command command) { command.execute(); history.push(command); }
        void undo() { if (!history.isEmpty()) history.pop().undo(); }
    }
    public static void main(String[] args) {
        Lamp lamp = new Lamp();
        Remote remote = new Remote();
        remote.press(new TurnOn(lamp));
        remote.press(new TurnOn(lamp)); // 실행마다 새 명령 객체로 이전 상태 보존
        remote.undo();
        remote.undo();
    }
}
