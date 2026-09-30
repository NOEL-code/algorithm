package dev.study.patterns.behavioral;

import java.util.ArrayList;
import java.util.List;

/**
 * 학습: 객체 사이의 직접 참조를 중재자로 모아 상호작용을 조정한다.
 * 실습: 퇴장 기능을 추가하고 퇴장한 사용자에게 메시지가 오지 않는지 확인하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class MediatorDemo {

    static class ChatRoom {
        private final List<User> users = new ArrayList<>();
        void join(User user) { users.add(user); }
        void send(User sender, String message) {
            for (User user : users) if (user != sender) user.inbox.add(sender.name + ":" + message);
        }
    }
    static class User {
        final String name;
        final ChatRoom room;
        final List<String> inbox = new ArrayList<>();
        User(String name, ChatRoom room) { this.name = name; this.room = room; room.join(this); }
        void send(String message) { room.send(this, message); }
    }
    public static void main(String[] args) {
        ChatRoom room = new ChatRoom();
        User alice = new User("Alice", room);
        User bob = new User("Bob", room);
        alice.send("안녕");
        System.out.println(bob.inbox);
    }
}
