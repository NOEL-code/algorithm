import { useEffect, useState } from "react";
import { request, finished } from "../api";

/**
 * 학습: HTTP 202를 완료로 오해하지 않고 최종 상태를 관찰하는 UI 프로토콜.
 * setInterval 대신 요청 완료 후 setTimeout을 예약해 느린 응답이 요청을 겹치게 하지 않는다.
 * ID가 바뀌면 이전 fetch와 타이머를 취소한다. 오래된 응답이 새 주문을 덮어쓰는 경쟁을 막는다.
 * 주문/결제 조회는 서로 다른 DB의 스냅샷이다. 결제 404는 아직 생성 전일 수 있어 별도로 다룬다.
 * 이 Hook은 전체 이벤트 이력을 재구성하지 않는다. 폴링 사이의 중간 상태는 놓칠 수 있다.
 */
export function useOrderObservation(selected, refresh) {
  const [order, setOrder] = useState(null);
  const [payment, setPayment] = useState(null);
  const [error, setError] = useState("");
  const [paymentError, setPaymentError] = useState("");
  const [updated, setUpdated] = useState(null);
  useEffect(() => {
    setOrder(null);
    setPayment(null);
    setError("");
    setPaymentError("");
    setUpdated(null);
    if (!selected) return;
    const controller = new AbortController();
    let timer;
    async function poll() {
      let done = false;
      try {
        const next = await request(`/orders/${encodeURIComponent(selected)}`, {
          signal: controller.signal,
        });
        if (controller.signal.aborted) return;
        setOrder(next);
        setError("");
        setUpdated(new Date());
        try {
          const result = await request(
            `/payments/${encodeURIComponent(selected)}`,
            { signal: controller.signal, allowMissing: true },
          );
          if (controller.signal.aborted) return;
          setPayment(result);
          setPaymentError("");
          done = finished(next.status) && result !== null;
        } catch (e) {
          if (!controller.signal.aborted) {
            setPayment(null);
            setPaymentError(e.message);
          }
        }
      } catch (e) {
        if (!controller.signal.aborted) setError(e.message);
      } finally {
        if (!controller.signal.aborted && !done) timer = setTimeout(poll, 1500);
      }
    }
    poll();
    return () => {
      controller.abort();
      clearTimeout(timer);
    };
  }, [selected, refresh]);

  return { order, payment, error, paymentError, updated };
}
