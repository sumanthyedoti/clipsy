#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define INPUT_BUFFER_SIZE 2048

static char input[INPUT_BUFFER_SIZE];

static bool is_running() {
  if (strcmp(input, ":q\n") == 0 || strcmp(input, ":quit\n") == 0) {
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {

  /** Print Version and Exit information */
  puts("Clispy Version 0.0.1");
  puts("To Exit, press Ctrl+c or type :quit/:q\n");
  printf("%hd\n", is_running());
  /** REPL */
  while (true) {
    /** Output the promt */
    fputs("clispy> ", stdout);

    /** Read a line of user input */
    fgets(input, INPUT_BUFFER_SIZE, stdin);

    if (!is_running())
      break;

    printf("%s", input);
  }
  return 0;
}
