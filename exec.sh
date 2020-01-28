#!/bin/bash

export LD_LIBRARY_PATH=$CRAY_LD_LIBRARY_PATH:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/opt/cray/wlm_detect/1.3.3-6.0.7.1_5.6__g7109084.ari/lib64/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/opt/cray/pe/lib64/:$LD_LIBRARY_PATH

export SINGULARITYENV_LD_LIBRARY_PATH=$LD_LIBRARY_PATH
# echo $SINGULARITYENV_LD_LIBRARY_PATH

#aprun -n $TOTAL_RANKS -N $RANKS_PER_NODE singularity run -B /opt:/opt:ro /
aprun -n 1 -N 1 singularity exec -B /opt:/opt:ro ./ldap-stressor.sif /getpwuid_dos/simulator
