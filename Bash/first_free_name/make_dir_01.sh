#!/bin/bash

PRECISION_NUMBER=4
NAME_FOLDER="folder"
NAME_RESULTS_FOLDER="data"

if [[ $1 =~ ^(-[Hh]|--[Hh][Ee][Ll][Pp])$ ]]; then
	echo -e "\n\tUSAGE: "
	echo -e "\t\t -H | --help  : Help"
	echo -e "\t\t -F           : Main folder name, default is '$NAME_FOLDER'"
	echo -e "\t\t -f           : Data folder name, default is '$NAME_RESULTS_FOLDER'"
	echo -e "\t\t -n           : Number lenght, default is '$PRECISION_NUMBER' (e.g. data.0001)"	
	echo -e "\n"
	exit 1
else
	while [[ $# -gt 0 ]]; do
		opt="$1"
		shift;
		current_arg="$1"
		if [[ "$current_arg" =~ ^-{1,2}.* ]]; then
			echo "WARNING: You may have left an argument blank. Double check your command." 
		fi
		case "$opt" in
			"-f" ) NAME_RESULTS_FOLDER="$1"; shift;;
			"-F" ) NAME_FOLDER="$1"; shift;;
			"-n" ) PRECISION_NUMBER="$1"; shift;;
			*    ) echo "ERROR: Invalid option: \""$opt"\"" >&2
			exit 1;;
		esac
	done
fi

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
	num=`expr ${num} + 1`
fi

num=$(printf %0${PRECISION_NUMBER}d $num)

mkdir  ${PATH_FOLDER}.${num}

echo ${PATH_FOLDER}.${num}