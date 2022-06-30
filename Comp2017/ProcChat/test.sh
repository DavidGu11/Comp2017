#!/bin/bash
#! /usr/bin/env sh
count=0 # number of test cases run so far
var="C"


for folder in `ls -d test_folder/*.out | sort -V`; do
    name=`echo "$folder" | cut -d'.' -f1`
    echo Running test $name
    y=${folder%.out}
    dir=${y##*/}
    gcc server.c -o server 
    gcc ${name}0.c -o ${name}0 
    gcc ${name}1.c -o ${name}1
    
    expected_file=${name}.out

    if [[ "$dir" =~ ^Cl.* ]]; then
        ./test &
        sleep 1
        ./${name}0 > temp &
        sleep 1
        ./${name}1 
        output="temp"
        diff $output $expected_file || echo "Test $name: failed!\n"

    else
        ./test &
        ./${name}1 &
        eval "./${name}0 &"  | diff --ignore-all-space - $expected_file || echo "Test $name: failed!\n"
    fi

    count=$((count+1))
    rm gevent
    rm ${name}0
    rm ${name}1
    rm -rf ${dir}
    rm server
done


kill $(ps aux | grep './test' | awk '{print $2}')
echo "Finished running $count tests!"