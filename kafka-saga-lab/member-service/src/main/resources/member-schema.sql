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
