
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid;
    int n, count;
    int SIZE = 4096;
    
    int fd = shm_open("/collatz_shared_memory", O_CREAT | O_RDWR, S_IRWXU);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }
    
    if (ftruncate(fd, SIZE) == -1) {
        perror("ftruncate");
        return 1;
    }
    
    void *ptr = mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    
    printf("Digite um número inteiro positivo: ");
    scanf("%d", &n);
    count = 0;
    
    pid = fork();
    
    if (pid == 0) {
        while (n != 1) {
            printf("%d ", n);
            if (n % 2 == 0) {
                n = n / 2;
            } else {
                n = 3 * n + 1;
            }
            count += sprintf(ptr, "%d ", n);
            ptr += count;
        }
    } else if (pid > 0) {
        wait(NULL);
        printf("\nSequência de Collatz calculada pelo filho: ");
        printf("%s\n", (char *)ptr);
        
        munmap(ptr, SIZE);
        close(fd);
        shm_unlink("/collatz_shared_memory");
    } else {
        perror("fork");
        return 1;
    }
    
    return 0;
}