#!/bin/bash
rm ./result-lin.tsv

for i in 0 1 2
do
   ./test.bin $i >> ./result-lin.tsv
done
