#!/usr/bin/env bash

IN="nome1;nome2;nome3"

arr=$(echo $IN | tr ";" "\n")

for x in $arr
do
	echo "> [$x]"
done
