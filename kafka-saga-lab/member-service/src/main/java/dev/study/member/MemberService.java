package dev.study.member;

import java.sql.Timestamp;
import java.time.Instant;
import java.time.temporal.ChronoUnit;
import java.util.*;
import org.springframework.dao.DuplicateKeyException;
import org.springframework.http.HttpStatus;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.server.ResponseStatusException;

@Service
public class MemberService {
    public record Member(String id, String email, String name) {}
    public record Login(Member member, String token) {}
    private final JdbcTemplate db;
    // A missing user still takes a password verification to avoid a fast account-enumeration path.
    private final String dummyHash = Passwords.hash(Passwords.token());
    public MemberService(JdbcTemplate db) { this.db = db; }

    @Transactional
    public Login register(String email, String name, String password) {
        var member = new Member(UUID.randomUUID().toString(), normalize(email), name.strip());
        try { db.update("insert into members(id,email,name,password_hash) values(?,?,?,?)", member.id(), member.email(), member.name(), Passwords.hash(password)); }
        catch (DuplicateKeyException e) { throw new ResponseStatusException(HttpStatus.CONFLICT, "이미 가입된 이메일입니다."); }
        return session(member);
    }
    @Transactional
    public Login login(String email, String password) {
        var rows = db.queryForList("select * from members where email=?", normalize(email));
        String hash = rows.isEmpty() ? dummyHash : (String) rows.getFirst().get("PASSWORD_HASH");
        if (!Passwords.matches(password, hash) || rows.isEmpty()) throw new ResponseStatusException(HttpStatus.UNAUTHORIZED, "이메일 또는 비밀번호를 확인해 주세요.");
        var row = rows.getFirst();
        return session(new Member((String)row.get("ID"), (String)row.get("EMAIL"), (String)row.get("NAME")));
    }
    public Member current(String token) {
        if (token == null || token.length() != 43) throw unauthorized();
        var rows = db.query("select m.id,m.email,m.name from members m join member_sessions s on s.member_id=m.id where s.token_hash=? and s.expires_at > current_timestamp",
                (rs, n) -> new Member(rs.getString("id"), rs.getString("email"), rs.getString("name")), Passwords.digest(token));
        if (rows.isEmpty()) throw unauthorized();
        return rows.getFirst();
    }
    @Transactional
    public Member update(String token, String name) {
        Member member = current(token);
        db.update("update members set name=? where id=?", name.strip(), member.id());
        return new Member(member.id(), member.email(), name.strip());
    }
    @Transactional
    public void logout(String token) {
        if (token != null) db.update("delete from member_sessions where token_hash=?", Passwords.digest(token));
    }
    @Transactional
    public void changePassword(String token, String currentPassword, String newPassword) {
        Member member = current(token);
        String hash = db.queryForObject("select password_hash from members where id=? for update", String.class, member.id());
        if (!Passwords.matches(currentPassword, hash)) throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "현재 비밀번호가 일치하지 않습니다.");
        db.update("update members set password_hash=? where id=?", Passwords.hash(newPassword), member.id());
        db.update("delete from member_sessions where member_id=?", member.id());
    }
    private Login session(Member member) {
        db.update("delete from member_sessions where expires_at <= current_timestamp");
        String token = Passwords.token();
        db.update("insert into member_sessions(token_hash,member_id,expires_at) values(?,?,?)", Passwords.digest(token), member.id(), Timestamp.from(Instant.now().plus(1, ChronoUnit.DAYS)));
        return new Login(member, token);
    }
    private static String normalize(String email) { return email.strip().toLowerCase(Locale.ROOT); }
    private static ResponseStatusException unauthorized() { return new ResponseStatusException(HttpStatus.UNAUTHORIZED, "로그인이 필요합니다."); }
}
