package dev.study.patterns.behavioral;

import java.util.Set;

/**
 * 학습: 작은 언어의 문법을 표현식 객체로 구성하고 해석한다.
 * 실습: Or 표현식을 추가해 복합 조건을 만드세요.
 * 중첩 타입은 한 파일에서 참여 객체의 관계를 읽기 위한 구성이다.
 */
public class InterpreterDemo {

    interface Expression { boolean interpret(Set<String> roles); }
    static class Role implements Expression {
        private final String name;

        Role(String name) {
            this.name = name;
        }

        public String name() {
            return name;
        }

        public boolean interpret(Set<String> roles) { return roles.contains(name); }
    }
    static class And implements Expression {
        private final Expression left;
        private final Expression right;

        And(Expression left, Expression right) {
            this.left = left;
            this.right = right;
        }

        public Expression left() {
            return left;
        }

        public Expression right() {
            return right;
        }

        public boolean interpret(Set<String> roles) { return left.interpret(roles) && right.interpret(roles); }
    }
    static class Not implements Expression {
        private final Expression expression;

        Not(Expression expression) {
            this.expression = expression;
        }

        public Expression expression() {
            return expression;
        }

        public boolean interpret(Set<String> roles) { return !expression.interpret(roles); }
    }
    public static void main(String[] args) {
        // 문법: 역할 | (표현식 AND 표현식) | NOT 표현식. 문자열 파서는 생략한다.
        Expression rule = new And(new Role("MEMBER"), new Not(new Role("BLOCKED")));
        boolean allowed = rule.interpret(Set.of("MEMBER"));
        System.out.println(allowed);
    }
}
