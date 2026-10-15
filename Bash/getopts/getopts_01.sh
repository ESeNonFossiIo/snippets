#!/bin/bash

function print_usage {
	echo -e "\n\tUSAGE: "
	echo -e "\t\t -H | --help  : Help"
	echo -e "\t\t -a | --aaasd : etc..."
	echo -e "\n"
}

if [[ $1 =~ ^(-[Hh]|--[Hh][Ee][Ll][Pp])$ ]]; then
	print_usage; exit 1
else
	while [[ $# -gt 0 ]]; do
		opt="$1"
		shift;
		current_arg="$1"
		if [[ "$current_arg" =~ ^-{1,2}.* ]]; then
			echo "WARNING: You may have left an argument blank. Double check your command." 
		fi
		case "$opt" in
			"-a"|"--apple"      ) APPLE="$1"; shift;;
			"-b"|"--banana"     ) BANANA="$1"; shift;;
			"-c"|"--cherry"     ) CHERRY="$1"; shift;;
			"-d"|"--dfruit"     ) DFRUIT="$1"; shift;;
			"-e"|"--eggplant"   ) EGGPLANT="$1"; shift;;
			"-f"|"--fig"        ) FIG="$1"; shift;;
			*                   ) echo "ERROR: Invalid option: \""$opt"\"" >&2
			exit 1;;
		esac
	done
fi

if [[ "$APPLE" == "" || "$BANANA" == "" ]]; then
	echo "ERROR: Options -a and -b require arguments." >&2
	exit 1
fi