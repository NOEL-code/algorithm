package dev.study.patterns.creational;

import java.util.ArrayList;
import java.util.List;

/**
 * 학습: 기존 객체를 복제하되 가변 필드는 독립적으로 복사한다.
 * 실습: List 안의 원소도 가변 객체라면 복사 방법이 어떻게 달라지는지 실험하세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class PrototypeDemo {

    static class Template {
        final List<String> sections;
        Template(List<String> sections) { this.sections = new ArrayList<>(sections); }
        Template copy() { return new Template(sections); }
    }
    public static void main(String[] args) {
        Template original = new Template(List.of("제목"));
        Template copy = original.copy();
        copy.sections.add("본문");
        System.out.println(original.sections);
        System.out.println(copy.sections);
    }
}
