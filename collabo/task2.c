#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
  char *prompt = "#";
  char *command;
  char **args;
  int i;

  while (1) {
    printf("%s ", prompt);
    fflush(stdout);
    command = fgets(prompt, 100, stdin);

    if (command == NULL) {
      break;
    }

    if (command[strlen(command) - 1] == '\n') {
      command[strlen(command) - 1] = '\0';
    }

    if (strcmp(command, "exit") == 0) {
      break;
    }

    args = malloc(sizeof(char *) * (1 + countTokens(command)));

    for (i = 0; command[i] != '\0'; i++) {
      if (command[i] == ' ') {
        command[i] = '\0';
        args[i] = command + i + 1;
      }
    }

    args[i] = NULL;

    pid_t pid = fork();

    if (pid == 0) {
      execvp(args[0], args);
      perror("execvp");
      exit(1);
    } else if (pid < 0) {
      perror("fork");
      exit(1);
    }

    waitpid(pid, NULL, 0);
  }

  return 0;
}

int countTokens(char *str) {
  int count = 0;
  int i;

  for (i = 0; str[i] != '\0'; i++) {
    if (str[i] == ' ') {
      count++;
    }
  }

  return count + 1;
}