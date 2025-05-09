#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""

all_flags=("-b" "-n" "-s" "-v" "-e" "-t" ) #("-T" "-E" "--number-nonblank" "number" "squeeze-blank")
input_files=("bytes.txt" "test1.txt")

echo " "
echo " ONE FILES "
echo " "
for flags in "${all_flags[@]}"
do
    for input_file in "${input_files[@]}"
    do
        echo "$flags $input_file :"
        ./s21_cat "$flags" "$input_file" > your_output.txt
        cat "$flags" "$input_file" > original_output.txt
            
        if diff -q your_output.txt original_output.txt >/dev/null 2>&1;
        then
            let "COUNTER++"
            let "SUCCESS++"
            echo "\033[92m$COUNTER\033[0m - Success"
        else
            let "COUNTER++"
            let "FAIL++"
            echo "\033[31m$FAIL\033[0m - Fail"
        fi

        rm your_output.txt
        rm original_output.txt
    done
done

echo " "
echo " TWO FILES "
echo " "
for flags in "${all_flags[@]}"
do
    for file_1 in "${input_files[@]}"
    do
        for file_2 in "${input_files[@]}"
        do
            if [ $file_1 != $file_2 ]
            then
                echo "$flags $file_1 $file_2:"
                ./s21_cat "$flags" "$file_1" "$file_2"> your_output.txt
                cat "$flags" "$file_1" "$file_2" > original_output.txt
                    
                if diff -q your_output.txt original_output.txt >/dev/null 2>&1;
                then
                    let "COUNTER++"
                    let "SUCCESS++"
                    echo "\033[92m$COUNTER\033[0m/ - Success"
                else
                    let "COUNTER++"
                    let "FAIL++"
                    echo "\033[91m$COUNTER\033[0m/ - Fail"
                fi

                rm your_output.txt
                rm original_output.txt
            fi
        done
    done
done
echo "Total: $COUNTER, Success: $SUCCESS, Fail: $FAIL"
