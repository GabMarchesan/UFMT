// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Int main
int main(){
    int fd[2];
    int pid;

    if (pipe(fd) == -1){
        perror("pipe");
        exit(1);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0){
        char buffer[20];
        close(fd[1]); // Fecha o lado de escrita do pipe
        // lê o pipe
        read(fd[0], buffer, sizeof(buffer));
        printf("Filho leu: %s\n", buffer);
        // Fecha o lado de leitura do pipe
        close(fd[0]);
    }
    else {
        char *msg = "Oi, Filhão!";
        close(fd[0]); // Fecha o lado de leitura do pipe

        //Escreve no pipe
        write(fd[1], msg, strlen(msg) + 1);
        // Fecha o lado de escrita do pipe
        close(fd[1]);
        wait(NULL);

    }
    return 0;
}