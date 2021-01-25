DROP SCHEMA IF EXISTS guitar CASCADE;
CREATE SCHEMA guitar;

CREATE TABLE guitar.strings(
   brand varchar(20) not null,
   mark  varchar(20) not null,
   units  varchar(16) not null,
   series varchar(32),
   tension varchar(32),
   trebles varchar(64),
   basses  varchar(64),
   remark  varchar(64),
   price   integer,
   primary key(brand,mark));
CREATE INDEX strings_mark ON guitar.strings(mark);

CREATE TABLE guitar.hannabach_tension(
   abbrev varchar(4) primary key,
   tension varchar(16) not null,
   color   varchar(24) not null,
   e1 float not null,
   b2 float not null,
   g3 float not null,
   d4 float not null,
   a5 float not null,
   e6 float not null);

create table guitar.tensions(
   brand varchar(20) not null,
   mark varchar(20) not null,
   tension varchar(32) not null,
   e1 float not null,
   b2 float not null,
   g3 float not null,
   d4 float not null,
   a5 float not null,
   e6 float not null,
   primary key(brand,mark));
CREATE INDEX tensions_mark ON guitar.tensions(mark);



