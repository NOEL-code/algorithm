/**
 * 학습: 같은 자원에 적용되는 일관된 잠금 규약 수동 입출금·별칭 변경·해지는 계좌 행을 잠근다. AccountOrderHandler도 같은 행을 먼저 잠근다. 잠금을 잡은 채
 * 잔액, 환불 가능 금액, 거래 원장을 검사·갱신해야 읽고 쓰는 사이에 값이 바뀌지 않는다. requestId 재사용 시 이전 결과를 반환한다. 멱등성은 락과 달리 순차적으로
 * 반복된 요청도 방어한다.
 */
package dev.study.account;

import org.springframework.http.HttpStatus;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.server.ResponseStatusException;

import java.util.*;

@Service
public class AccountService {
    public record Account(String id, String name, long balance, String status) {}

    public record Entry(
            String id,
            String requestId,
            String type,
            long amount,
            long balanceAfter,
            String createdAt) {}

    private final JdbcTemplate db;

    public AccountService(JdbcTemplate db) {
        this.db = db;
    }

    // 조회에도 member_id 조건이 포함된다. URL의 accountId를 바꿔 다른 회원의 계좌를 읽을 수 없다.
    public Account get(String member, String id) {
        return owned(member, id, false);
    }

    // 회원별 인덱스는 이 조회의 접근 경로다. 대량 데이터에서는 전체 반환 대신 커서 페이지네이션이 필요하다.
    public List<Account> list(String member) {
        return db.query(
                "select * from accounts where member_id=? order by created_at desc,id",
                (rs, n) ->
                        new Account(
                                rs.getString("id"),
                                rs.getString("name"),
                                rs.getLong("balance"),
                                rs.getString("status")),
                member);
    }

    @Transactional
    // 가입과 계좌 개설은 다른 로컬 트랜잭션이다. 회원 생성 때 다른 DB까지 원자적으로 만든다고 가정하지 않는다.
    public Account create(String member, String name) {
        String id = UUID.randomUUID().toString();
        db.update(
                "insert into accounts(id,member_id,name) values(?,?,?)", id, member, name.strip());
        return new Account(id, name.strip(), 0, "OPEN");
    }

    @Transactional
    // 별칭 수정도 해지와 같은 행에서 경합하므로 OPEN 확인과 변경을 동일 잠금 범위에 둔다.
    public Account rename(String member, String id, String name) {
        Account account = owned(member, id, true);
        ensureOpen(account);
        db.update("update accounts set name=? where id=?", name.strip(), id);
        return new Account(id, name.strip(), account.balance(), account.status());
    }

    @Transactional
    // 삭제 대신 CLOSED로 바꿔 거래 원장을 보존한다. 잔액 0이어도 진행 중인 환불 가능 주문은 남을 수 있다.
    public void close(String member, String id) {
        Account account = owned(member, id, true);
        if (account.balance() != 0)
            throw new ResponseStatusException(HttpStatus.CONFLICT, "잔액이 0원인 계좌만 해지할 수 있습니다.");
        if (pendingRefunds(id) > 0)
            throw new ResponseStatusException(HttpStatus.CONFLICT, "주문 처리가 끝난 뒤 계좌를 해지해 주세요.");
        db.update("update accounts set status='CLOSED' where id=?", id);
    }

    @Transactional
    public Entry transact(String member, String id, String type, long amount, String requestId) {
        var movement = AccountRules.manualMovement(type, amount, requestId);
        Account account = owned(member, id, true);
        // 계좌 행을 이미 잠갔으므로 같은 계좌의 요청 키 검사와 실행이 경쟁하지 않는다. UNIQUE는 추가 방어다.
        var existing =
                db.query(
                        "select * from account_transactions where account_id=? and request_id=?",
                        (rs, n) ->
                                new Entry(
                                        rs.getString("id"),
                                        rs.getString("request_id"),
                                        rs.getString("type"),
                                        rs.getLong("amount"),
                                        rs.getLong("balance_after"),
                                        rs.getString("created_at")),
                        id,
                        requestId);
        if (!existing.isEmpty()) {
            Entry entry = existing.getFirst();
            if (!entry.type().equals(type) || entry.amount() != amount)
                throw new ResponseStatusException(
                        HttpStatus.CONFLICT, "같은 요청 ID에 다른 거래를 사용할 수 없습니다.");
            return entry;
        }
        // 중복 결과는 OPEN 검사 전에 반환한다. 성공 후 해지된 계좌라도 동일 요청 재조회는 과거 결과를 받는다.
        ensureOpen(account);
        long next = account.balance() + movement.delta(amount);
        if (next < 0) throw new ResponseStatusException(HttpStatus.CONFLICT, "계좌 잔액이 부족합니다.");
        if (next + pendingRefunds(id) > AccountRules.MAX_BALANCE)
            throw new ResponseStatusException(HttpStatus.CONFLICT, "환불 예정액을 포함한 계좌 잔액 한도를 초과했습니다.");
        // 잔액 변경과 원장 INSERT 중 하나라도 실패하면 둘 다 취소된다. 외부 네트워크 호출은 이 잠금 안에서 하지 않는다.
        db.update("update accounts set balance=? where id=?", next, id);
        String entryId = UUID.randomUUID().toString();
        db.update(
                "insert into"
                    + " account_transactions(id,account_id,request_id,type,amount,balance_after)"
                    + " values(?,?,?,?,?,?)",
                entryId,
                id,
                requestId,
                type,
                amount,
                next);
        return db.queryForObject(
                "select * from account_transactions where id=?",
                (rs, n) ->
                        new Entry(
                                entryId, requestId, type, amount, next, rs.getString("created_at")),
                entryId);
    }

    // 소유권을 먼저 확인한 뒤 거래 내역을 조회한다. 시간+ID는 표시 정렬이며 DB의 엄밀한 커밋 순서가 아니다.
    public List<Entry> history(String member, String id) {
        owned(member, id, false);
        return db.query(
                "select * from account_transactions where account_id=? order by created_at desc,id"
                    + " limit 100",
                (rs, n) ->
                        new Entry(
                                rs.getString("id"),
                                rs.getString("request_id"),
                                rs.getString("type"),
                                rs.getLong("amount"),
                                rs.getLong("balance_after"),
                                rs.getString("created_at")),
                id);
    }

    // SQL 파라미터 바인딩으로 사용자 값을 코드와 분리한다. 동적 부분은 내부 boolean으로 정한 고정 SQL뿐이다.
    private Account owned(String member, String id, boolean lock) {
        var rows =
                db.query(
                        "select * from accounts where id=? and member_id=?"
                                + (lock ? " for update" : ""),
                        (rs, n) ->
                                new Account(
                                        rs.getString("id"),
                                        rs.getString("name"),
                                        rs.getLong("balance"),
                                        rs.getString("status")),
                        id,
                        member);
        if (rows.isEmpty())
            throw new ResponseStatusException(HttpStatus.NOT_FOUND, "계좌를 찾을 수 없습니다.");
        return rows.getFirst();
    }

    private void ensureOpen(Account account) {
        if (!account.status().equals("OPEN"))
            throw new ResponseStatusException(HttpStatus.CONFLICT, "해지된 계좌입니다.");
    }

    // DEBITED 합계는 아직 성공 확정/환불되지 않은 차감이다. 잔액+이 합계가 상한 이하여야 환불 공간이 남는다.
    private long pendingRefunds(String id) {
        return db.queryForObject(
                "select coalesce(sum(amount),0) from account_orders where account_id=? and"
                    + " status='DEBITED'",
                Long.class,
                id);
    }
}
