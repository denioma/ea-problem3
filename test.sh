#!/bin/bash

for output in testcases/*.out rui/*.out
do
    testcase=`echo $output | cut -d "." -f 0`
    out=`./main < $testcase | diff $output -`
    if [ $? -ne 0 ];
    then
        echo "Failed $testcase"
    else
        echo "Passed $testcase"
    fi
done
