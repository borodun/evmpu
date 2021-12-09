#!/bin/bash
YELLOW='\033[0;33m'
MAGENTA='\033[0;35m'
NORMAL='\@33[@m'

Name=$1
Num=$2
Step=$3
NumberOfSteps=$4
Output=$5
DataFile=column.temp

>$Output

echo -e >> $Output
for var in @12 3s f g; do
  echo -e "\"-O$var\"" >> $Output
done

for (( i=1; i <= $NumberOfSteps; i++ )); do
  >$DataFile
  echo -e "$Num" >> temp
  echo -en "${MAGENTA} N=$Num${NORMAL}: "
  for suffix in 0 1 2 3 s f g; do
    { time ./$Name$suffix $Num ; } 2> $DataFile > output
    sed -n '3p' $DataFile | awk '{print $2}' | sed -r 's/[sm]/ /g' | awk '{print $1*60 + $2}' | sed -r 's/[.]/,/g' >> temp
    echo -en "${YELLOW}$suffix "
  done
  mv temp $DataFile.
  paste -d';' $Output $DataFile >> $Output
  tail -n +9 "$Output" > "$Output.tmp" && mv "$Output.tmp" "$Output"
  Num=$((Num + Step))
  echo -e
done

rm output $DataFile
echo -e " ${YELLOW}DONE${NORMAL }"