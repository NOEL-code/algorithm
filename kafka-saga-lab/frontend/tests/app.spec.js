/* 학습: 사용자 시나리오로 API 계약 검증
 * 가입→입금→결제→환불을 실제 백엔드로 확인한다. 성공 토스트만 보지 않고 잔액/거래 내역도 확인한다.
 * 실패 전용 테스트만 네트워크를 차단해 응답 유실을 재현한다. 서버 실패와 응답 유실을 구분하는 UX를 확인한다.
 */
import { test, expect } from "@playwright/test";

test("회원가입 → 계좌 입금 → 주문 차감 → 재고 부족 환불 → 거래 내역", async ({
  page,
}) => {
  const errors = [];
  page.on("pageerror", (error) => errors.push(error.message));
  await page.goto("/");
  await page
    .getByRole("button", { name: "회원 · 계좌 관리", exact: true })
    .click();
  await page.getByRole("button", { name: "회원가입", exact: true }).click();
  await page.getByLabel("이름", { exact: true }).fill("테스트 사용자");
  await page
    .getByLabel("이메일", { exact: true })
    .fill(`browser-${Date.now()}@example.test`);
  await page
    .getByLabel("비밀번호", { exact: true })
    .fill("browser-password-123");
  await page.getByRole("button", { name: "가입하고 시작하기" }).click();
  await expect(
    page.getByRole("heading", { name: "회원 정보 관리" }),
  ).toBeVisible();
  await page.getByLabel("계좌 별칭", { exact: true }).fill("생활비");
  await page.getByRole("button", { name: "계좌 개설하기" }).click();
  await expect(
    page.getByRole("heading", { name: "생활비 관리" }),
  ).toBeVisible();
  await page.getByLabel("거래 금액 (원)").fill("20000000");
  await page.getByRole("button", { name: "거래 실행" }).click();
  await expect(page.getByRole("status")).toContainText("20,000,000원");
  await page.screenshot({
    path: "test-results/accounts-desktop.png",
    fullPage: true,
  });
  await page.getByRole("button", { name: "주문 실험실", exact: true }).click();
  await expect(page.getByLabel("결제 계좌").locator("option")).toHaveCount(2);
  await page.getByLabel("결제 계좌").selectOption({ index: 1 });
  await page.getByRole("button", { name: "주문 생성하기" }).click();
  await expect(page.locator(".order-content .status-message")).toContainText(
    "결제와 재고 예약이 모두 완료",
  );
  await expect(page.getByLabel("결제 계좌")).toContainText("19,990,000원");
  await page.screenshot({
    path: "test-results/orders-desktop.png",
    fullPage: true,
  });
  await page.getByRole("button", { name: "재고 부족", exact: true }).click();
  await page.getByRole("button", { name: "주문 생성하기" }).click();
  await expect(page.locator(".order-content .status-message")).toContainText(
    "결제 금액을 환불",
  );
  await expect(page.locator(".payment")).toContainText("환불 완료");
  await expect(page.getByLabel("결제 계좌")).toContainText("19,990,000원");
  await page
    .getByRole("button", { name: "회원 · 계좌 관리", exact: true })
    .click();
  await page.getByRole("button", { name: /생활비.*19,990,000원/ }).click();
  await expect(
    page.getByRole("cell", { name: "주문 환불", exact: true }),
  ).toBeVisible();
  await expect(
    page.getByRole("cell", { name: "주문 결제", exact: true }),
  ).toHaveCount(2);
  await page.getByLabel("이름", { exact: true }).fill("변경된 사용자");
  await page.getByRole("button", { name: "정보 저장" }).click();
  await expect(
    page.getByRole("heading", { name: "변경된 사용자님" }),
  ).toBeVisible();
  await page.getByRole("button", { name: "로그아웃", exact: true }).click();
  await expect(page.getByRole("button", { name: "로그인하기" })).toBeVisible();
  expect(errors).toEqual([]);
});

test("잘못된 로그인과 모바일 화면의 오류 상태", async ({ page }) => {
  await page.setViewportSize({ width: 390, height: 844 });
  await page.route("**/api/inventory/**", (route) =>
    route.fulfill({ status: 503, contentType: "application/json", body: "{}" }),
  );
  await page.goto("/");
  await expect(page.getByRole("alert")).toContainText("503");
  await expect(
    page.getByRole("button", { name: "주문 생성하기" }),
  ).toBeDisabled();
  await page.screenshot({ path: "test-results/mobile.png", fullPage: true });
  expect(
    await page.evaluate(
      () => document.documentElement.scrollWidth <= window.innerWidth,
    ),
  ).toBe(true);
  await page
    .getByRole("button", { name: "회원 · 계좌 관리", exact: true })
    .click();
  await page.getByLabel("이메일", { exact: true }).fill("missing@example.test");
  await page.getByLabel("비밀번호", { exact: true }).fill("wrong-password");
  await page.getByRole("button", { name: "로그인하기" }).click();
  await expect(page.getByRole("alert")).toContainText("이메일 또는 비밀번호");
});

