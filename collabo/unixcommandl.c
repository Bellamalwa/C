#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*main function starts by displaying a prompt and waiting for the user to type a command.
*/
int main()
{
  char *prompt = "#";
  char *command;

  while (1)
  {
    printf("%s ", prompt);
    fflush(stdout);
    command = fgets(prompt, 100, stdin);

    if (command == NULL)
    {
      break;
    }

    if (command[strlen(command) - 1] == '\n')
    {
      command[strlen(command) - 1] = '\0';
    }

    if (strcmp(command, "exit") == 0)
    {
      break;
    }

    pid_t pid = fork();

    if (pid == 0)
    {
      execl(command, command, NULL);
      perror("execl");
      exit(1);
    } else if (pid < 0)
    {
      perror("fork");
      exit(1);
    }

    waitpid(pid, NULL, 0);
  }

  return 0;
}
/*main function continues to loop until the user enters the "exit" command.*/