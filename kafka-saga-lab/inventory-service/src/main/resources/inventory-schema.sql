create table if not exists stock (
    product_id varchar(100) primary key,
    available integer not null check (available >= 0)
);
create table if not exists reservations (
    order_id varchar(36) primary key,
    product_id varchar(100) not null,
    quantity integer not null,
    status varchar(20) not null
);
-- 재시작할 때 기존 재고를 10개로 덮어쓰지 않는다.
insert into stock(product_id, available)
select 'book', 10 where not exists (select 1 from stock where product_id = 'book');
