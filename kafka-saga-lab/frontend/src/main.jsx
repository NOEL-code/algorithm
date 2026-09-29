// 학습: 진입점은 DOM 마운트만 담당한다. 화면 상태는 App, 비동기 주문 관찰은 전용 Hook으로 분리한다.
// StrictMode의 개발용 effect 재실행은 취소/정리 코드가 실제로 안전한지 드러내는 연습이다.
import React from "react";
import { createRoot } from "react-dom/client";
import App from "./App";
import "./styles.css";

createRoot(document.getElementById("root")).render(
  <React.StrictMode>
    <App />
  </React.StrictMode>,
);
