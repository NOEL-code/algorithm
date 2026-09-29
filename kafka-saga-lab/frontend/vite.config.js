/* 학습: 개발 환경의 동일 출처 프록시
 * 브라우저는 /api만 호출하고 Vite가 실제 서비스로 전달한다. CORS 전체 허용으로 쿠키 문제를 우회하지 않는다.
 * 이 설정은 개발/preview용이다. 배포 환경의 동등한 규칙은 nginx.conf에 있다.
 */
import { defineConfig, loadEnv } from "vite";

export default defineConfig(({ mode }) => {
  const env = loadEnv(mode, process.cwd(), "");
  const proxy = Object.fromEntries(
    [
      ["orders", env.ORDER_API_URL || "http://127.0.0.1:8080"],
      ["payments", env.PAYMENT_API_URL || "http://127.0.0.1:8081"],
      ["inventory", env.INVENTORY_API_URL || "http://127.0.0.1:8082"],
      ["members", env.MEMBER_API_URL || "http://127.0.0.1:8083"],
      ["accounts", env.ACCOUNT_API_URL || "http://127.0.0.1:8084"],
    ].map(([route, target]) => [
      `/api/${route}`,
      {
        target,
        changeOrigin: true,
        rewrite: (path) => path.replace(/^\/api/, ""),
      },
    ]),
  );
  return {
    server: { port: 5173, strictPort: true, proxy },
    preview: { port: 4173, strictPort: true, proxy },
  };
});
