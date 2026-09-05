package dev.study.order;

import java.net.URI;
import java.util.Map;
import jakarta.validation.Valid;
import jakarta.validation.constraints.*;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/orders")
public class OrderController {
    public record CreateOrder(@NotBlank @Size(max = 100) String productId,
                              @Min(1) int quantity, @Min(1) long amount, boolean rejectPayment) {}
    private final OrderSaga saga;
    public OrderController(OrderSaga saga) { this.saga = saga; }

    @PostMapping
    public ResponseEntity<Map<String, String>> create(@Valid @RequestBody CreateOrder request) {
        String id = saga.create(request.productId(), request.quantity(), request.amount(), request.rejectPayment());
        return ResponseEntity.accepted().location(URI.create("/orders/" + id))
                .body(Map.of("orderId", id, "status", "PAYMENT_PENDING"));
    }
    @GetMapping("/{id}")
    public Map<String, Object> get(@PathVariable String id) { return saga.get(id); }
}
