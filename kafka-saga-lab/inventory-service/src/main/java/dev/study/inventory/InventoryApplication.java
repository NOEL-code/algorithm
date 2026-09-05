package dev.study.inventory;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.scheduling.annotation.EnableScheduling;

@EnableScheduling
@SpringBootApplication(scanBasePackages = {"dev.study.inventory", "dev.study.common"})
public class InventoryApplication {
    public static void main(String[] args) { SpringApplication.run(InventoryApplication.class, args); }
}
