#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./modules/mpc.h"

/** #### If compiling on Windows, compile these functions */
#ifdef _WIN32
#define INPUT_BUFFER_SIZE 2048
static char buffer[INPUT_BUFFER_SIZE];

/** Fake readline function */
char *readline(char *prompt) {
  fputs(prompt, stdout);
  fgets(buffer, INPUT_BUFFER_SIZE, stdin);
  char *cpy = malloc(strlen(buffer) + 1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy) - 1] = '\0';
  return cpy;
}

/** Fake address function */
void add_history(char *unused) {}
/** #### Otherwise include editline headers */
#else
#include <editline/history.h>
#include <editline/readline.h>
#endif

static char *input;

static bool is_running(void);

int main(int argc, char *argv[]) {

  /* Create Some Parsers */
  mpc_parser_t *Number = mpc_new("number");
  mpc_parser_t *Operator = mpc_new("operator");
  mpc_parser_t *Expr = mpc_new("expr");
  mpc_parser_t *Clispy = mpc_new("clispy");

  /* Define them with the following Language */
  mpca_lang(MPCA_LANG_DEFAULT, "\
    number   : /(-?[0-9]+)(.[0-9]+)*/ ;                                     \
    operator : '+' | '-' | '*' | '/' | '%';                          \
    expr     : <number> | '(' <operator> <expr> <expr>+ ')' ;  \
    clispy    : /^/ <expr> /$/ ; \
  ",
            Number, Operator, Expr, Clispy);

  /** Print Version and Exit information */
  puts("Clispy Version 0.0.1");
  puts("To Exit, press Ctrl+c or type :quit/:q\n");

  /** REPL */
  while (true) {
    /** Output the promt and get input */
    input = readline("clispy> ");

    if (strlen(input) > 0)
      add_history(input);

    if (!is_running())
      break;

    /* Attempt to Parse the user Input */
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Clispy, &r)) {
      /* On Success Print the AST */
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      /* Otherwise Print the Error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    free(input);
  }
  mpc_cleanup(4, Number, Operator, Expr, Clispy);
  return 0;
}

static bool is_running() {
  if (strcmp(input, ":q") == 0 || strcmp(input, ":quit") == 0) {
    return false;
  }
  return true;
}
