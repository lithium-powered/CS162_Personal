#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#include "tokenizer.h"

/* Whether the shell is connected to an actual terminal or not. */
bool shell_is_interactive;

/* File descriptor for the shell input */
int shell_terminal;

/* Terminal mode settings for the shell */
struct termios shell_tmodes;

/* Process group id for the shell */
pid_t shell_pgid;

/* Current directory for the shell */
char dir[4096];

int cmd_exit(struct tokens *tokens);
int cmd_help(struct tokens *tokens);
int cmd_pwd(struct tokens *tokens);
int cmd_cd(struct tokens *tokens);

/* Built-in command functions take token array (see parse.h) and return int */
typedef int cmd_fun_t(struct tokens *tokens);

/* Built-in command struct and lookup table */
typedef struct fun_desc {
  cmd_fun_t *fun;
  char *cmd;
  char *doc;
} fun_desc_t;

fun_desc_t cmd_table[] = {
  {cmd_help, "?", "show this help menu"},
  {cmd_exit, "exit", "exit the command shell"},
  {cmd_pwd, "pwd", "print current working directory"},
  {cmd_cd, "cd", "change current directory"}
};

/* Prints a helpful description for the given command */
int cmd_help(struct tokens *tokens) {
  for (int i = 0; i < sizeof(cmd_table) / sizeof(fun_desc_t); i++)
    printf("%s - %s\n", cmd_table[i].cmd, cmd_table[i].doc);
  return 1;
}

/* Exits this shell */
int cmd_exit(struct tokens *tokens) {
  exit(0);
}

/* Print current working directory */
int cmd_pwd(struct tokens *tokens) {
  printf("%s\n", dir);
  return 1;
}

/* Change current directory */
int cmd_cd(struct tokens *tokens) {
  char *word = tokens_get_token(tokens, 1);
  strcpy(dir, word);
  return 1;
}

/* Looks up the built-in command, if it exists. */
int lookup(char cmd[]) {
  for (int i = 0; i < sizeof(cmd_table) / sizeof(fun_desc_t); i++)
    if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0))
      return i;
  return -1;
}

/* Intialization procedures for this shell */
void init_shell() {
  /* Our shell is connected to standard input. */
  shell_terminal = STDIN_FILENO;

  /* Check if we are running interactively */
  shell_is_interactive = isatty(shell_terminal);

  if (shell_is_interactive) {
    /* If the shell is not currently in the foreground, we must pause the shell until it becomes a
     * foreground process. We use SIGTTIN to pause the shell. When the shell gets moved to the
     * foreground, we'll receive a SIGCONT. */
    while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
      kill(-shell_pgid, SIGTTIN);

    /* Saves the shell's process id */
    shell_pgid = getpid();

    /* Take control of the terminal */
    tcsetpgrp(shell_terminal, shell_pgid);

    /* Save the current termios to a variable, so it can be restored later. */
    tcgetattr(shell_terminal, &shell_tmodes);
  }
}

int main(int argc, char *argv[]) {
  init_shell();

  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
  signal(SIGKILL, SIG_IGN);
  signal(SIGTERM, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);
  signal(SIGCONT, SIG_IGN);
  signal(SIGTTIN, SIG_IGN);
  signal(SIGTTOU, SIG_IGN);

  static char line[4096];
  int line_num = 0;

  /* Please only print shell prompts when standard input is not a tty */
  if (shell_is_interactive)
    fprintf(stdout, "%d: ", line_num);

  while (fgets(line, 4096, stdin)) {
    /* Split our line into words. */
    struct tokens *tokens = tokenize(line);
    /* Find which built-in function to run. */
    int fundex = lookup(tokens_get_token(tokens, 0));

    if (fundex >= 0) {
      cmd_table[fundex].fun(tokens);
    } else {
      /* REPLACE this to run commands as programs. */
      pid_t pid = fork();
      if (pid == 0){
        pid = getpid();
        setpgid(pid,pid);
        tcsetpgrp(0, pid);
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGKILL, SIG_DFL);
        signal(SIGTERM, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGCONT, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        int numTokens = tokens_get_length(tokens);
        char *args[numTokens+1];
        int i;
        for(i = 0; i < numTokens; i++){
          if (!strcmp("<", tokens_get_token(tokens, i))){
            freopen (tokens_get_token(tokens, i+1),"r",stdin);
            args[i] = NULL;
            break;
          } else if (!strcmp(">", tokens_get_token(tokens, i))){
            freopen (tokens_get_token(tokens, i+1),"w",stdout);
            args[i] = NULL;
            break;
          }
          args[i] = tokens_get_token(tokens, i);
        }
        args[numTokens] = NULL;
        char *firstArg = tokens_get_token(tokens, 0);

        if (strchr(firstArg, '/') == NULL){
          char *paths;
          char *s = getenv("PATH");
          const char delim[2] = ":";
          const char delim2[2] = "/";
          paths = strtok(s, delim);
          char pathHolder[4096];
          while (paths != NULL){
            strcpy(pathHolder,  paths);
            strcat(pathHolder, delim2);
            strcat(pathHolder, firstArg);
            args[0] = pathHolder;
            if (execv(pathHolder, args) != -1){
              break;
            }
            paths = strtok(NULL, delim);
          }
        }else{
          execv(firstArg, args);
        }
        exit(0);
      }else{
        int status;
        wait(&status);
        tcsetpgrp(0, getpid());
      }
    }

    if (shell_is_interactive)
      /* Please only print shell prompts when standard input is not a tty */
      fprintf(stdout, "%d: ", ++line_num);

    /* Clean up memory */
    tokens_destroy(tokens);
  }

  return 0;
}
