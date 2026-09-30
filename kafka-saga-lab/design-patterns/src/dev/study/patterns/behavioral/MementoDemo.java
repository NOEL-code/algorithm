package dev.study.patterns.behavioral;

import java.util.ArrayDeque;
import java.util.Deque;

/**
 * 학습: 객체 내부 상태를 외부에 공개하지 않고 저장하고 복원한다.
 * 실습: 여러 스냅샷을 쌓아 다단계 되돌리기를 구현하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class MementoDemo {

    static class Editor {
        private String text = "";
        // Caretaker는 Snapshot을 보관하지만 내부 필드에 접근하지 않는다.
        static final class Snapshot {
            private final String text;
            private Snapshot(String text) { this.text = text; }
        }
        void type(String value) { text += value; }
        String text() { return text; }
        Snapshot save() { return new Snapshot(text); }
        void restore(Snapshot snapshot) { text = snapshot.text; }
    }
    static class History {
        private final Deque<Editor.Snapshot> snapshots = new ArrayDeque<>();
        void backup(Editor editor) { snapshots.push(editor.save()); }
        void undo(Editor editor) { if (!snapshots.isEmpty()) editor.restore(snapshots.pop()); }
    }
    public static void main(String[] args) {
        Editor editor = new Editor();
        History history = new History();
        editor.type("안녕");
        history.backup(editor);
        editor.type(" 자바");
        history.undo(editor);
        System.out.println(editor.text());
    }
}
