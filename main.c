/*Usando pipes como mecanismo de comunicação entre processos, crie um programa que execute o seguinte procedimento:
- O processo pai, P0, lê do teclado um valor X inteiro entre 1-5.
- O processo pai, P0, lê do teclado um segundo valor Y inteiro entre 6 e 10.
- P0 cria três filhos, P1, P2 e P3.
- P0 envia para o filho P1 via pipe (pipe1) o valor de X.
- P0 envia para o filho P2 via pipe (pipe2) o valor de Y.
- P0 envia para P1, pelo mesmo pipe1, a seguinte mensagem (string): "Meu filho, crie e envie para o seu irmão P3 um array de tamanho randômico entre 100 e 200, preenchido completamente com o valor X" (obs: ele envia o valor de X e não a letra 'X').

- P0 envia para P2, pelo mesmo pipe2, a seguinte mensagem (string): "Meu filho, crie e envie para o seu irmão P3 um array de tamanho randômico entre 100 e 200, preenchido completamente com o valor Y" (obs: ele envia o valor de Y e não a letra 'Y').

- P1 e P2 recebem e imprimem a mensagem enviada pelo pai (identifique se é P1 ou P2 quem printa cada mensagem).

- P1 e P2 criam seus arrays de tamanho randômico, entre 100 e 200, preenchendo-os com valores X (no caso de P1) e Y (no caso de P2), conforme solicitado pelo pai.
- P1 e P2 enviam os arrays para o irmão P3, via pipe3 (sim... eles usarão o mesmo pipe!! mas cuidado! as escritas podem se embaralhar.... resolvam isso!).
- P3 lê os dois arrays enviados e printa os arrays na saída padrão. */
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
  FILE *fp;
  int pid;
  int pipefds1[2];
  int pipefds2[2];
  char *username;
  int segundo=-1;
  /* da o nome do usuario */

  /* Cria um tubo. Isto deve ser feito antes do fork para que
   * o filho possa herdar esse tubo
   */
  int x, y;

  scanf("%d", &x);
  scanf("%d", &y);

  if (pipe(pipefds1) < 0){
    perror("Error pipe");
    exit(1);
  }

  if (pipe(pipefds2) < 0)
  {
    perror("Error pipe");
    exit(1);
  }

  pid = fork();

  if (pid == 0){
    printf("\nSou filho 1\n");
    close(0); // fecha a entrada padrao
    close(pipefds1[1]);//fecha para escrita

    int buf;
    char letras[150];
 
    if (read(pipefds1[0], &buf, sizeof(int)) < 0) /* leitura na saida do tubo*/
      perror("Error: reading message");
    printf("Lido do tubo 1-->%c\n", buf);
    
     
    if (read(pipefds1[0], letras, 150) < 0) /* leitura na saida do tubo*/
      perror("Error: reading message");
      printf("Frase do tubo 1-->%s\n", letras);
    
    close(pipefds1[0]);
  }

  if (pid > 0){
     segundo = fork(); // segundo filho

    if (segundo == 0){
      printf("\nSou filho 2\n");
      close(0); // fecha a entrada padrao
      close(pipefds2[1]);

      int buf2;
      char frase[100];
 

      if (read(pipefds2[0], &buf2, sizeof(int)) < 0) /* leitura na saida do tubo*/
        perror("Error: reading message");
        printf("Lido do tubo 2-->%c\n", buf2);

      close(pipefds1[0]);
    }
  }

  if (segundo > 0){
    int terceiro = fork(); // terceiro filho
    if (terceiro == 0)
      printf("\nSou filho 3\n");
  }

  /* Codigo do filho:
   * executa o comando mail e entao envia ao username
   * a mensagem contida no tubo */
  if (pid == 0){
    // /*      redirige a stdin para o tubo; o comando executado em seguida tera
    //     como entrada (uma mensagem) a leitura do tubo */
    //     close(0);
    //     dup(pipefds[0]);
    //     close(pipefds[0]);
    //     /* fecha o lado de escrita do tubo, para poder ver a saida na tela */
    //     close(pipefds[1]);
    //     /* executa o comando mail */
    //     execl("/bin/mail", "mail", username, 0);
    //     perror("Error execl");
    //     exit(1);
  }

  if (pid > 0){
    char msg1[150] = "Meu filho, crie e envie para o seu irmão P3 um array de tamanho randômico entre 100 e 200, preenchido completamente com o valor X";

    /* Codigo do pai:
     * escreve uma mensagem no tubo 1*/
    close(pipefds1[0]);
    fp = fdopen(pipefds1[1], "w");
    fprintf(fp, "%d", x);
    fprintf(fp,"%s" ,msg1);
    fclose(fp);

    /* escreve uma mensagem no tubo 2*/
    close(pipefds2[0]);
    fp = fdopen(pipefds2[1], "w");
    fprintf(fp, "%d", y);
    fclose(fp);

    /* Espera da morte do processo filho */
    while (wait((int *)0) != pid);

    exit(0);
  }
}

//! cria 3 filhos
//! se comunica com 2 filhos
