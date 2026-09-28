import { defineConfig, loadEnv } from 'vite';

export default defineConfig(({ mode }) => {
  const env = loadEnv(mode, process.cwd(), '');
  const proxy = Object.fromEntries([
    ['orders', env.ORDER_API_URL || 'http://127.0.0.1:8080'],
    ['payments', env.PAYMENT_API_URL || 'http://127.0.0.1:8081'],
    ['inventory', env.INVENTORY_API_URL || 'http://127.0.0.1:8082'],
    ['members', env.MEMBER_API_URL || 'http://127.0.0.1:8083'],
    ['accounts', env.ACCOUNT_API_URL || 'http://127.0.0.1:8084'],
  ].map(([route, target]) => [`/api/${route}`, {
    target, changeOrigin: true, rewrite: (path) => path.replace(/^\/api/, ''),
  }]));
  return {
    server: { port: 5173, strictPort: true, proxy },
    preview: { port: 4173, strictPort: true, proxy },
  };
});
