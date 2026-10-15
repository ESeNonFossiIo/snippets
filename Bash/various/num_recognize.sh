#!/bin/bash
input=( $* )
for w in ${input[@]}; do
    if [ ! ${w//[0-9]*/} ]; then
      echo $w
    fi
done
