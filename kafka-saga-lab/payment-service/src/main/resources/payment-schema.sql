-- 학습: order_id PK는 같은 주문 결제를 두 번 만드는 경쟁을 방어한다.
-- PROCESSING/REFUND_PENDING은 원격 계좌 결과가 아직 확정되지 않았음을 표현한다.
create table if not exists payments (
    order_id varchar(36) primary key,
    amount bigint not null,
    status varchar(20) not null
);
alter table payments add column if not exists member_id varchar(36);
alter table payments add column if not exists account_id varchar(36);
