#!/bin/bash/

function print_from_to
{
  echo "$1" | head -n $2 | tail -n $(($2-$3+1))
}

function print_line
{
  echo "$1" | head -n $2 | tail -n 1
}



# External Drive
################################################################################
N_LINES=$(df -lH | wc -l | xargs)
let N_LINES=${N_LINES}-1
DF_OUT=$(df -lH | tail -${N_LINES})

while IFS="\n" read -ra ADDR; do
     for i in "${ADDR[@]}"; do
        NAME=$(echo $i | cut -d' ' -f9)
        SIZE=$(echo $i | cut -d' ' -f2)
        AVAI=$(echo $i | cut -d' ' -f4)
        echo -ne "${NAME}@${SIZE}@${AVAI}#"
     done
done <<< "$DF_OUT"



# External Drive
################################################################################
LSUB_OUT=$(lsusb -v )

# Detech all lines that are for Removable device:
LINES=$(lsusb -v | grep -Ein '(Removable)' | cut -f1 -d" " | sed 's#:##')

for LINE in ${LINES}
do
  let START=${LINE}-2
  let END=${LINE}+8

  NAME=$(print_line "${LSUB_OUT}" "${START}" | sed 's#:##' | xargs)
  SIZE=$(print_line "${LSUB_OUT}" $((${END}-1)) | sed 's#Capacity:##' | xargs | sed -e 's/([^()]*)//g')
  AVAI=$(print_line "${LSUB_OUT}" "${END}" | sed 's#Available:##' | xargs | sed -e 's/([^()]*)//g')

  echo -ne "${NAME}@${SIZE}@${AVAI}#"
done
