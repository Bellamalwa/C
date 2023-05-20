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

    int j = 0;
    for (i = 0; command[i] != '\0'; i++) {
      if (command[i] == ';') {
        if (args[j] != NULL) {
          // Execute the current command.
          for (int k = 0; path[k] != '\0'; k++) {
            char *full_path = malloc(sizeof(char) * (strlen(path) + strlen(args[j]) + 2));
            strcpy(full_path, path);
            strcat(full_path, "/");
            strcat(full_path, args[j]);

            if (access(full_path, F_OK) == 0) {
              pid_t pid = fork();

              if (pid == 0) {
                // Replace variables in the command.
                for (int l = 0; l < strlen(command); l++) {
                  if (command[l] == '$' && command[l + 1] == '{') {
                    int end = l + 1;
                    while (command[end] != '}') {
                      end++;
                    }
                    char *var_name = malloc(sizeof(char) * (end - l));
                    strncpy(var_name, command + l + 1, end - l - 1);
                    var_name[end - l - 1] = '\0';
                    char *var_value = getenv(var_name);
                    if (var_value != NULL) {
                      // Replace the variable with its value.
                      for (int m = l; m < end; m++) {
                        command[m] = var_value[m - l];
                      }
                    }
                    free(var_name);
                  }
                }

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
          }
        }

        // Reset the arguments.
        args[j] = NULL;
      } else if (args[j] == NULL) {
        args[j] = command;
      } else 