async function fundedMember(page, balance = 10000) {
  const response = await page.request.post("/api/members", {
    data: {
      email: `member-${crypto.randomUUID()}@example.test`,
      name: "주문 회원",
      password: "test-password-123",
    },
  });
  expect(response.status()).toBe(201);
  const created = await page.request.post("/api/accounts", {
    data: { name: "테스트 계좌" },
  });
  const { id } = await created.json();
  await page.request.post(`/api/accounts/${id}/transactions`, {
    data: { type: "DEPOSIT", amount: balance, requestId: crypto.randomUUID() },
  });
  await page.goto("/");
  await expect(page.getByLabel("결제 계좌").locator("option")).toHaveCount(2);
  await page.getByLabel("결제 계좌").selectOption(id);
  return id;
}

test("잔액 부족으로 주문 취소, 결제 거절 상태 표시", async ({ page }) => {
  await fundedMember(page, 1);
  await page.getByRole("button", { name: "주문 생성하기" }).click();
  await expect(page.locator(".order-content .status-message")).toContainText(
    "잔액 부족",
  );
  await expect(page.locator(".payment")).toContainText("결제 거절");
  await expect(page.getByLabel("결제 계좌")).toContainText("1원");
});

test("주문 응답 유실 시 자동 재전송하지 않고 안내", async ({ page }) => {
  await fundedMember(page);
  let posts = 0;
  await page.route("**/api/orders", async (route) => {
    if (route.request().method() === "POST") {
      posts++;
      await route.abort();
    } else await route.continue();
  });
  await page.getByRole("button", { name: "주문 생성하기" }).click();
  await expect(page.getByRole("alert")).toContainText("별도 주문이 생성됩니다");
  await expect(
    page.getByRole("button", { name: "주문 생성하기" }),
  ).toBeEnabled();
  expect(posts).toBe(1);
});

test("모바일에서 계좌 별칭 변경·전액 출금·해지와 비밀번호 변경", async ({
  page,
}) => {
  await fundedMember(page, 10000);
  const member = await (await page.request.get("/api/members/me")).json();
  await page.setViewportSize({ width: 390, height: 844 });
  await page
    .getByRole("button", { name: "회원 · 계좌 관리", exact: true })
    .click();
  await expect(
    page.getByRole("button", { name: "로그아웃", exact: true }),
  ).toBeVisible();
  await page.getByRole("button", { name: /테스트 계좌.*10,000원/ }).click();
  await page.getByLabel("계좌 별칭 변경").fill("정리할 계좌");
  await page.getByRole("button", { name: "변경", exact: true }).click();
  await expect(
    page.getByRole("heading", { name: "정리할 계좌 관리" }),
  ).toBeVisible();
  await page.getByLabel("거래 구분").selectOption("WITHDRAW");
  await page.getByRole("button", { name: "거래 실행" }).click();
  await expect(page.getByRole("status")).toContainText("출금 10,000원");
  page.once("dialog", (dialog) => dialog.accept());
  await page
    .getByRole("button", { name: "계좌 해지 (잔액 0원일 때 가능)" })
    .click();
  await expect(page.locator(".account-detail .badge")).toHaveText(
    "해지된 계좌",
  );
  await page.locator("summary").click();
  await page.getByLabel("현재 비밀번호").fill("test-password-123");
  await page.getByLabel("새 비밀번호").fill("changed-password-456");
  await page.getByRole("button", { name: "비밀번호 변경하기" }).click();
  await expect(page.getByRole("status")).toContainText("다시 로그인");
  await page.getByLabel("이메일", { exact: true }).fill(member.email);
  await page
    .getByLabel("비밀번호", { exact: true })
    .fill("changed-password-456");
  await page.getByRole("button", { name: "로그인하기" }).click();
  await expect(
    page.getByRole("heading", { name: "회원 정보 관리" }),
  ).toBeVisible();
  expect(
    await page.evaluate(
      () => document.documentElement.scrollWidth <= window.innerWidth,
    ),
  ).toBe(true);
  await page.screenshot({
    path: "test-results/accounts-mobile.png",
    fullPage: true,
  });
});
