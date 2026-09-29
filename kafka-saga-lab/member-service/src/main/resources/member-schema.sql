-- 학습: 정규화 이메일 UNIQUE는 동시 가입의 최종 방어다. 세션에는 토큰 원문 대신 해시를 저장한다.
-- 세션→회원 FK는 같은 DB 안에서만 사용한다. 만료 데이터 정리는 인증 검사와 별개다.
create table if not exists members (
    id varchar(36) primary key,
    email varchar(254) not null unique,
    name varchar(60) not null,
    password_hash varchar(255) not null,
    created_at timestamp default current_timestamp not null
);
create table if not exists member_sessions (
    token_hash varchar(64) primary key,
    member_id varchar(36) not null references members(id),
    expires_at timestamp not null
);
