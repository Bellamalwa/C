#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main() {
  char *prompt = "#";
  char *command;
  char **args;
  int i;
  char *path = getenv("PATH");

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

    for (i = 0; path[i] != '\0'; i++) {
      char *full_path = malloc(sizeof(char) * (strlen(path) + strlen(command) + 2));
      strcpy(full_path, path);
      strcat(full_path, "/");
      strcat(full_path, command);

      if (access(full_path, F_OK) == 0) {
        pid_t pid = fork();

        if (pid == 0) {
          execvp(full_path, args);
          perror("execvp");
          exit(1);
        } else if (pid < 0) {
          perror("fork");
          exit(1);
        }

        waitpid(pid, NULL, 0);

        free(full_path);
        break;
      }

      free(full_path);
    }

    if (i == strlen(path)) {
      if (strcmp(command, "exit") == 0) {
        exit(0);
      } else {
        printf("Command not found\n");
      }
    }
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