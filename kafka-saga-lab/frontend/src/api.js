export async function request(path, { signal, body, method, allowMissing = false } = {}) {
  const timeout = AbortSignal.timeout(8000);
  let response;
  try {
    response = await fetch(`/api${path}`, {
      signal: signal ? AbortSignal.any([signal, timeout]) : timeout,
      credentials: 'same-origin',
      cache: 'no-store',
      method: method || (body ? 'POST' : 'GET'),
      ...(body ? { headers: { 'Content-Type': 'application/json' }, body: JSON.stringify(body) } : {}),
    });
  } catch (error) {
    if (signal?.aborted) throw error;
    throw new Error('서버에 연결할 수 없습니다. 백엔드 실행 상태를 확인해 주세요.');
  }
  if (allowMissing && response.status === 404) return null;
  if (!response.ok) {
    const payload = await response.json().catch(() => ({}));
    const error = new Error(payload.message || (response.status === 404
      ? '조회한 항목이 없습니다. ID를 확인해 주세요.'
      : `요청에 실패했습니다 (HTTP ${response.status}). 잠시 후 다시 확인해 주세요.`));
    error.status = response.status;
    throw error;
  }
  return response.status === 204 ? null : response.json();
}

export const finished = (status) => ['COMPLETED', 'CANCELLED'].includes(status);
