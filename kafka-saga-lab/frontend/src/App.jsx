/* 학습: 주문 화면의 조합과 관찰 상태
 * 회원 변경 시 이전 회원의 선택·최근 주문을 지워 UI의 데이터 혼선을 막는다. API 인가는 서버에서 별도로 수행한다.
 * 계좌/재고는 조회 시점 값이다. 버튼 활성화나 화면 검증이 서버의 잔액/재고 검증을 대신하지 않는다.
 */
import React, { useEffect, useRef, useState } from "react";
import { request, finished } from "./api";
import { useOrderObservation } from "./hooks/useOrderObservation";
import MemberArea from "./MemberArea";

const labels = {
  PAYMENT_PENDING: "결제 대기",
  STOCK_PENDING: "재고 예약 중",
  COMPENSATING: "환불 진행 중",
  COMPLETED: "주문 완료",
  CANCELLED: "주문 취소",
  PROCESSING: "계좌 차감 중",
  REFUND_PENDING: "계좌 환불 중",
  CHARGED: "결제 완료",
  REJECTED: "결제 거절",
  REFUNDED: "환불 완료",
};
const reasons = {
  PAYMENT_REJECTED:
    "잔액 부족, 사용 불가능한 계좌 또는 거절 시뮬레이션으로 결제가 거절되었습니다.",
  OUT_OF_STOCK: "재고가 부족하여 결제 금액을 환불했습니다.",
};
const won = (value) => new Intl.NumberFormat("ko-KR").format(value) + "원";
function Badge({ status }) {
  return (
    <span className={`badge ${status?.toLowerCase() || ""}`}>
      {labels[status] || status}
    </span>
  );
}

