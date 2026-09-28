create table if not exists accounts (
    id varchar(36) primary key,
    member_id varchar(36) not null,
    name varchar(60) not null,
    balance bigint not null default 0 check (balance >= 0 and balance <= 9000000000000),
    status varchar(10) not null default 'OPEN' check (status in ('OPEN','CLOSED')),
    created_at timestamp default current_timestamp not null
);
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
