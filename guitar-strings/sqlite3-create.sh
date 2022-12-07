#!/bin/bash

tables="tension_kinds strings hannabach_tension tensions"
usage="Usage: $0 database"
sq=sqlite3
schema=strings-schema.sql

case $1 in
    -h|-?|--help|-help)
        echo "$usage"
        exit 0
        ;;
    -*)
        echo "Bad option"
        echo "$usage"
        exit 1
        ;;
    *)
	if [ -z "$1" ] ; then
		echo "No databas name specified!"
		exit 1
	fi
        db="$1"
        ;;
esac

create_db () {
	rm -f $1
	$sq $1 << EOF
.read $2
EOF
}

import_table () {
	$sq $1 << EOF
.separator ","
.import $2.csv $2
EOF
}

create_db $db $schema

for t in $tables ; do import_table $db $t ; done