export default function App() {
  const [tab, setTab] = useState("orders");
  const [form, setForm] = useState({
    productId: "book",
    quantity: 1,
    amount: 10000,
    rejectPayment: false,
  });
  const [history, setHistory] = useState([]);
  const [member, setMember] = useState(null);
  const [accounts, setAccounts] = useState([]);
  const [accountId, setAccountId] = useState("");
  const [accountError, setAccountError] = useState("");
  const [selected, setSelected] = useState("");
  const [lookup, setLookup] = useState("");
  const [stock, setStock] = useState(null);
  const [stockError, setStockError] = useState("");
  const [submitError, setSubmitError] = useState("");
  const [submitting, setSubmitting] = useState(false);
  const [refresh, setRefresh] = useState(0);
  const { order, payment, error, paymentError, updated } = useOrderObservation(
    selected,
    refresh,
  );
  const submittingRef = useRef(false);
  const selectedProduct = order?.productId || form.productId.trim();

  useEffect(() => {
    const controller = new AbortController();
    request("/members/me", { signal: controller.signal })
      .then(setMember)
      .catch((e) => {
        if (!controller.signal.aborted) {
          if (e.status === 401) setMember(null);
          else setAccountError(e.message);
        }
      });
    return () => controller.abort();
  }, [tab]);

  useEffect(() => {
    setHistory([]);
    setSelected("");
    setLookup("");
    setAccountId("");
    setAccounts([]);
  }, [member?.id]);

  useEffect(() => {
    if (!member) return;
    const controller = new AbortController();
    let timer;
    async function pollAccounts() {
      try {
        const values = await request("/accounts", {
          signal: controller.signal,
        });
        if (controller.signal.aborted) return;
        setAccounts(values.filter((a) => a.status === "OPEN"));
        setAccountError("");
      } catch (e) {
        if (!controller.signal.aborted) {
          setAccountError(e.message);
          if (e.status === 401) setMember(null);
        }
      } finally {
        if (!controller.signal.aborted) timer = setTimeout(pollAccounts, 3000);
      }
    }
    pollAccounts();
    return () => {
      controller.abort();
      clearTimeout(timer);
    };
  }, [member?.id, tab]);

  useEffect(() => {
    const controller = new AbortController();
    let timer;
    setStock(null);
    setStockError("");
    async function poll() {
      try {
        const value = await request(
          `/inventory/${encodeURIComponent(selectedProduct)}`,
          { signal: controller.signal, allowMissing: true },
        );
        if (controller.signal.aborted) return;
        setStock(value);
        setStockError(value ? "" : "등록되지 않은 상품입니다.");
      } catch (e) {
        if (!controller.signal.aborted) {
          setStock(null);
          setStockError(e.message);
        }
      } finally {
        if (!controller.signal.aborted) timer = setTimeout(poll, 3000);
      }
    }
    if (selectedProduct) poll();
    return () => {
      controller.abort();
      clearTimeout(timer);
    };
  }, [selectedProduct, refresh]);

  useEffect(() => {
    if (order)
      setHistory((items) =>
        [
          order,
          ...items.filter((item) => item.orderId !== order.orderId),
        ].slice(0, 20),
      );
  }, [order]);

  // ref로 재렌더 전에 들어온 중복 클릭도 막는다. 이는 브라우저 한 인스턴스의 보호이며 API 멱등 키를 대신하지 않는다.
  async function createOrder(event) {
    event.preventDefault();
    if (submittingRef.current) return;
    const body = {
      ...form,
      accountId,
      productId: form.productId.trim(),
      quantity: Number(form.quantity),
      amount: Number(form.amount),
    };
    if (!member || !accountId) {
      setSubmitError("로그인 후 결제 계좌를 선택해 주세요.");
      return;
    }
    if (
      !body.productId ||
      !Number.isInteger(body.quantity) ||
      body.quantity < 1 ||
      body.quantity > 2147483647 ||
      !Number.isSafeInteger(body.amount) ||
      body.amount < 1 ||
      body.amount > 1000000000
    ) {
      setSubmitError("상품 ID와 1 이상의 정수 수량·금액을 입력해 주세요.");
      return;
    }
    submittingRef.current = true;
    setSubmitting(true);
    setSubmitError("");
    try {
      const created = await request("/orders", { body });
      setHistory((items) => [{ ...body, ...created }, ...items].slice(0, 20));
      setSelected(created.orderId);
      setLookup(created.orderId);
    } catch (e) {
      setSubmitError(
        `${e.message} 응답을 받지 못해도 주문이 접수되었을 수 있습니다. 재전송하면 별도 주문이 생성됩니다.`,
      );
    } finally {
      submittingRef.current = false;
      setSubmitting(false);
    }
  }
  function preset(kind) {
    setForm({
      productId: "book",
      quantity: kind === "refund" ? 999 : 1,
      amount: kind === "refund" ? 9990000 : 10000,
      rejectPayment: kind === "reject",
    });
  }
  const activeStep = !order
    ? -1
    : order.status === "PAYMENT_PENDING"
      ? 0
      : order.status === "STOCK_PENDING"
        ? 1
        : 2;

  return (
    <div className="shell">
      <header className="topbar">
        <a className="brand" href="/" aria-label="Saga Lab 홈">
          <span className="brand-icon">S</span> SAGA
          <span className="brand-light">LAB</span>
        </a>
        <span className="environment">
          <i /> LOCAL WORKSPACE
        </span>
      </header>
      <nav className="main-nav" aria-label="주요 메뉴">
        <button
          className={tab === "orders" ? "current" : ""}
          onClick={() => setTab("orders")}
        >
          주문 실험실
        </button>
        <button
          className={tab === "members" ? "current" : ""}
          onClick={() => setTab("members")}
        >
          회원 · 계좌 관리
        </button>
      </nav>
      {tab === "members" && (
        <main>
          <MemberArea onMemberChange={setMember} />
        </main>
      )}
      <main hidden={tab !== "orders"}>
        <section className="intro">
          <div>
            <p className="eyebrow">DISTRIBUTED SYSTEMS / HANDS-ON LAB</p>
            <h1>
              주문 하나로 보는
              <br />
              서비스의 연결.
            </h1>
            <p className="lead">
              주문, 결제, 재고가 이어지는 순간을 확인하세요.
              <br />
              성공부터 보상 트랜잭션까지 직접 실험할 수 있습니다.
            </p>
          </div>
          <div className="flow-preview">
            <span>
              01 <b>주문</b>
            </span>
            <em>→</em>
            <span>
              02 <b>결제</b>
            </span>
            <em>→</em>
            <span>
              03 <b>재고</b>
            </span>
            <small>Kafka로 연결되는 비동기 주문 흐름</small>
          </div>
        </section>
        <div className="workspace">
          <section className="panel create-panel">
            <div className="section-heading">
              <span className="eyebrow">01 / CREATE</span>
              <h2>새 주문 만들기</h2>
              <p>시나리오를 선택하거나 값을 직접 입력하세요.</p>
            </div>
            <div className="presets">
              <button
                type="button"
                disabled={submitting}
                onClick={() => preset("success")}
              >
                정상 주문
              </button>
              <button
                type="button"
                disabled={submitting}
                onClick={() => preset("reject")}
              >
                결제 거절
              </button>
              <button
                type="button"
                disabled={submitting}
                onClick={() => preset("refund")}
              >
                재고 부족
              </button>
            </div>
            <form onSubmit={createOrder}>
              <fieldset disabled={submitting}>
                {!member ? (
                  <p className="status-message">
                    회원가입·로그인 후 입금한 계좌로 주문할 수 있습니다.{" "}
                    <button
                      type="button"
                      className="text-button"
                      onClick={() => setTab("members")}
                    >
                      회원 · 계좌 관리로 이동
                    </button>
                  </p>
                ) : (
                  <label>
                    결제 계좌
                    <select
                      className="order-account-select"
                      aria-label="결제 계좌"
                      value={accountId}
                      required
                      onChange={(e) => setAccountId(e.target.value)}
                    >
                      <option value="">계좌를 선택하세요</option>
                      {accounts.map((a) => (
                        <option key={a.id} value={a.id}>
                          {a.name} · {won(a.balance)}
                        </option>
                      ))}
                    </select>
                  </label>
                )}
                {accountError && (
                  <p className="error" role="alert">
                    {accountError}
                  </p>
                )}
                <label>
                  상품 ID
                  <input
                    required
                    maxLength={100}
                    value={form.productId}
                    onChange={(e) =>
                      setForm({ ...form, productId: e.target.value })
                    }
                    placeholder="book"
                  />
                </label>
                <div className="form-row">
                  <label>
                    수량
                    <input
                      required
                      type="number"
                      min="1"
                      max="2147483647"
                      step="1"
                      value={form.quantity}
                      onChange={(e) =>
                        setForm({ ...form, quantity: e.target.value })
                      }
                    />
                  </label>
                  <label>
                    총 결제 금액 (원)
                    <input
                      required
                      type="number"
                      min="1"
                      max="1000000000"
                      step="1"
                      value={form.amount}
                      onChange={(e) =>
                        setForm({ ...form, amount: e.target.value })
                      }
                    />
                  </label>
                </div>
                <label className="check">
                  <input
                    type="checkbox"
                    checked={form.rejectPayment}
                    onChange={(e) =>
                      setForm({ ...form, rejectPayment: e.target.checked })
                    }
                  />
                  <span>
                    결제 거절 시뮬레이션
                    <small>결제 실패 후 주문 취소를 확인합니다.</small>
                  </span>
                </label>
                {submitError && (
                  <p role="alert" className="error">
                    {submitError}
                  </p>
                )}
                <button
                  className="primary"
                  type="submit"
                  disabled={!member || !accountId}
                >
                  {submitting ? "주문 접수 중…" : "주문 생성하기"}
                  <span>↗</span>
                </button>
              </fieldset>
            </form>
            <p className="footnote">
              실제 결제가 발생하지 않는 학습용 환경입니다.
              <br />
              접수 후 최종 처리 결과를 오른쪽에서 확인하세요.
            </p>
          </section>
          <section className="panel detail-panel">
            <div className="detail-heading">
              <div>
                <span className="eyebrow">02 / OBSERVE</span>
                <h2>주문 흐름 살펴보기</h2>
              </div>
              <button
                className="text-button"
                onClick={() => setRefresh((n) => n + 1)}
              >
                ↻ 새로고침
              </button>
            </div>
            <form
              className="lookup"
              onSubmit={(e) => {
                e.preventDefault();
                if (lookup.trim()) {
                  setSelected(lookup.trim());
                  setRefresh((n) => n + 1);
                }
              }}
            >
              <input
                aria-label="조회할 주문 ID"
                placeholder="기존 주문 ID를 입력해 조회"
                required
                value={lookup}
                onChange={(e) => setLookup(e.target.value)}
              />
              <button type="submit">조회</button>
            </form>
            {error && (
              <p className="error" role="alert">
                {error} 자동으로 다시 조회합니다.
              </p>
            )}
            {!order ? (
              <div className="empty">
                <span className="empty-symbol">◎</span>
                <h3>
                  {selected
                    ? "주문을 조회하고 있습니다"
                    : "첫 번째 주문을 기다리고 있어요"}
                </h3>
                <p>
                  주문을 생성하거나 기존 주문 ID를 입력하면
                  <br />
                  처리 상태와 결제 결과가 여기에 표시됩니다.
                </p>
              </div>
            ) : (
              <div className="order-content">
                <div className="order-title">
                  <div>
                    <small>ORDER ID</small>
                    <code>{order.orderId}</code>
                  </div>
                  <Badge status={order.status} />
                </div>
                <ol className="steps">
                  {[
                    "결제 처리",
                    "재고 예약",
                    order.status === "COMPENSATING" ||
                    order.reason === "OUT_OF_STOCK"
                      ? "환불 · 취소"
                      : order.status === "CANCELLED"
                        ? "주문 취소"
                        : "주문 완료",
                  ].map((step, i) => (
                    <li
                      key={i}
                      className={`${i === activeStep ? "active" : ""} ${order.reason === "PAYMENT_REJECTED" && i === 1 ? "skipped" : ""}`}
                    >
                      <span>{String(i + 1).padStart(2, "0")}</span>
                      {step}
                    </li>
                  ))}
                </ol>
                <p className="status-message" role="status">
                  {order.status === "COMPENSATING"
                    ? "재고 부족으로 환불을 진행 중입니다. 환불 확인 후 주문이 취소됩니다."
                    : finished(order.status)
                      ? order.status === "COMPLETED"
                        ? "결제와 재고 예약이 모두 완료되었습니다."
                        : reasons[order.reason] || "주문이 취소되었습니다."
                      : "서비스가 비동기로 처리 중입니다. 결과를 자동으로 확인합니다."}
                </p>
                <dl className="facts">
                  <div>
                    <dt>상품</dt>
                    <dd>{order.productId}</dd>
                  </div>
                  <div>
                    <dt>주문 수량</dt>
                    <dd>{order.quantity}개</dd>
                  </div>
                  <div>
                    <dt>총 결제 금액</dt>
                    <dd>{won(order.amount)}</dd>
                  </div>
                </dl>
                <div className="payment">
                  <span>결제 서비스</span>
                  {payment ? (
                    <Badge status={payment.status} />
                  ) : (
                    <span>
                      {paymentError ? "연결 확인 필요" : "결제 내역 대기 중"}
                    </span>
                  )}
                </div>
                {paymentError && (
                  <p className="error" role="alert">
                    {paymentError}
                  </p>
                )}
                <p className="footnote">
                  마지막 주문 조회 {updated?.toLocaleTimeString("ko-KR")} ·{" "}
                  {finished(order.status)
                    ? "최종 주문 상태"
                    : "1.5초 간격 자동 조회"}
                  <br />
                  조회 사이에 빠르게 지나간 중간 상태는 표시되지 않을 수
                  있습니다.
                </p>
              </div>
            )}
            <div className="stock">
              <div>
                <span className="eyebrow">INVENTORY</span>
                <h3>
                  {selectedProduct || "상품 미선택"} <span>현재 재고</span>
                </h3>
              </div>
              <strong>
                {stock ? (
                  <>
                    {stock.available}
                    <small>개</small>
                  </>
                ) : (
                  "—"
                )}
              </strong>
            </div>
            {stockError && (
              <p className="error" role="alert">
                {stockError}
              </p>
            )}
          </section>
        </div>
        <section className="panel history-panel">
          <div className="detail-heading">
            <div>
              <span className="eyebrow">03 / HISTORY</span>
              <h2>최근 확인한 주문</h2>
            </div>
            <span className="history-note">
              현재 로그인 세션에서 최대 20건 · 선택 시 최신 상태 조회
            </span>
          </div>
          {history.length === 0 ? (
            <p className="history-empty">
              아직 확인한 주문이 없습니다. 새 주문을 만들어 흐름을 시작해
              보세요.
            </p>
          ) : (
            <div className="table-wrap">
              <table>
                <thead>
                  <tr>
                    <th>주문 ID</th>
                    <th>상품 / 수량</th>
                    <th>결제 금액</th>
                    <th>마지막 확인 상태</th>
                  </tr>
                </thead>
                <tbody>
                  {history.map((item) => (
                    <tr
                      key={item.orderId}
                      className={selected === item.orderId ? "selected" : ""}
                    >
                      <td>
                        <button
                          className="order-link"
                          onClick={() => {
                            setSelected(item.orderId);
                            setLookup(item.orderId);
                            setRefresh((n) => n + 1);
                          }}
                        >
                          {item.orderId}
                        </button>
                      </td>
                      <td>
                        {item.productId}{" "}
                        <span className="muted">/ {item.quantity}개</span>
                      </td>
                      <td>{won(item.amount)}</td>
                      <td>
                        <Badge status={item.status} />
                      </td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          )}
        </section>
        <footer>
          <span>SAGA LAB</span>
          <span>Spring Boot + Kafka + React · Orchestration Saga</span>
        </footer>
      </main>
    </div>
  );
}
