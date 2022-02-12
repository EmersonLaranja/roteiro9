#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
  int fd[2];

  if (pipe(fd) == -1)
  {
    perror("erro ao criar pipe");
    exit(1);
  }

  int num1, num2;
  int id1 = -1, id2 = -1;

  id1 = fork();

  if (id1 > 0)
  {
    id2 = fork();
  }

  if (id1 == 0 && id2 == -1)
  {
    close(fd[1]);
    read(fd[0], &num1, sizeof(int));
    close(fd[0]);
    printf("O primeiro numero foi: %d\n", num1);
  }

  if (id2 == 0)
  {
    close(fd[1]);
    read(fd[0], &num2, sizeof(int));
    close(fd[0]);
    printf("O segundo numero foi: %d\n", num2);
  }

  else if (id1 > 0 && id2 > 0) // pai
  {
    close(fd[0]);

    printf("Digite o numero 1 e o numero 2: ");
    scanf("%d", &num1);
    scanf("%d", &num2);
    write(fd[1], &num1, sizeof(num1));
    write(fd[1], &num2, sizeof(num2));

    close(fd[1]);
  }

  return 0;
}