#!/bin/bash
echo "$(date) - $(squeue -p teaching -u $(whoami) | grep node-sw | wc -l) running - $(squeue -p teaching -u $(whoami) | grep $(whoami) | wc -l) remaining - $(squeue -p teaching | grep teaching | wc -l) queued - $(COUNTER=-1 ; squeue -p teaching | (while read -r line ; do if test "${line#*$(whoami)}" != "$line" ; then break ; fi ; export COUNTER=$[COUNTER+1]  ; done ; echo $COUNTER)) jobs until yours are served"