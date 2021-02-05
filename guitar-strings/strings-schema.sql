CREATE TABLE tension_kinds(tension varchar(16) primary key);
   
CREATE TABLE strings(
   brand varchar(20) not null,
   mark  varchar(20) not null primary key,
   units  varchar(16) not null,
   series varchar(32),
   tension varchar(16) references tension_kinds,
   trebles varchar(64),
   basses  varchar(64),
   remark  varchar(64),
   price   integer);
CREATE UNIQUE INDEX strings_bmark on strings(brand, mark);

CREATE TABLE hannabach_tension(
   abbrev varchar(4) primary key,
   tension varchar(16) references tension_kinds,
   color   varchar(24) not null,
   e1 float not null,
   b2 float not null,
   g3 float not null,
   d4 float not null,
   a5 float not null,
   e6 float not null);

create table tensions(
   brand varchar(20) not null,
   mark varchar(20) not null primary key references strings,
   tension varchar(16) references tension_kinds,
   e1 float not null,
   b2 float not null,
   g3 float not null,
   d4 float not null,
   a5 float not null,
   e6 float not null);
CREATE UNIQUE INDEX tensions_bmark on tensions(brand, mark);
