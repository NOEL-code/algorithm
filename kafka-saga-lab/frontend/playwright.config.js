/* 학습: 격리된 종단 간 검증
 * 전용 포트와 메모리 DB를 사용하고 기존 서버 재사용을 금지해 실습 데이터를 변경하지 않는다.
 * 스크린샷과 trace는 실패 재현 자료다. worker=1은 공유 테스트 재고 간섭을 피하는 테스트 환경 선택이다.
 */
import { defineConfig } from "@playwright/test";

export default defineConfig({
  testDir: "./tests",
  fullyParallel: false,
  workers: 1,
  timeout: 60000,
  expect: { timeout: 20000 },
  use: {
    baseURL: "http://127.0.0.1:15173",
    trace: "retain-on-failure",
    screenshot: "only-on-failure",
  },
  webServer: [
    {
      command: "python3 ../scripts/browser-lab.py",
      url: "http://127.0.0.1:18082/inventory/book",
      timeout: 90000,
      reuseExistingServer: false,
      stdout: "ignore",
      stderr: "pipe",
    },
    {
      command: "npm run dev -- --host 127.0.0.1 --port 15173",
      url: "http://127.0.0.1:15173",
      timeout: 30000,
      reuseExistingServer: false,
      env: {
        ORDER_API_URL: "http://127.0.0.1:18080",
        PAYMENT_API_URL: "http://127.0.0.1:18081",
        INVENTORY_API_URL: "http://127.0.0.1:18082",
        MEMBER_API_URL: "http://127.0.0.1:18083",
        ACCOUNT_API_URL: "http://127.0.0.1:18084",
      },
    },
  ],
});
