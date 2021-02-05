#!/bin/bash

tables="tension_kinds strings hannabach_tension tensions"

for t in $tables ; do
	psql -c "DROP TABLE IF EXISTS $t CASCADE"
done

psql < strings-schema.sql

for t in $tables ; do
	psql -c "copy $t FROM STDIN (FORMAT 'csv', DELIMITER ',')" < $t.csv
done

if [ -n "$1" ] ; then
	psql -c "DROP SCHEMA IF EXISTS $1 CASCADE; CREATE SCHEMA $1"
	for t in $tables ; do psql -c "ALTER TABLE $t SET SCHEMA $1" ; done
fi
