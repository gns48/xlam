#!/bin/bash

tables="tension_kinds strings hannabach_tension tensions"
usage="Usage: $0 [<schema name>]"

case $1 in
    "-h"|"-?"|"--help"|"-help")
        echo "$usage"
        exit 0
        ;;
    -*)
        echo "Bad option"
        echo "$usage"
        exit 1
        ;;
    *)
        if [ -n "$1" ] ; then
            schema="$1"
        fi
        ;;
esac

for t in $tables ; do
	psql -c "DROP TABLE IF EXISTS $t CASCADE"
done

psql < strings-schema.sql

for t in $tables ; do
	psql -c "copy $t FROM STDIN (FORMAT 'csv', DELIMITER ',')" < $t.csv
done

if [ -n "$schema" ] ; then
	psql -c "DROP SCHEMA IF EXISTS $schema CASCADE; CREATE SCHEMA $schema"
	for t in $tables ; do psql -c "ALTER TABLE $t SET SCHEMA $schema" ; done
fi




