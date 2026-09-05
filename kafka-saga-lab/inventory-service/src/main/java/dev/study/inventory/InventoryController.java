package dev.study.inventory;

import java.util.Map;
import org.springframework.http.HttpStatus;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;

@RestController
public class InventoryController {
    private final JdbcTemplate db;
    public InventoryController(JdbcTemplate db) { this.db = db; }
    @GetMapping("/inventory/{productId}")
    public Map<String, Object> get(@PathVariable String productId) {
        var rows = db.queryForList("select available from stock where product_id = ?", Integer.class, productId);
        if (rows.isEmpty()) throw new ResponseStatusException(HttpStatus.NOT_FOUND);
        return Map.of("productId", productId, "available", rows.getFirst());
    }
}
