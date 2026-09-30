package dev.study.patterns.structural;

import java.util.HashMap;
import java.util.Map;

/**
 * 학습: 공통의 불변 상태를 공유하고 개별 상태는 외부에서 전달한다.
 * 실습: 공유 객체에 좌표를 저장하면 왜 문제가 되는지 설명하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class FlyweightDemo {

    static class Glyph {
        private final char symbol;

        Glyph(char symbol) {
            this.symbol = symbol;
        }

        public char symbol() {
            return symbol;
        }

        String draw(int x, int y) { return symbol + "@" + x + "," + y; }
    }
    static class GlyphPool {
        private final Map<Character, Glyph> cache = new HashMap<>();
        Glyph get(char symbol) {
            Glyph glyph = cache.get(symbol);
            if (glyph == null) {
                glyph = new Glyph(symbol);
                cache.put(symbol, glyph);
            }
            return glyph;
        }
    }
    public static void main(String[] args) {
        GlyphPool pool = new GlyphPool();
        Glyph letter = pool.get('A');
        System.out.println(letter.draw(10, 20));
        System.out.println(letter.draw(30, 40));
    }
}
