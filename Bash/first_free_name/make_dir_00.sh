#!/bin/bash

PRECISION_NUMBER=$1
NAME_FOLDER=$2
NAME_RESULTS_FOLDER=$3

PATH_FOLDER="${NAME_FOLDER}/${NAME_RESULTS_FOLDER}"

if [[ ! -e $NAME_FOLDER ]];
	then
    		mkdir $NAME_FOLDER
fi

num=0
zeros=$(printf %0${PRECISION_NUMBER}d 0)

if [[ -e $NAME_FOLDER/$NAME_RESULTS_FOLDER.${zeros} ]];
	then
		num=$(ls -d $PATH_FOLDER* | sed "s#${PATH_FOLDER}.##" | sort -n | tail -1)
		echo num = $num
		num=`expr ${num} + 1`
fi

num=$(printf %0${PRECISION_NUMBER}d $num)

mkdir  ${PATH_FOLDER}.${num}
echo ${PATH_FOLDER}.${num} 'has been created!'