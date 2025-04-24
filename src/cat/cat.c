#define _GNU_SOURCE
#include <errno.h>
#include <getopt.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 1024

struct flags_for_flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
};

struct flags_for_flags read_flag(int argc, char* argv[]) {
  struct option long_flags[] = {{"number-nonblank", 0, NULL, 'b'},
                                {"number", 0, NULL, 'n'},
                                {"squeeze-blank", 0, NULL, 's'},
                                {NULL, 0, NULL, 0}};
  int flag_now = getopt_long(argc, argv, "beEnstTv", long_flags, NULL);
  struct flags_for_flags flag = {0, 0, 0, 0, 0, 0};
  for (; flag_now != -1;
       flag_now = getopt_long(argc, argv, "beEnstTv", long_flags, NULL)) {
    switch (flag_now) {
      case 'b':
        flag.b = 1;
        break;
      case 'e':
        flag.e = 1;
        flag.v = 1;
        break;
      case 'n':
        flag.n = 1;
        break;
      case 's':
        flag.s = 1;
        break;
      case 't':
        flag.t = 1;
        flag.v = 1;
        break;
      case 'v':
        flag.v = 1;
        break;
      case 'E':
        flag.e = 1;
        break;
      case 'T':
        flag.t = 1;
        break;
      default:
        break;
    }
  }
  return flag;
}

void print_line(char* line, int num, int len, struct flags_for_flags flag,
                int flag_last, int idx) {
  if (flag.n && !flag.b)
    printf("%6d\t", num);
  else if (flag.b && len > 0)
    printf("%6d\t", num);
  int j = 0;
  while (line[j - 1] != '\n' && !(flag_last && j == idx)) {
    if (line[j] == '\t' && flag.t)
      printf("^I");
    else if ((flag.e && line[j] == '\n'))
      printf("$\n");
    else {
      if (flag.v) {
        if (line[j] < 32 && line[j] != '\t' && line[j] != '\n')
          printf("^%c", line[j] + 64);
        else if (line[j] == 127)
          printf("^?");
        else
          printf("%c", line[j]);
      } else
        printf("%c", line[j]);
    }
    j++;
  }
}

void process_file(char* filename, struct flags_for_flags flag) {
  FILE* f = fopen(filename, "r");
  if (f) {
    char str[SIZE];
    int k = 1, empty_str = 0, idx = 0, flag_last = 0;
    char ch;
    while ((ch = fgetc(f)) != EOF) {
      str[idx++] = ch;
      if (ch == '\n' || idx == SIZE - 1) {
        str[idx] = '\0';
        if (flag.s && str[0] == '\n') {
          if (!empty_str) {
            putchar('\n');
            empty_str = 1;
          }

        } else {
          int len = idx;
          if (idx > 0 && str[0] == '\n') len = 0;
          print_line(str, k, len, flag, flag_last, idx);
          if (flag.n || (flag.b && len > 0)) k++;
          empty_str = 0;
        }
        idx = 0;
      }
    }
    if (idx > 0) {
      str[idx] = '\0';
      flag_last = 1;
      int len = idx;
      print_line(str, k, len, flag, flag_last, idx);
    }
    fclose(f);
  } else
    printf("cat: %s: No such file or directory\n", filename);
}

int main(int argc, char* argv[]) {
  setlocale(LC_ALL, "");
  struct flags_for_flags flag = read_flag(argc, argv);
  for (int i = optind; i < argc; i++) {
    process_file(argv[i], flag);
  }
  return 0;
}