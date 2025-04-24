CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
CF = clang-format

all: s21_grep

s21_grep: grep.o
	$(CC) $(CFLAGS) grep.o -o s21_grep

grep.o: grep.c
	$(CC) $(CFLAGS) -c grep.c -o grep.o

testg:
	./test_grep.sh

clean:
	rm -f *.o s21_grep
ver:
	$(CF) -i *.c
check:
	$(CF) -n *.c
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	leaks -atExit -- ./s21_grep

rebuild: clean all
