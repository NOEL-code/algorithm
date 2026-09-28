package dev.study.account;

import java.util.*;
import org.springframework.http.HttpStatus;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.server.ResponseStatusException;

@Service
public class AccountService {
    public record Account(String id, String name, long balance, String status) {}
    public record Entry(String id, String requestId, String type, long amount, long balanceAfter, String createdAt) {}
    private final JdbcTemplate db;
    public AccountService(JdbcTemplate db) { this.db = db; }
    public List<Account> list(String member) {
        return db.query("select * from accounts where member_id=? order by created_at desc,id", (rs,n) -> new Account(rs.getString("id"),rs.getString("name"),rs.getLong("balance"),rs.getString("status")), member);
    }
    @Transactional
    public Account create(String member, String name) {
        String id = UUID.randomUUID().toString();
        db.update("insert into accounts(id,member_id,name) values(?,?,?)",id,member,name.strip());
        return new Account(id,name.strip(),0,"OPEN");
    }
    @Transactional
    public Account rename(String member, String id, String name) {
        Account account = owned(member,id,true);
        ensureOpen(account);
        db.update("update accounts set name=? where id=?",name.strip(),id);
        return new Account(id,name.strip(),account.balance(),account.status());
    }
    @Transactional
    public void close(String member, String id) {
        Account account = owned(member,id,true);
        if (account.balance()!=0) throw new ResponseStatusException(HttpStatus.CONFLICT,"잔액이 0원인 계좌만 해지할 수 있습니다.");
        db.update("update accounts set status='CLOSED' where id=?",id);
    }
    @Transactional
    public Entry transact(String member, String id, String type, long amount, String requestId) {
        Account account = owned(member,id,true);
        var existing = db.query("select * from account_transactions where account_id=? and request_id=?", (rs,n) -> new Entry(rs.getString("id"),rs.getString("request_id"),rs.getString("type"),rs.getLong("amount"),rs.getLong("balance_after"),rs.getString("created_at")),id,requestId);
        if (!existing.isEmpty()) {
            Entry entry = existing.getFirst();
            if (!entry.type().equals(type) || entry.amount()!=amount) throw new ResponseStatusException(HttpStatus.CONFLICT,"같은 요청 ID에 다른 거래를 사용할 수 없습니다.");
            return entry;
        }
        ensureOpen(account);
        long next = account.balance() + (type.equals("DEPOSIT") ? amount : -amount);
        if (next < 0) throw new ResponseStatusException(HttpStatus.CONFLICT,"계좌 잔액이 부족합니다.");
        if (next > 9_000_000_000_000L) throw new ResponseStatusException(HttpStatus.CONFLICT,"계좌 잔액 한도를 초과했습니다.");
        db.update("update accounts set balance=? where id=?",next,id);
        String entryId = UUID.randomUUID().toString();
        db.update("insert into account_transactions(id,account_id,request_id,type,amount,balance_after) values(?,?,?,?,?,?)",entryId,id,requestId,type,amount,next);
        return db.queryForObject("select * from account_transactions where id=?", (rs,n) -> new Entry(entryId,requestId,type,amount,next,rs.getString("created_at")),entryId);
    }
    public List<Entry> history(String member, String id) {
        owned(member,id,false);
        return db.query("select * from account_transactions where account_id=? order by created_at desc,id limit 100", (rs,n) -> new Entry(rs.getString("id"),rs.getString("request_id"),rs.getString("type"),rs.getLong("amount"),rs.getLong("balance_after"),rs.getString("created_at")),id);
    }
    private Account owned(String member, String id, boolean lock) {
        var rows = db.query("select * from accounts where id=? and member_id=?" + (lock ? " for update" : ""), (rs,n) -> new Account(rs.getString("id"),rs.getString("name"),rs.getLong("balance"),rs.getString("status")),id,member);
        if (rows.isEmpty()) throw new ResponseStatusException(HttpStatus.NOT_FOUND,"계좌를 찾을 수 없습니다.");
        return rows.getFirst();
    }
    private void ensureOpen(Account account) {
        if (!account.status().equals("OPEN")) throw new ResponseStatusException(HttpStatus.CONFLICT,"해지된 계좌입니다.");
    }
}
