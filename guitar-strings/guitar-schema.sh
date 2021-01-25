#!/bin/bash
ht="copy guitar.hannabach_tension FROM STDIN (FORMAT 'csv', DELIMITER ',')"
st="copy guitar.strings FROM STDIN (FORMAT 'csv', DELIMITER ',')"
tn="copy guitar.tensions FROM STDIN (FORMAT 'csv', DELIMITER ',')"
psql < guitar-schema.sql
psql -c "$st" < guitar-strings.csv
psql -c "$ht" < guitar-hannabach_tension.csv
psql -c "$tn" < guitar-tensions.csv
