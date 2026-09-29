-- 학습: 잔액 CHECK, 요청 유일 키, 거래 원장이 애플리케이션 버그에 대한 마지막 방어선이다.
-- member_id에는 다른 서비스 DB의 FK를 걸지 않는다. 소유권은 회원 API/업무 명령으로 검증한다.
-- account_orders의 DEBITED 금액은 환불 가능 공간이므로 잔액과 함께 상한을 계산한다.
create table if not exists accounts (
    id varchar(36) primary key,
    member_id varchar(36) not null,
    name varchar(60) not null,
    balance bigint not null default 0 check (balance >= 0 and balance <= 9000000000000),
    status varchar(10) not null default 'OPEN' check (status in ('OPEN','CLOSED')),
    created_at timestamp default current_timestamp not null
);
create table if not exists account_orders (
    order_id varchar(36) primary key,
    account_id varchar(36) not null,
    member_id varchar(36) not null,
    amount bigint not null check(amount > 0),
    status varchar(12) not null
);
create index if not exists account_orders_account on account_orders(account_id,status);
create index if not exists accounts_member on accounts(member_id);
create table if not exists account_transactions (
    id varchar(36) primary key,
    account_id varchar(36) not null references accounts(id),
    request_id varchar(36) not null,
    type varchar(10) not null,
    amount bigint not null check (amount > 0),
    balance_after bigint not null,
    created_at timestamp default current_timestamp not null,
    unique(account_id, request_id)
);
