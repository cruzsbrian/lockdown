#!/bin/bash

# NOTE: this should be run from the root lockdown directory

make build_eval

rm temp/*

echo ""
echo "Building eval data:"
echo "0%--------------------------------------------------------------------------100%"

./bin/build_eval 50000 $1 6 > temp/$1.txt &
./bin/build_eval 50000 $(expr $1 + 1) 6 > temp/$(expr $1 + 1).txt &
./bin/build_eval 50000 $(expr $1 + 2) 6 > temp/$(expr $1 + 2).txt &
./bin/build_eval 50000 $(expr $1 + 3) 6 > temp/$(expr $1 + 3).txt &
wait

echo ""
echo ""
echo "Training eval parameters:"

cat temp/* > data/$1-$(expr $1 + 3).txt
rm temp/*

python3 src/analysis/train_eval.py data/$1-$(expr $1 + 3).txt
