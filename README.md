# xlam
Different utilities &amp; small distinct projects 

## ./boost-asio:
boost asio echo client and server (inspired by boost examples)

- echo-client.cpp
- echo-server.cpp

## ./elf:
ELF section extractor

- extract-sect.cpp
- extract-sect.hpp
- fmapper.hpp

## ./utility:
Different utility &amp; tests

- rowhummer.c -- The 'Rowhammer' memory bug tester. Not mine
- backtrace.c -- backtrace test
- ratelimit.c - some test with atomics

## ./vigener:

text_stat.cpp -- repeating substrings dumper for vigener.txt
vigener.txt  -- encrypted message from Charles Wetherell's
    'Etudes for programmers'. Russian

## ./guitar-strings
Small database of classical guitar strings. Tested on Postgres RDBMS 
but it has nothing postgres-specific staff.
Data collected from public catalogues and russian online shops.
Prices given in rubles just for reference and comarison, may be not actual.

- guitar-schema.sql: database creation script
- guitar-schema.sh: intergration script (bash)
- guitar-strings.csv           :
- guitar-tensions.csv          : table data 
- guitar-hannabach_tension.csv :

## ./

- stext.c -- quine in C
- sqltest.sql -- some test task written in sql
	         with joins and aggregating functions


