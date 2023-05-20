#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024

static char buffer[BUFFER_SIZE];
static int buffer_pos = 0;
static int buffer_size = 0;

char *getline(void) {
  char *line = NULL;
  int c;

  while ((c = getchar()) != '\n' && c != EOF) {
    if (buffer_pos == buffer_size) {
      buffer_size *= 2;
      char *new_buffer = realloc(buffer, buffer_size);
      if (new_buffer == NULL) {
        return NULL;
      }
      buffer = new_buffer;
    }
    buffer[buffer_pos++] = c;
  }

  if (buffer_pos > 0) {
    buffer[buffer_pos - 1] = '\0';
    line = buffer;
  }

  return line;
}

int main() {
  char *prompt = "#";
  char *command;
  char **args;
  int i;
  char *path = getenv("PATH");

  while (1) {
    printf("%s ", prompt);
    fflush(stdout);
    command = getline();

    if (command == NULL) {
      break;
    }

    if (command[strlen(command) - 1] == '\n') {
      command[strlen(command) - 1] = '\0';
    }

    if (strcmp(command, "exit") == 0) {
      break;
    } else if (strcmp(command, "env") == 0) {
      for (i = 0; environ[i] != NULL; i++) {
        printf("%s\n", environ[i]);
      }
    } else {
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
