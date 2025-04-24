#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 10000
struct arguments {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  char* pattern;
};
struct arguments read_flag(int argc, char* argv[]) {
  int flag_now;
  struct arguments arg = {0};
  while ((flag_now = getopt(argc, argv, "e:ivcln")) != -1) {
    switch (flag_now) {
      case 'e':
        arg.e = 1;
        arg.pattern = optarg;
        break;
      case 'i':
        arg.i |= REG_ICASE;
        break;
      case 'v':
        arg.v = 1;
        break;
      case 'c':
        arg.c = 1;
        break;
      case 'l':
        arg.c = 1;
        arg.l = 1;
        break;
      case 'n':
        arg.n = 1;
        break;
      case '?':
        fprintf(stderr, "Invalid option: %c\n", optopt);
        exit(1);
        break;
      default:
        break;
    }
  }
  if (arg.pattern == NULL) {
    if (optind < argc) {
      arg.pattern = argv[optind];
      optind++;
    } else {
      fprintf(stderr, "Pattern not found.\n");
      exit(1);
    }
  }
  return arg;
}
int get_line(char* line, FILE* file) {
  int len = 0;
  int c;
  while ((c = fgetc(file)) != EOF && c != '\n') {
    line[len++] = c;
  }
  if (len >= MAX - 1) {
    len = -2;
  }
  if (c == EOF && len == 0) len = -1;
  if (len != -1) {
    line[len] = '\n';
    line[len + 1] = '\0';
  }
  return len;
}
void process_file(char* filename, struct arguments arg, regex_t* reg,
                  int counter, int argc) {
  FILE* fp = fopen(filename, "r");
  if (fp) {
    char line[MAX];
    int len = 0;
    int num = 0;
    while ((len = get_line(line, fp)) != -1) {
      int result = regexec(reg, line, 0, NULL, 0);
      num++;
      if ((result == 0 && !arg.v) || (!result == 0 && arg.v)) {
        counter++;
        if (argc - optind > 1 && !arg.c && !arg.l) printf("%s:", filename);
        if (arg.n && !arg.c && !arg.l) printf("%d:", num);
        if (!arg.c && !arg.l) printf("%s", line);
      }
    }
    fclose(fp);
    if (arg.c && !arg.l) {
      int c_f = argc - optind;
      if (c_f > 1)
        printf("%s:%d\n", filename, counter);
      else
        printf("%d\n", counter);
    }
    if (arg.l && counter > 0) {
      printf("%s\n", filename);
    }
  } else
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
}

void print_file(struct arguments arg, int argc, char** argv) {
  regex_t re;
  regcomp(&re, arg.pattern, arg.i);
  int counter = 0;
  for (int i = optind; i < argc; i++) {
    process_file(argv[i], arg, &re, counter, argc);
  }
  regfree(&re);
}
int main(int argc, char* argv[]) {
  struct arguments arg = read_flag(argc, argv);
  if (arg.pattern == NULL) {
    if (optind < argc) {
      if (arg.e == 1) {
      } else {
        arg.pattern = argv[optind];
        optind++;
      }
    } else {
      fprintf(stderr, "Pattern not found.\n");
      exit(1);
    }
  }
  print_file(arg, argc, argv);

  return 0;
}