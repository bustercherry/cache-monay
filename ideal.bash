#!/bin/bash

read line
while read line; do

  #echo $line

  trace=$(echo $line | awk -F, '{print $1}')
  ideal_time=$(echo $line | awk -F, '{print $2}')
  ideal_cpi=$(echo $line | awk -F, '{print $3}')
  misal_time=$(echo $line | awk -F, '{print $4}')
  misal_cpi=$(echo $line | awk -F, '{print $5}')

  #echo "${trace}|${ideal_time}|${ideal_cpi}|${misal_time}|${misal_cpi}"

  #sed -e 's/\(^\s*Inst.*\)$/\1\nIdeal: Exec. Time = ${ideal_time}; CPI = ${ideal_cpi}\nIdeal mis-aligned: Exec. Time = ${misal_time}; CPI = ${misal_cpi}/' configs/${trace}*

  for file in output/${trace}*; do
    sed -ib -e "s/\(^\s*Inst.*\)$/\1\nIdeal: Exec. Time = ${ideal_time}; CPI = ${ideal_cpi}\nIdeal mis-aligned: Exec. Time = ${misal_time}; CPI = ${misal_cpi}/" $file
  done

  #echo -e "\
#Ideal: Exec. Time = ${ideal_time}; CPI = ${ideal_cpi}\n\
#Ideal mis-aligned: Exec. Time = ${misal_time}; CPI = ${misal_cpi}"

done
