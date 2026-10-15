#!/bin/bash

# This tool provide a README.md of the folder
# If there is some file named DESCRIPTION.md
#  the content of this file is added
#  to the README.md as comment.
#

function print_sharp {
  if [[ $1 == 0 ]]
  then
    printf " * "
  else
    # printf "\n"
    printf "%0.s#" $(/usr/bin/seq 1 $1)
  fi
}

function count_dept {
  echo $1 | /usr/bin/grep -o '/' | /usr/bin/grep '/' -c
}

FILE=$(ls -R | sed 's#:# # ' | sed "s#\n# #")
# FILE=$(ls -R | tr "/" "\n")
ABS_PATH=$PWD
PATH=
space="%0.s"

for file in $FILE;
do
  if [[ "$file" =~ (.\/) ]];
    then
      PATH=$file
      pathFile=$file
      if [[ $(count_dept $file) -ge 3 ]]
        then
	space="%0.s"
        # space="\t\t"
      elif [[ $(count_dept $file) == 2 ]]
        then
	space="%0.s"
        # space="\t"
      elif [[ $(count_dept $file) == 1 ]]
        then
        printf "\n\n"
        space="%0.s"
      else
        space="%0.s"
      fi
  else
    pathFile=$PATH/$file
  fi
  if [[ "$file" =~ (DESCRIPTION.md) ]];
    then
    printf "\n > "
    /bin/cat $pathFile
    printf "\n"
  else
    printf $space
    print_sharp $(count_dept $file)
    printf "[$(echo $file | /usr/bin/sed 's#./# #')]($pathFile)"
    printf "\n"
  fi
  printf "\n"
done
