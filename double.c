#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define PROGNAME "double"
#define ARRAYSTART 2000

void mem_failure(void) {
  fprintf(stderr, PROGNAME ": failed to allocate memory\n");
  exit(1);
}

int main(int argc, char *argv[]) {

  static char buf[BUFSIZ];
  if (setvbuf(stdout, buf, _IOFBF, BUFSIZ) != 0) {
    fprintf(stderr, "setvbuf\n");
    exit(1);
  }

  FILE *f;
  char *line = NULL;
  char **lines;
  char *line_ptr;
  size_t used = 0, size = ARRAYSTART;
  size_t line_size = 0;

  // Establish input from file or stdin
  if (argc > 1) {
    f = fopen(argv[1], "r");
    if (f == NULL) {
      fprintf(stderr, PROGNAME ": unable to open input file\n");
      exit(1);
    }
  } else {
    f = stdin;
  }

  // advice posix on access patterns
#if defined(__linux__)
  int err;
  if ((err = posix_fadvise(fileno(f), 0, 0, POSIX_FADV_SEQUENTIAL)) < 0) {
    fprintf(stderr, "fadvise: %d\n", err);
    exit(1);
  }
  if ((err = posix_fadvise(fileno(stdout), 0, 0, POSIX_FADV_NOREUSE)) < 0) {
    fprintf(stderr, "fadvise: %d\n", err);
    exit(1);
  }
#endif
  // allocate array
  lines = malloc(ARRAYSTART * sizeof(uintptr_t));
  if (lines == NULL) {
    mem_failure();
  }

  // Read inputs into array
  while (getline(&line, &line_size, f) != -1) {
    line[strcspn(line, "\n")] = 0;
    line_ptr = malloc(line_size * sizeof(char) + 1);
    if (line_ptr == NULL) {
      mem_failure();
    }
    strcpy(line_ptr, line);
    if (used == size) {
      lines = (char **)realloc(lines, ((size *= 2) * (sizeof(char))));
      if (lines == NULL) {
        mem_failure();
      }
    }
    lines[used++] = line_ptr;
  }
  fclose(f);

  for (int i = 0; i < used; i++) {
    for (int j = 0; j < used; j++) {
#if defined(__linux__)
      fputs_unlocked(lines[i], stdout);
      fputs_unlocked(lines[j], stdout);
      fputc_unlocked('\n', stdout);
      fputs_unlocked(lines[i], stdout);
      fputc_unlocked(' ', stdout);
      fputs_unlocked(lines[j], stdout);
      fputc_unlocked('\n', stdout);
#else
      fputs(lines[i], stdout);
      fputs(lines[j], stdout);
      fputc('\n', stdout);
      fputs(lines[i], stdout);
      fputc(' ', stdout);
      fputs(lines[j], stdout);
      fputc('\n', stdout);
#endif
    }
  }
}
