#!/bin/bash
rm ./result-lin.tsv

for i in 0 1 2 3 4 5 6
do
   ./test.bin $i >> ./result-lin.tsv
done
