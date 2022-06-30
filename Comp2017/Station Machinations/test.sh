#! /usr/bin/env sh
count=0 # number of test cases run so far
gcc timetable.c -o timetable
# Assume all `.in` and `.out` files are located in a separate `tests_e2e` directory

for folder in `ls -d tests/*.args | sort -V`; do
    name=`echo "$folder" | cut -d'.' -f1`
    echo Running test $name.

    config_file=${name}.args
    expected_file=${name}.out
    in_file=${name}.in

    cat $in_file | xargs -a $config_file ./timetable  | diff - $expected_file || echo "Test $name: failed!\n"

    count=$((count+1))
done

echo "Finished running $count tests!"
rm timetable