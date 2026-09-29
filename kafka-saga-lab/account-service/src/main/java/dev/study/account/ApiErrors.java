/**
 * 학습: 실패를 HTTP 계약으로 번역 예상 가능한 업무 예외와 DTO 검증 오류만 사용자 메시지로 바꾼다. SQL/스택/비밀번호를 응답으로 노출하지 않는다. 모든 예외를
 * 200으로 감싸면 호출자는 실패를 판별할 수 없다. 각 서비스에 두어 독립적인 오류 계약 확장을 허용한다.
 */
package dev.study.account;

import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.MethodArgumentNotValidException;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;

import java.util.Map;

@RestControllerAdvice
public class ApiErrors {
    // 도메인 오류를 전송 계약으로 번역한다. 배치/Bean 직접 호출은 HTTP 예외에 의존하지 않는다.
    @ExceptionHandler(AccountRules.InvalidMovement.class)
    public ResponseEntity<?> movement(AccountRules.InvalidMovement e) {
        return ResponseEntity.badRequest().body(Map.of("message", e.getMessage()));
    }

    @ExceptionHandler(ResponseStatusException.class)
    public ResponseEntity<?> status(ResponseStatusException e) {
        return ResponseEntity.status(e.getStatusCode())
                .body(Map.of("message", e.getReason() == null ? "요청 실패" : e.getReason()));
    }

    @ExceptionHandler(MethodArgumentNotValidException.class)
    public ResponseEntity<?> validation(MethodArgumentNotValidException e) {
        return ResponseEntity.badRequest().body(Map.of("message", "입력한 값의 형식과 길이를 확인해 주세요."));
    }
}
