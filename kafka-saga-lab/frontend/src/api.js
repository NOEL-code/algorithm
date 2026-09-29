/* 학습: HTTP 공통 경계와 실패 의미
 * 쿠키는 브라우저가 전송한다. HttpOnly 토큰을 JavaScript로 읽거나 localStorage에 보관하지 않는다.
 * 요청 취소와 8초 타임아웃은 서버 작업 rollback이 아니다. 쓰기 요청을 자동 재시도하지 않는다.
 * 204는 본문 없음, 선택적 404는 아직 없음이다. 그 밖의 실패를 정상 데이터로 숨기지 않는다.
 */
export async function request(
  path,
  { signal, body, method, allowMissing = false } = {},
) {
  const timeout = AbortSignal.timeout(8000);
  let response;
  try {
    response = await fetch(`/api${path}`, {
      signal: signal ? AbortSignal.any([signal, timeout]) : timeout,
      credentials: "same-origin",
      cache: "no-store",
      method: method || (body ? "POST" : "GET"),
      ...(body
        ? {
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(body),
          }
        : {}),
    });
  } catch (error) {
    if (signal?.aborted) throw error;
    throw new Error(
      "서버에 연결할 수 없습니다. 백엔드 실행 상태를 확인해 주세요.",
    );
  }
  if (allowMissing && response.status === 404) return null;
  if (!response.ok) {
    const payload = await response.json().catch(() => ({}));
    const error = new Error(
      payload.message ||
        (response.status === 404
          ? "조회한 항목이 없습니다. ID를 확인해 주세요."
          : `요청에 실패했습니다 (HTTP ${response.status}). 잠시 후 다시 확인해 주세요.`),
    );
    error.status = response.status;
    throw error;
  }
  return response.status === 204 ? null : response.json();
}

export const finished = (status) => ["COMPLETED", "CANCELLED"].includes(status);
