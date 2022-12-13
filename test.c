#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ARROW_UP 65
#define ARROW_DOWN 66
#define ARROW_RIGHT 67
#define ARROW_LEFT 68

char *find_history(char * prefix, int find_iteration){
    if (find_iteration == 0) return prefix;
    return "Test";
}

char *read_line(void)
{
  int bufsize = 1024;
  int length = 0;
  int cursor = 0;
  int find_iteration = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "lsh: allocation error\n");
  }

  while (1) {
    // Read a character
    c = getchar();

    if (c == 27){
        c = getchar();
        if (c == 91) {
            c = getchar();
            if (c == ARROW_UP) {find_iteration++; find_history(buffer, find_iteration);};
            if (c == ARROW_DOWN) {find_iteration--; find_history(buffer, find_iteration);};
            if (c == ARROW_LEFT) {if(cursor >0) cursor--;};
            if (c == ARROW_RIGHT) {if(cursor < length) cursor++;};
            continue;
        }
    }

    if (c == EOF || c == '\n') {
      buffer[cursor] = '\0';
      return buffer;
    } else {
      buffer[cursor] = c;
    }

    length++;
    cursor++;

    // If we have exceeded the buffer, reallocate.
    if (cursor >= bufsize) {
      bufsize += 1024;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
  return buffer;
}

int main(){
    printf("%s\n", read_line());
    return 0;
}