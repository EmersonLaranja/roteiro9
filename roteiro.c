#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

int main(){
    int pipe1[2];
    int pipe2[2];
    int pipe3[2];
    int x, y;
    char msg[200]="Meu filho, crie e envie para o seu irmão P3 um array de tamanho randômico entre 100 e 200, preenchido completamente com o valor ";
    char msg2[200]="Meu filho, crie e envie para o seu irmão P3 um array de tamanho randômico entre 100 e 200, preenchido completamente com o valor ";
    
    if (pipe(pipe1) < 0){ //cria o pipe1
        perror("Error pipe");
        exit(1);
    }  
    if (pipe(pipe2) < 0){ //cria o pipe2
        perror("Error pipe");
        exit(1);
    }    
    if (pipe(pipe3) < 0){ //cria o pipe3
        perror("Error pipe");
        exit(1);
    }  


    int n1 = fork();
    int n2 = fork();
  
    if (n1 > 0 && n2 > 0) {
        printf("Pai\n");

        srand(time(NULL));

        printf("Digite o numero entre 1-5: \n");
        scanf("%d", &x);
        printf("Digite o numero entre 6-10: \n");
        scanf("%d", &y);

 


        char converte = x + '0';
        strcat(msg,&converte);

        char converte2 = y + '0';
        strcat(msg2,&converte2);

        //escreve no tubo para o filho 1
        close(pipe1[0]); 
        write(pipe1[1], &x, sizeof(x));//manda o numero
        write(pipe1[1], msg, sizeof(msg));//manda a mensagem
        close(pipe1[1]);


        //escreve no tubo para o filho 2
        close(pipe2[0]);
        write(pipe2[1], &y, sizeof(y));
        write(pipe2[1], msg2, sizeof(msg2));
        close(pipe2[1]);
        
    }
    
    
    else if (n1 == 0 && n2 > 0){
        //! Filho 1
        char frase1[200]={0};
        int buf;    

        close(pipe1[1]);//fecha para descritor n utilizado

        if (read(pipe1[0], &buf, sizeof(int)) < 0){ /* leitura na saida do tubo*/
            perror("Error: reading message");
        } 
        printf("O numero recebido pelo processo 1: %d\n", buf);
        
        if (read(pipe1[0], frase1, sizeof(frase1)) < 0){ /* leitura na saida do tubo*/
            perror("Error: reading message");
        } 
        printf("A mensagem recebida pelo processo 1: %s\n", frase1);
        close(pipe1[0]);


        //gerando numero aleatorio
        srand(time(NULL));
        int numberRandom = rand() % 100 + 100;
        int vec1[numberRandom];

        //criando vetor de numero aleatorio
        for (int i = 0; i < numberRandom; i++) {
            vec1[i] = buf;
        }


        //escreve no tubo para o filho 3    
        close(pipe3[0]);
        write(pipe3[1], &numberRandom, sizeof(numberRandom));
        write(pipe3[1], vec1, sizeof(vec1));
        close(pipe3[1]);
    }
    
    else if (n1 > 0 && n2 == 0){
        //! Filho 2

        char frase2[200]={0};
        int buf2;

        close(pipe2[1]);//fecha para descritor n utilizado

        if (read(pipe2[0], &buf2, sizeof(int)) < 0){ /* leitura na saida do tubo*/
            perror("Error: reading message");
        }
        printf("O numero recebido pelo processo 2: %d\n", buf2);

        if (read(pipe2[0], frase2, sizeof(frase2)) < 0){ /* leitura na saida do tubo*/
            perror("Error: reading message");
        }
        printf("A mensagem do processo 2: %s\n", frase2);

        close(pipe2[0]);

        //gerando numero aleatorio
        srand(time(NULL));
        int numberRandom2 = rand() % 100 + 100;
        int vec2[numberRandom2]; //vetor de numero aleatorio

        //criando vetor de numero aleatorio
        for (int i = 0; i < numberRandom2; i++) {
            vec2[i] = buf2;
        }

        //! esperar o outro irmão escrever primeiro e depois escreve
        sleep(2);

        close(pipe3[0]);
        write(pipe3[1], &numberRandom2, sizeof(numberRandom2));
        write(pipe3[1], vec2, sizeof(vec2));
        close(pipe3[1]);

    }


    else {
        //! Filho 3
        int buf3;

        //* Vai ler primeiro os dados enviados pelo processo 1 e depois pelo processo 2
        close(pipe3[1]);//fecha para descritor n utilizado

        if (read(pipe3[0], &buf3, sizeof(int)) < 0){ /* leitura na saida do tubo*/
            perror("Error: reading message");
        }

        //pega o tamanho do vetor aleatorio
        int vec3[buf3];
        for(int i = 0; i < buf3; i++){ //limpa o vetor de qualquer lixo
            vec3[i] = 0;
        }

        //faz a leitura do vetor 
        if (read(pipe3[0], vec3, sizeof(vec3)) < 0){ /* leitura na saida do tubo*/
            perror("Error: reading message");
        }
        printf("A Primeira mensagem recebida pelo processo 3: ");
        for (int i = 0; i < buf3; i++) {
            printf("%d", vec3[i]);
        }
        printf("\n");
      
        


        //* Agora vai ler o vetor do processo 2
        close(pipe3[1]);//fecha para descritor n utilizado

        if (read(pipe3[0], &buf3, sizeof(int)) < 0){ /* leitura na saida do tubo*/
            perror("Error: reading message");
        }

        //pega o tamanho do vetor aleatorio
        int vec4[buf3];
        for(int i = 0; i < buf3; i++){//limpa o vetor de qualquer lixo
            vec4[i] = 0;
        }

        if (read(pipe3[0], vec4, sizeof(vec4)) < 0){ /* leitura na saida do tubo*/
            perror("Error: reading message");
        }
        printf("A Segunda mensagem recebida pelo processo 3: ");
        for (int i = 0; i < buf3; i++) {
            printf("%d", vec4[i]);
        }
        printf("\n");
        close(pipe3[0]);
    }
    return 0;
}
