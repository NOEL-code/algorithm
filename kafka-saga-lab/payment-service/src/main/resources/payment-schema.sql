create table if not exists payments (
    order_id varchar(36) primary key,
    amount bigint not null,
    status varchar(20) not null
);
