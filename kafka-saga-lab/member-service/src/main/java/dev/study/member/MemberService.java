/**
 * 학습: 영속 세션과 자격 증명 수명 회원 생성과 세션 저장을 같은 트랜잭션으로 묶어 가입만 성공한 반쪽 상태를 줄인다. 정규화한 이메일의 UNIQUE가 동시 가입을 최종
 * 방어한다. 사전 SELECT만으로는 부족하다. JWT 대신 서버 세션을 선택해 즉시 폐기를 관찰한다. 그 대가로 매 인증 시 DB/회원 서비스에 의존한다.
 */
package dev.study.member;

import org.springframework.dao.DuplicateKeyException;
import org.springframework.http.HttpStatus;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.server.ResponseStatusException;

import java.sql.Timestamp;
import java.time.Instant;
import java.time.temporal.ChronoUnit;
import java.util.*;

@Service
public class MemberService {
    public record Member(String id, String email, String name) {}

    public record Login(Member member, String token) {}

    private final JdbcTemplate db;
    // 없는 이메일도 KDF 계산을 수행해 빠른 실패 시간으로 계정 존재를 추정하는 경로를 줄인다.
    // 완전한 타이밍 동일성/계정 열거 방지는 아니다. 가입 중복 응답과 rate limit도 별도 정책이다.
    private final String dummyHash = Passwords.hash(Passwords.token());

    public MemberService(JdbcTemplate db) {
        this.db = db;
    }

    @Transactional
    public Login register(String email, String name, String password) {
        var member = new Member(UUID.randomUUID().toString(), normalize(email), name.strip());
        // 이메일을 먼저 SELECT해도 두 요청이 동시에 없음을 볼 수 있다. UNIQUE 위반을 409로 번역한다.
        try {
            db.update(
                    "insert into members(id,email,name,password_hash) values(?,?,?,?)",
                    member.id(),
                    member.email(),
                    member.name(),
                    Passwords.hash(password));
        } catch (DuplicateKeyException e) {
            throw new ResponseStatusException(HttpStatus.CONFLICT, "이미 가입된 이메일입니다.");
        }
        return session(member);
    }

    @Transactional
    public Login login(String email, String password) {
        // 비밀번호 변경도 같은 회원 행을 잠근다. 검증 후 세션 생성 사이에 변경이 끼어들면
        // 옛 비밀번호로 만든 세션이 전체 세션 폐기 뒤 살아남을 수 있으므로 함께 직렬화한다.
        // 비용: 느린 KDF 동안 행 잠금을 보유한다. 운영에서는 자격 증명 버전 CAS도 비교할 수 있다.
        var rows =
                db.queryForList("select * from members where email=? for update", normalize(email));
        String hash = rows.isEmpty() ? dummyHash : (String) rows.getFirst().get("PASSWORD_HASH");
        if (!Passwords.matches(password, hash) || rows.isEmpty())
            throw new ResponseStatusException(HttpStatus.UNAUTHORIZED, "이메일 또는 비밀번호를 확인해 주세요.");
        var row = rows.getFirst();
        return session(
                new Member(
                        (String) row.get("ID"),
                        (String) row.get("EMAIL"),
                        (String) row.get("NAME")));
    }

    // 인증 시 만료 조건을 검사하므로 청소 작업이 늦어도 만료 토큰은 사용할 수 없다.
    public Member current(String token) {
        if (token == null || token.length() != 43) throw unauthorized();
        var rows =
                db.query(
                        "select m.id,m.email,m.name from members m join member_sessions s on"
                            + " s.member_id=m.id where s.token_hash=? and s.expires_at >"
                            + " current_timestamp",
                        (rs, n) ->
                                new Member(
                                        rs.getString("id"),
                                        rs.getString("email"),
                                        rs.getString("name")),
                        Passwords.digest(token));
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
        if (token != null)
            db.update("delete from member_sessions where token_hash=?", Passwords.digest(token));
    }

    @Transactional
    public void changePassword(String token, String currentPassword, String newPassword) {
        Member member = current(token);
        String hash =
                db.queryForObject(
                        "select password_hash from members where id=? for update",
                        String.class,
                        member.id());
        if (!Passwords.matches(currentPassword, hash))
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "현재 비밀번호가 일치하지 않습니다.");
        db.update(
                "update members set password_hash=? where id=?",
                Passwords.hash(newPassword),
                member.id());
        // 비밀번호 갱신과 세션 폐기는 같은 트랜잭션이다. 중간 실패 시 둘 다 rollback한다.
        db.update("delete from member_sessions where member_id=?", member.id());
    }

    // 로그인 시 만료 행 청소는 작은 실습용 정책이다. 대량 세션이면 만료 인덱스·주기 배치로 분리한다.
    private Login session(Member member) {
        db.update("delete from member_sessions where expires_at <= current_timestamp");
        String token = Passwords.token();
        db.update(
                "insert into member_sessions(token_hash,member_id,expires_at) values(?,?,?)",
                Passwords.digest(token),
                member.id(),
                Timestamp.from(Instant.now().plus(1, ChronoUnit.DAYS)));
        return new Login(member, token);
    }

    private static String normalize(String email) {
        return email.strip().toLowerCase(Locale.ROOT);
    }

    private static ResponseStatusException unauthorized() {
        return new ResponseStatusException(HttpStatus.UNAUTHORIZED, "로그인이 필요합니다.");
    }
}
