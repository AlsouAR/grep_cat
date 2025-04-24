CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
all: s21_cat
CF = clang-format

s21_cat: cat.o
	$(CC) $(CFLAGS) cat.o -o s21_cat

s21_cat.o: cat.c
	$(CC) $(CFLAGS) -c cat.c -o cat.o

testc:
	sh t_cat.sh

clean:
	rm -f *.o s21_cat
ver:
	$(CF) -i *.c
check:
	$(CF) -n *.c
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	leaks -atExit -- ./s21_cat

rebuild: clean all
