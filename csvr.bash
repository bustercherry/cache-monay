#!/bin/bash

R_NUM=".*=\s*\(\S*\).*"
D_NUM=".*=\s*\$\(\S*\).*"
T_NUM="[^=]*=\s*\(\S*\),[^=]*=\s*\(\S*\),[^=]*=\s*\(\S*\)"
C_SED="s/${R_NUM}/\1,/"
D_SED="s/${D_NUM}/\1,/"
T_SED="s/${T_NUM}/\1,\2,\3,/"
N_SED="s/${D_NUM}/\1/"
F_SED="s/\([^-]*\)-\(.*\).yaml/\1,\2,/"

echo "trace,config,num_reads,num_writes,num_inst,tot_ref,read_cycles,write_cycles,\
inst_cycles,tot_time,avg_cyc_per_read,avg_cyc_per_write,avg_inst_per_inst,\
L1i_hits,L1i_miss,L1i_total,L1i_kickout,L1i_dirty_k,L1i_transfer,\
L1d_hits,L1d_miss,L1d_total,L1d_kickout,L1d_dirty_k,L1d_transfer,\
L2_hits,L2_miss,L2_total,L2_kickout,L2_dirty_k,L2_transfer,\
L1i_cost,L1d_cost,L2_cost,mem_cost,tot_cost"

for file in $@; do
  echo -n "${file}" | sed -e "${F_SED}"
  (
      #Number of reference types: 
    read line
      #  Number of reads   =  2549106849  [25.49%]
    read line; echo -n $line | sed -e "${C_SED}"
      #  Number of writes  =   626305991  [6.26%]
    read line; echo -n $line | sed -e "${C_SED}"
      #  Number of inst    =  6824587160  [68.25%]
    read line; echo -n $line | sed -e "${C_SED}"
      #  Total             =  10000000000
    read line; echo -n $line | sed -e "${C_SED}"
      #
    read line
      #Total cycles for all activities: 
    read line
      #  Cycles for reads  =  29021325420  [55.88%]
    read line; echo -n $line | sed -e "${C_SED}"
      #  Cycles for writes =  11433176077  [22.01%]
    read line; echo -n $line | sed -e "${C_SED}"
      #  Cycles for inst   =  11480910007  [22.11%]
    read line; echo -n $line | sed -e "${C_SED}"
      #  Total time        =  51935411504
    read line; echo -n $line | sed -e "${C_SED}"
      #
    read line
      #Average cycles per activity:
    read line;
      #  Read  = 11.38
    read line; echo -n $line | sed -e "${C_SED}"
      #  Write = 18.25
    read line; echo -n $line | sed -e "${C_SED}"
      #  Inst  = 1.68
    read line; echo -n $line | sed -e "${C_SED}"
      #
    read line;
      #Memory level: L1i
    read line
      #  Hits   =  11393011748  [99.99%]
    read line; echo -n $line | sed -e "${C_SED}"
      #  Misses =     1303125  [0.01%]
    read line; echo -n $line | sed -e "${C_SED}"
      #  Total  =  11394314873
    read line; echo -n $line | sed -e "${C_SED}"
      #  Kickouts = 1302869, Dirty kickouts = 0, Transfers = 1303125
    read line; echo -n $line | sed -e "${T_SED}"
      #
    read line
      #Memory level: L1d
    read line
      #  Hits   =  4214841543  [94.54%]
    read line; echo -n $line | sed -e "${C_SED}"
      #  Misses =   243330909  [5.46%]
    read line; echo -n $line | sed -e "${C_SED}"
      #  Total  =  4458172452
    read line; echo -n $line | sed -e "${C_SED}"
      #  Kickouts = 243330653, Dirty kickouts = 94594555, Transfers = 243330909
    read line; echo -n $line | sed -e "${T_SED}"
      #
      #
    read line
      #Memory level: L2
    read line
      #  Hits   =   198286480  [58.45%]
    read line; echo -n $line | sed -e "${C_SED}"
      #  Misses =   140942109  [41.55%]
    read line; echo -n $line | sed -e "${C_SED}"
      #  Total  =   339228589
    read line; echo -n $line | sed -e "${C_SED}"
      #  Kickouts = 140941597, Dirty kickouts = 65799800, Transfers = 140942109
    read line; echo -n $line | sed -e "${T_SED}"
      #
      #
    read line
      #Cost analysis: 
    read line
      #  L1i cache cost = $200 
    read line; echo -n $line | sed -e "${D_SED}"
      #  L1d cache cost = $200 
    read line; echo -n $line | sed -e "${D_SED}"
      #  L2  cache cost = $50 
    read line; echo -n $line | sed -e "${D_SED}"
      #  Memory cost    = $75
    read line; echo -n $line | sed -e "${D_SED}"
      #  Total cost     = $525
    read line; echo -n $line | sed -e "${N_SED}"
    echo
  ) < $file
done
