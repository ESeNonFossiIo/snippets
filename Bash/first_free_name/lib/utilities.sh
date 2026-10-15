#!/bin/bash

# $1 is the name of the file
# $2 is the extension of the file
# $3 is the number of digits of the filename
function find_free_name()
{
  num=$(ls -1 $1_*.$2 | sort | tail -1 | sed "s#$1_##" | sed "s#.$2##")
  tmp=${num%%[!0]*}
  num=${num#$tmp}
  num=$(($num+1))
  zeroes=$(($3 - 1 - ${#num}))
  zeroes=$( for (( c=1; c<=$zeroes; c++ )); do echo -n "0"; done )
  file_name=$1_$zeroes$num.$2
  echo $file_name
}
export -f find_free_name


# $1 is the name of the file
# $2 is the number of digits of the direcotry name
function find_free_dir()
{
  num=$(ls -d -1 ${1}_* | sort | tail -1 | sed "s#$1_##")
  tmp=${num%%[!0]*}
  num=${num#$tmp}
  num=$(($num+1))
  zeroes=$(($2 - 1 - ${#num}))
  zeroes=$( for (( c=1; c<=$zeroes; c++ )); do echo -n "0"; done )
  file_name=$1_$zeroes$num
  echo $file_name
}
export -f find_free_dir
