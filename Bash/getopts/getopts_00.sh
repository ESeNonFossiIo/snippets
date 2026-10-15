#!/bin/bash

while getopts adbm name
do
	case $name in
		a) A=$name;;
		b) B=$name;;
		d) dopt=1;;
		m) mopt=1;;
		*) echo "Invalid arg";;
	esac
done

DT=`date '+%d %b'`

if [[ ! -z $dopt ]]
	then
	echo "Date is: " ${DT/ */}
fi
if [[ ! -z $mopt ]]
	then
	echo "Month is: " ${DT/* /}
fi

if [[ ! -z $A ]]
	then
		echo $A
fi

if [[ ! -z $B ]]
	then
		echo $B
fi 

shift $(($OPTIND -1))
