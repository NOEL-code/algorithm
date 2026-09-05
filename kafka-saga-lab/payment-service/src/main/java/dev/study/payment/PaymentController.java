package dev.study.payment;

import java.util.Map;
import org.springframework.http.HttpStatus;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;

@RestController
public class PaymentController {
    private final JdbcTemplate db;
    public PaymentController(JdbcTemplate db) { this.db = db; }
    @GetMapping("/payments/{orderId}")
    public Map<String, Object> get(@PathVariable String orderId) {
        var rows = db.queryForList("select * from payments where order_id = ?", orderId);
        if (rows.isEmpty()) throw new ResponseStatusException(HttpStatus.NOT_FOUND);
        var row = rows.getFirst();
        return Map.of("orderId", row.get("ORDER_ID"), "amount", row.get("AMOUNT"), "status", row.get("STATUS"));
    }
}
