#!/bin/bash
#! /usr/bin/env sh
count=0 # number of test cases run so far


for folder in `ls -d test_folder/*.c | sort -V`; do
    name=`echo "$folder" | cut -d'.' -f1`
    echo Running test $name
    gcc ${name}.c -o ${name} -lm
    expected_file=${name}.out

    eval "./${name}"  | diff --ignore-all-space - $expected_file || echo "Test $name: failed!\n"

    count=$((count+1))
    rm ${name}
done



echo "Finished running $count tests!"
