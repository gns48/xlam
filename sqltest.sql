DROP TABLE IF EXISTS instr CASCADE;
CREATE TABLE instr(
       id serial primary key,
       name varchar not null,
       CONSTRAINT "instr_name_uniq" UNIQUE("name")
);

DROP TYPE IF EXISTS side_type CASCADE;
CREATE TYPE side_type AS ENUM ('B', 'S');
DROP TABLE IF EXISTS deals CASCADE;
CREATE TABLE deals (
       id serial primary key,
       instr_id int REFERENCES instr(id),
       date timestamp not null default now(),
       side side_type not null,
       price NUMERIC not null,
       qty int not null
);

DROP FUNCTION IF EXISTS calc_pos();
create function  calc_pos() returns table(instr_id int,
                                          name varchar,
                                          qty bigint,
                                          cost numeric,
                                          pnl numeric)
AS $$
with
    -- bought view
    bvt as (select instr_id, sum(qty) as qty, sum(price * qty)/sum(qty) as cost
            from deals where deals.side = 'B' group by (instr_id,side)),
    -- sold view
    svt as (select instr_id, sum(qty) as qty, sum(price * qty)/sum(qty) as cost
            from deals where deals.side = 'S' group by (instr_id,side)),
    -- names view, left outer joins are needed because instrument id
    -- may be absent in either view
    nvt as (select id, name from instr
            left outer join bvt on bvt.instr_id = instr.id
            left outer join svt on svt.instr_id = instr.id)
select nvt.id, nvt.name,
       coalesce(bvt.qty,0) - coalesce(svt.qty, 0) as qty,
       coalesce(svt.cost,0) - coalesce(bvt.cost, 0) as cost,
       coalesce((svt.cost - bvt.cost) * svt.qty, 0) as pnl
       from nvt left join bvt on bvt.instr_id = nvt.id
                left join svt on svt.instr_id = nvt.id
$$ language sql;

INSERT INTO instr(id, name) values(1, 'AAPL');                        
INSERT INTO deals(instr_id, side, price, qty) VALUES(1, 'B', 100, 10);
INSERT INTO deals(instr_id, side, price, qty) VALUES(1, 'B', 120, 5); 
INSERT INTO deals(instr_id, side, price, qty) VALUES(1, 'B', 115, 10);

select * from calc_pos();


/*
-- AAPL cost = (100*10 + 120*5 + 115*10) / (10 + 5 + 10)
pos=# select * from calc_pos();
instr_id | name | qty | cost | pnl
----------+-------------------------
       1 | AAPL | 25  | 110.0| 0
*/

INSERT INTO instr(id, name) values(2, 'SBER');
INSERT INTO deals(instr_id, side, price, qty) VALUES(2, 'S', 230, 10);
INSERT INTO deals(instr_id, side, price, qty) VALUES(2, 'S', 210, 5); 
INSERT INTO deals(instr_id, side, price, qty) VALUES(2, 'S', 215, 10);

select * from calc_pos();


/*
-- SBER cost = (230*10 + 210*5 + 215*10) / (10 + 5 + 10)
pos=# select * from calc_pos();
instr_id | name | qty | cost | pnl
----------+-------------------------
       1 | AAPL | 25  | 110.0| 0.0
       2 | SBER | -25 | 220.0| 0.0
*/

INSERT INTO deals(instr_id, side, price, qty) VALUES(2, 'B', 230, 30);
select * from calc_pos();

/*
-- SBER cost = 230*5 / 5
-- SBER pnl = (220-230) * 25
pos=# select * from calc_pos();
instr_id | name | qty | cost | pnl
----------+-------------------------
       1 | AAPL | 25  | 110.0| 0.0
       2 | SBER |  5  | 230.0|-250.0
*/

INSERT INTO deals(instr_id, side, price, qty) VALUES(1, 'S', 130, 25);
select * from calc_pos();

/*
-- AAPL pnl = (130-110) * 25
pos=# select * from calc_pos();
instr_id | name | qty | cost | pnl
----------+-------------------------
       1 | AAPL |  0  | 110.0| 500.0
       2 | SBER |  5  | 230.0|-250.0
*/








