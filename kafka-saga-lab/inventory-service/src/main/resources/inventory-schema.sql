-- 학습: available >= 0 CHECK와 조건부 UPDATE는 각각 저장소 제약과 정상 업무 분기라는 다른 역할이다.
-- reservations의 order_id PK로 새 eventId의 동일 예약 명령도 두 번 차감하지 않는다.
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
