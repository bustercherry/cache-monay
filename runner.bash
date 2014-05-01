#!/bin/bash

for trace in ../traces-long/*; do
  for config  in configs/*; do
  
    while [ `jobs -l | wc -l` -ge 10 ]; do
      #jobs -l
      sleep 300
    done
  
    echo "zcat $trace | ./cache $config"
    zcat $trace | ./cache $config > "output/`basename -s .gz $trace`-`basename $config`" &
  
  done

  while [ `jobs -l | wc -l` -gt 3 ]; do
    jobs -l
    echo "--------------------------------------------------------------------------------"
    sleep 300
  done

done
