#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SHELL_TOK_BUFSIZE 64
#define SHELL_TOK_DELIM " \t\r\n\a"

//Built in COMMANDS
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);

char *builtin_str[] = {
  "cd",
  "myhelp",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &shell_cd,
  &shell_help,
  &shell_exit
};

int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

//Built in functons 
int shell_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "Error: Argument expected for \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("LinuxShell");
    }
  }
  return 1;
}

int shell_help(char **args)
{
  int i;
  printf("\t*********************************************\n");
  printf("\t Welcome to Vedant and Tanvi's LinuxShell\n");
  printf("\t*********************************************\n");
  printf("Built in functions in this shell are:\n");

  for (i = 0; i < lsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("You can use the man command for the manual\n");
  return 1;
}

int shell_exit(char **args)
{
  return 0;
}


char *shell_read_line(void)
{
  char *line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us
  getline(&line, &bufsize, stdin);
  return line;
}

char **shell_split_line(char *line)
{
  int bufsize = SHELL_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "LinuxShell: Allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, SHELL_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += SHELL_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "LinuxShell: Allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, SHELL_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

int shell_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {

    if (execvp(args[0], args) == -1) {
      perror("LinuxShell");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
   
    perror("LinuxShell");
  } else {

    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int shell_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    return 1;
  }

  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return shell_launch(args);
}
void shell_loop(void)
{
  char *line;
  char **args;
  int exit_status;

  do {
    printf(">>> ");
    line = shell_read_line();
    args = shell_split_line(line);
    exit_status = shell_execute(args);

    free(line);
    free(args);
  } while (exit_status);
}

int main(int argc, char **argv)
{
  shell_loop();
  return EXIT_SUCCESS;
}