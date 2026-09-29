/**
 * 학습: 읽기 모델과 업무 명령의 구분 재고 조회는 현재 스냅샷일 뿐 예약 보장이 아니다. 변경은 Kafka 명령을 처리하는 Handler만 수행한다. 상품이 없으면 404를
 * 반환하고 HTTP 계층에서 임의로 재고를 생성하지 않는다. 읽은 재고가 충분해도 실제 예약 시점에는 부족할 수 있음을 주문 실습으로 확인한다.
 */
package dev.study.inventory;

import org.springframework.http.HttpStatus;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;

import java.util.Map;

@RestController
public class InventoryController {
    private final JdbcTemplate db;

    public InventoryController(JdbcTemplate db) {
        this.db = db;
    }

    @GetMapping("/inventory/{productId}")
    public Map<String, Object> get(@PathVariable String productId) {
        var rows =
                db.queryForList(
                        "select available from stock where product_id = ?",
                        Integer.class,
                        productId);
        if (rows.isEmpty()) throw new ResponseStatusException(HttpStatus.NOT_FOUND);
        return Map.of("productId", productId, "available", rows.getFirst());
    }
}
