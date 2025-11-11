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

    // Abre memória compartilhada

    int shm_fd = shm_open(nome, O_RDONLY, 0666);
    if(shm_fd == -1){
        perror("Erro ao abrir mem compartilhada");
        exit(1);
    }

    // mapeia a memória compartilhada
    void *ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    // Lê a memória compartilhada
    printf("%s\n", (char *) ptr);
    // Desfaz o compartilhamento da memória
    shm_unlink(nome);
    return 0;
}