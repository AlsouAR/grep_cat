#!/bin/bash

# Инициализация переменных
SUCCESS=0
FAIL=0
COUNTER=0
all_flags=("-e" "-i" "-v" "-c" "-l" "-n")
bonus_flags=("-i" "-v" "-c" "-l" "-n")
patterns=("Depp" "english")
input_files=("test_1.txt" "test_2.txt")

# Функция для выполнения теста и сравнения результатов
run_test() {
    local flags="$1"
    local pattern="$2"
    local file_args="$3"
    local test_description="$4"

    echo "$test_description :"

    # Выполнение вашей программы
    ./s21_grep $flags "$pattern" $file_args > your_output.txt
    # Выполнение оригинального grep
    grep $flags "$pattern" $file_args > original_output.txt

    # Сравнение результатов
    if diff -q your_output.txt original_output.txt >/dev/null 2>&1; then
        let "COUNTER++"
        let "SUCCESS++"
        echo "\033[92m$COUNTER\033[0m - Success"
    else
        let "COUNTER++"
        let "FAIL++"
        echo "\033[31m$COUNTER\033[0m - Fail"
    fi

    # Удаление временных файлов
    rm your_output.txt
    rm original_output.txt
}

# --- Тесты с одним файлом ---
echo " "
echo " ONE FILE TESTS "
echo " "

for flags in "${all_flags[@]}"; do
    for pattern in "${patterns[@]}"; do
        for input_file in "${input_files[@]}"; do
            run_test "$flags" "$pattern" "$input_file" "$flags $pattern $input_file"
        done
    done
done

# --- Тесты с двумя файлами ---
echo " "
echo " TWO FILES TESTS "
echo " "

for flags in "${all_flags[@]}"; do
    for pattern in "${patterns[@]}"; do
        for file_1 in "${input_files[@]}"; do
            for file_2 in "${input_files[@]}"; do
                if [ "$file_1" != "$file_2" ]; then
                    run_test "$flags" "$pattern" "$file_1 $file_2" "$flags $pattern $file_1 $file_2"
                fi
            done
        done
    done
done

# --- Тесты с двумя флагами ---
echo " "
echo " TWO FLAGS TESTS "
echo " "
for flag_1 in "${bonus_flags[@]}"; do
    for flag_2 in "${bonus_flags[@]}"; do
        if [ "$flag_1" != "$flag_2" ]; then
            for pattern in "${patterns[@]}"; do
                for input_file in "${input_files[@]}"; do
                    run_test "$flag_1 $flag_2" "$pattern" "$input_file" "$flag_1 $flag_2 $pattern $input_file"
                done
            done
        fi
    done
done


# Вывод общего результата
echo " "
echo "Total: $COUNTER, Success: $SUCCESS, Fail: $FAIL"