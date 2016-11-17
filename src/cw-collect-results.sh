#!/bin/sh
mkdir ~/out/tmp
mv ~/out/*.out ~/out/tmp/
mv ~/out/*.err ~/out/tmp/
head -qn 1 ~/out/tmp/*.out >> ~/runresults.out
mv ~/out/tmp/* ~/out/processed
rmdir ~/out/tmp
cat ~/runresults.out | sort | uniq > ~/result
cat ~/result