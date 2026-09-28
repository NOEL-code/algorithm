import React, { useEffect, useRef, useState } from 'react';
import { request } from './api';

const won = (value) => new Intl.NumberFormat('ko-KR').format(value) + '원';

export default function MemberArea() {
  const [member, setMember] = useState(null);
  const [checking, setChecking] = useState(true);
  const [mode, setMode] = useState('login');
  const [error, setError] = useState('');
  const [notice, setNotice] = useState('');
  const [busy, setBusy] = useState(false);
  const lock = useRef(false);
  const [accounts, setAccounts] = useState([]);
  const [accountId, setAccountId] = useState('');
  const [entries, setEntries] = useState([]);
  const [historyError, setHistoryError] = useState('');
  const [historyLoading, setHistoryLoading] = useState(false);
  const [revision, setRevision] = useState(0);
  // Retain the request ID after a network failure so retrying the same transaction is safe.
  const pending = useRef(null);
  const account = accounts.find((item) => item.id === accountId);

  useEffect(() => {
    const controller = new AbortController();
    request('/members/me', { signal: controller.signal }).then(setMember).catch((e) => {
      if (!controller.signal.aborted && e.status !== 401) setError(e.message);
    }).finally(() => { if (!controller.signal.aborted) setChecking(false); });
    return () => controller.abort();
  }, []);

  useEffect(() => {
    setAccounts([]); setAccountId(''); setEntries([]); pending.current = null;
    if (!member?.id) return;
    const controller = new AbortController();
    request('/accounts', { signal: controller.signal }).then(setAccounts).catch((e) => {
      if (!controller.signal.aborted) { setError(e.message); if (e.status === 401) setMember(null); }
    });
    return () => controller.abort();
  }, [member?.id]);

  useEffect(() => {
    setEntries([]); setHistoryError(''); setHistoryLoading(Boolean(accountId));
    if (!accountId) return;
    const controller = new AbortController();
    request(`/accounts/${encodeURIComponent(accountId)}/transactions`, { signal: controller.signal }).then(setEntries).catch((e) => {
      if (!controller.signal.aborted) { setHistoryError(e.message); if (e.status === 401) setMember(null); }
    }).finally(() => { if (!controller.signal.aborted) setHistoryLoading(false); });
    return () => controller.abort();
  }, [accountId, revision]);

  async function run(action) {
    if (lock.current) return;
    lock.current = true; setBusy(true); setError(''); setNotice('');
    try { await action(); }
    catch (e) { setError(e.message); if (e.status === 401 && member) setMember(null); }
    finally { lock.current = false; setBusy(false); }
  }
  async function reload() { setAccounts(await request('/accounts')); setRevision((n) => n + 1); }
  function authenticate(event) {
    event.preventDefault();
    const form = new FormData(event.currentTarget);
    run(async () => {
      const result = await request(mode === 'register' ? '/members' : '/members/sessions', {
        body: { email: form.get('email').trim(), password: form.get('password'), ...(mode === 'register' ? { name: form.get('name').trim() } : {}) },
      });
      setMember(result); setNotice(mode === 'register' ? '가입이 완료되었습니다. 첫 계좌를 만들어 보세요.' : '로그인했습니다.');
    });
  }
  function transact(event) {
    event.preventDefault();
    const form = new FormData(event.currentTarget);
    const type = form.get('type'); const amount = Number(form.get('amount'));
    const fingerprint = `${accountId}:${type}:${amount}`;
    if (pending.current?.fingerprint !== fingerprint) pending.current = { fingerprint, requestId: crypto.randomUUID() };
    const requestId = pending.current.requestId;
    run(async () => {
      await request(`/accounts/${accountId}/transactions`, { body: { type, amount, requestId } });
      pending.current = null;
      setNotice(`${type === 'DEPOSIT' ? '입금' : '출금'} ${won(amount)} 처리가 완료되었습니다.`);
      await reload();
    });
  }

  return <>
    <section className="intro"><div><p className="eyebrow">MEMBERS & ACCOUNTS</p><h1>나의 정보와 계좌를<br />한 곳에서.</h1><p className="lead">회원별로 분리된 계좌와 거래 내역을 관리하세요.<br />입출금은 실제 금융 거래가 아닌 학습용 잔액 처리입니다.</p></div>{member && <div className="member-summary"><span className="avatar">{member.name.slice(0, 1)}</span><h2>{member.name}님</h2><p>{member.email}</p><button disabled={busy} className="text-button" onClick={() => run(async () => { await request('/members/sessions/current', { method: 'DELETE' }); setMember(null); setNotice('로그아웃했습니다.'); })}>로그아웃</button></div>}</section>
    {error && <p className="error" role="alert">{error}</p>}{notice && <p className="status-message" role="status">{notice}</p>}
    {checking ? <p className="status-message">로그인 상태를 확인하고 있습니다…</p> : !member ? <section className="panel auth-panel">
      <div className="presets"><button aria-pressed={mode === 'login'} disabled={busy} onClick={() => { setMode('login'); setError(''); }}>로그인</button><button aria-pressed={mode === 'register'} disabled={busy} onClick={() => { setMode('register'); setError(''); }}>회원가입</button></div>
      <h2>{mode === 'register' ? '새 회원 등록' : '다시 만나 반가워요'}</h2>
      <form onSubmit={authenticate} key={mode}><fieldset disabled={busy}>
        {mode === 'register' && <label>이름<input name="name" required maxLength={60} autoComplete="name" /></label>}
        <label>이메일<input name="email" type="email" required maxLength={254} autoComplete="username" /></label>
        <label>비밀번호<input name="password" type="password" required minLength={mode === 'register' ? 10 : 1} maxLength={128} autoComplete={mode === 'register' ? 'new-password' : 'current-password'} /></label>
        {mode === 'register' && <p className="footnote">비밀번호는 10~128자로 입력하세요.</p>}
        <button className="primary" type="submit">{busy ? '처리 중…' : mode === 'register' ? '가입하고 시작하기' : '로그인하기'}<span>↗</span></button>
      </fieldset></form>
    </section> : <div className="member-workspace">
      <aside className="member-sidebar"><section className="panel"><span className="eyebrow">MY PROFILE</span><h2>회원 정보 관리</h2>
        <form key={member.id + member.name} onSubmit={(e) => { e.preventDefault(); const name = new FormData(e.currentTarget).get('name').trim(); run(async () => { setMember(await request('/members/me', { method: 'PATCH', body: { name } })); setNotice('회원 정보를 저장했습니다.'); }); }}><fieldset disabled={busy}><label>이름<input name="name" defaultValue={member.name} required maxLength={60} /></label><label>가입 이메일<input value={member.email} readOnly /></label><button className="secondary" type="submit">정보 저장</button></fieldset></form>
        <details className="password-details"><summary>비밀번호 변경</summary><form onSubmit={(e) => { e.preventDefault(); const form = new FormData(e.currentTarget); run(async () => { await request('/members/me/password', { method: 'PUT', body: { currentPassword: form.get('currentPassword'), newPassword: form.get('newPassword') } }); setMember(null); setMode('login'); setNotice('비밀번호가 변경되어 모든 세션이 종료되었습니다. 다시 로그인해 주세요.'); }); }}><fieldset disabled={busy}><label>현재 비밀번호<input name="currentPassword" type="password" required maxLength={128} autoComplete="current-password" /></label><label>새 비밀번호<input name="newPassword" type="password" required minLength={10} maxLength={128} autoComplete="new-password" /></label><button className="secondary">비밀번호 변경하기</button></fieldset></form></details>
      </section><section className="panel"><span className="eyebrow">NEW ACCOUNT</span><h2>계좌 개설</h2><form onSubmit={(e) => { e.preventDefault(); const element = e.currentTarget; const name = new FormData(element).get('name').trim(); run(async () => { const created = await request('/accounts', { body: { name } }); setAccountId(created.id); element.reset(); setNotice('새 계좌가 개설되었습니다.'); await reload(); }); }}><fieldset disabled={busy}><label>계좌 별칭<input name="name" required maxLength={60} placeholder="예: 생활비 계좌" /></label><button className="primary">계좌 개설하기<span>＋</span></button></fieldset></form><p className="footnote">잔액 0원으로 개설됩니다. 입금 후 출금해 보세요.</p></section></aside>
      <section className="panel accounts-panel"><div className="detail-heading"><div><span className="eyebrow">MY ACCOUNTS</span><h2>내 계좌 <span className="muted">{accounts.length}</span></h2></div><button className="text-button" disabled={busy} onClick={() => run(reload)}>↻ 계좌 새로고침</button></div>
        {accounts.length === 0 ? <div className="empty"><span className="empty-symbol">＋</span><h3>아직 개설된 계좌가 없습니다</h3><p>왼쪽에서 첫 계좌를 만들어 보세요.</p></div> : <div className="account-list">{accounts.map((item) => <button key={item.id} disabled={busy} className={`account-card ${item.id === accountId ? 'chosen' : ''}`} onClick={() => setAccountId(item.id)}><span><b>{item.name}</b><small>{item.id}</small></span><span><strong>{won(item.balance)}</strong><small>{item.status === 'OPEN' ? '사용 중' : '해지됨'}</small></span></button>)}</div>}
        {account && <section className="account-detail"><div className="detail-heading"><h3>{account.name} 관리</h3><span className="badge">{account.status === 'OPEN' ? '사용 중' : '해지된 계좌'}</span></div>
          {account.status === 'OPEN' && <>
            <form className="rename-form" key={account.id + account.name} onSubmit={(e) => { e.preventDefault(); const name = new FormData(e.currentTarget).get('name').trim(); run(async () => { await request(`/accounts/${accountId}`, { method: 'PATCH', body: { name } }); await reload(); setNotice('계좌 별칭을 변경했습니다.'); }); }}><label>계좌 별칭 변경<input name="name" defaultValue={account.name} required maxLength={60} disabled={busy} /></label><button className="secondary" disabled={busy}>변경</button></form>
            <form onSubmit={transact}><fieldset disabled={busy}><div className="form-row"><label>거래 구분<select name="type"><option value="DEPOSIT">입금</option><option value="WITHDRAW">출금</option></select></label><label>거래 금액 (원)<input name="amount" type="number" min="1" max="1000000000" step="1" required defaultValue="10000" /></label></div><button className="primary">{busy ? '처리 중…' : '거래 실행'}<span>↗</span></button></fieldset></form>
            <p className="footnote">한 번에 최대 10억 원까지 실습할 수 있습니다. 오류 후 같은 거래를 다시 실행하면 동일 요청 ID로 재시도합니다.</p>
            <button className="danger-link" disabled={busy || account.balance !== 0} onClick={() => { if (window.confirm('잔액 0원인 이 계좌를 해지할까요? 거래 내역은 보존되며 다시 사용할 수 없습니다.')) run(async () => { await request(`/accounts/${accountId}`, { method: 'DELETE' }); await reload(); setNotice('계좌를 해지했습니다.'); }); }}>계좌 해지 (잔액 0원일 때 가능)</button>
          </>}
          <h3 className="ledger-title">최근 거래 내역</h3>{historyError && <p className="error" role="alert">{historyError}</p>}{historyLoading ? <p className="footnote">거래 내역 조회 중…</p> : entries.length === 0 ? <p className="footnote">아직 거래 내역이 없습니다.</p> : <div className="table-wrap"><table><thead><tr><th>일시</th><th>구분</th><th>금액</th><th>거래 후 잔액</th></tr></thead><tbody>{entries.map((entry) => <tr key={entry.id}><td>{entry.createdAt}</td><td>{entry.type === 'DEPOSIT' ? '입금' : '출금'}</td><td>{won(entry.amount)}</td><td>{won(entry.balanceAfter)}</td></tr>)}</tbody></table><p className="footnote">최신 100건 표시 · 일시는 서버 기준</p></div>}
        </section>}
      </section>
    </div>}
    <footer><span>SAGA LAB</span><span>회원 · 계좌 관리 / 학습용 가상 계좌</span></footer>
  </>;
}
