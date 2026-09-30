package dev.study.patterns.behavioral;

import java.util.Iterator;
import java.util.List;
import java.util.NoSuchElementException;

/**
 * 학습: 내부 저장 구조를 노출하지 않고 순차 접근을 제공한다.
 * 실습: 서로 다른 두 iterator의 진행 위치가 독립적인지 확인하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class IteratorDemo {

    static class Playlist implements Iterable<String> {
        private final List<String> songs;
        Playlist(String... songs) { this.songs = List.of(songs); }
        public Iterator<String> iterator() {
            return new Iterator<>() {
                private int index;
                public boolean hasNext() { return index < songs.size(); }
                public String next() {
                    if (!hasNext()) throw new NoSuchElementException();
                    return songs.get(index++);
                }
            };
        }
    }
    public static void main(String[] args) {
        Playlist playlist = new Playlist("A", "B");
        for (String song : playlist) {
            System.out.println(song);
        }
    }
}
