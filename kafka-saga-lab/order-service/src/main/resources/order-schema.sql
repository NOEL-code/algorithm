-- 학습: 주문 상태는 프로세스 메모리가 아니라 DB에 남겨 재시작 후 이어간다.
-- ADD COLUMN IF NOT EXISTS는 기존 실습 DB를 유지하는 멱등 초기화다. 운영의 버전 관리 마이그레이션을 대체하지 않는다.
create table if not exists orders (
    id varchar(36) primary key,
    product_id varchar(100) not null,
    quantity integer not null check (quantity > 0),
    amount bigint not null check (amount > 0),
    status varchar(30) not null,
    reason varchar(100)
);
alter table orders add column if not exists member_id varchar(36);
alter table orders add column if not exists account_id varchar(36);
