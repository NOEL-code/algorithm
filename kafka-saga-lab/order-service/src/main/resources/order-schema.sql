create table if not exists orders (
    id varchar(36) primary key,
    product_id varchar(100) not null,
    quantity integer not null check (quantity > 0),
    amount bigint not null check (amount > 0),
    status varchar(30) not null,
    reason varchar(100)
);
