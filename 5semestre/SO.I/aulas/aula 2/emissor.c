// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

// Memória compartilhada

int main() {
    const char *nome = "minha_mem_compartilhada";
    const int SIZE = 4096;
    char *msg = "olá mundo";

    // Cria a memória copartilhada
    int shm_fd = shm_open(nome, O_CREAT | O_RDWR, 0666);
    if(shm_fd == -1){
        perror("Erro ao criar mem compartilhada");
        exit(1);
    }

    // Configura o tamanho da memória compartilhada
    ftruncate(shm_fd, SIZE);
    // mapeia a memória compartilhada
    void *ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    sprintf(ptr, "%s",msg);
    return 0;
}