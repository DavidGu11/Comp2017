#!/bin/bash
#! /usr/bin/env sh
count=0 # number of test cases run so far
gcc objdump_x2017.c -o objdump_x2017
# Assume all `.in` and `.out` files are located in a separate `tests_e2e` directory

for folder in `ls -d tests/*.x2017 | sort -V`; do
    name=`echo "$folder" | cut -d'.' -f1`
    echo Running objdump $name.

    expected_file=${name}.out
    in_file=${name}.x2017
    eval "./objdump_x2017 $in_file"  | diff --ignore-all-space - $expected_file || echo "Test $name: failed!\n"

    count=$((count+1))
done


rm objdump_x2017
gcc vm_x2017.c -o vm_x2017
for folder in `ls -d tests/*.x2017 | sort -V`; do
    name=`echo "$folder" | cut -d'.' -f1`
    echo Running vm $name.

    expected_file=${name}.out1
    in_file=${name}.x2017
    eval "./vm_x2017 $in_file"  | diff --ignore-all-space - $expected_file || echo "Test $name: failed!\n"

done

rm vm_x2017
echo "Finished running $count tests!"
