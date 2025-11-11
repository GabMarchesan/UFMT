#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define DELIM " \t\r\n\a"

// Variável CAMINHO
char *CAMINHO = "./";

// Função para mudar de diretório (cd)
void shell_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "shell: esperado argumento para \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("shell");
        }
    }
}

// Função para imprimir o diretório atual (pwd)
void shell_pwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("shell");
    }
}

// Função para manipular a variável CAMINHO
void shell_set_caminho(char *path) {
    CAMINHO = strdup(path);
}

// Função para verificar e exibir CAMINHO
void shell_show_caminho() {
    printf("%s\n", CAMINHO);
}

// Função para executar programas externos
int shell_exec(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Processo filho: tenta executar o comando
        if (execvp(args[0], args) == -1) {
            perror("shell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Erro no fork
        perror("shell");
    } else {
        // Processo pai: espera o filho terminar
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

// Função para execução de comandos com pipe
void shell_pipe(char **args1, char **args2) {
    int pipefd[2];
    pid_t pid1, pid2;

    if (pipe(pipefd) == -1) {
        perror("shell");
        return;
    }

    pid1 = fork();
    if (pid1 == 0) {
        // Primeiro processo filho (comando 1)
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        if (execvp(args1[0], args1) == -1) {
            perror("shell");
            exit(EXIT_FAILURE);
        }
    }

    pid2 = fork();
    if (pid2 == 0) {
        // Segundo processo filho (comando 2)
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);
        if (execvp(args2[0], args2) == -1) {
            perror("shell");
            exit(EXIT_FAILURE);
        }
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

// Função para checar e compilar programas C automaticamente
void shell_check_and_compile(char *progname) {
    char source_file[256];
    snprintf(source_file, sizeof(source_file), "%s.c", progname);

    struct stat source_stat, binary_stat;
    int binary_exists = stat(progname, &binary_stat);
    int source_exists = stat(source_file, &source_stat);

    if (source_exists == -1) {
        // Arquivo .c não encontrado
        if (binary_exists == -1) {
            fprintf(stderr, "shell: programa não encontrado\n");
        } else {
            shell_exec((char *[]) {progname, NULL});
        }
        return;
    }

    if (binary_exists == -1 || difftime(source_stat.st_mtime, binary_stat.st_mtime) > 0) {
        // Compilar o programa
        printf("Compilando %s...\n", source_file);
        if (fork() == 0) {
            execlp("gcc", "gcc", source_file, "-o", progname, NULL);
            perror("shell");
            exit(EXIT_FAILURE);
        }
        wait(NULL);
    }

    shell_exec((char *[]) {progname, NULL});
}

// Função para dividir a linha em argumentos
char **shell_split_line(char *line) {
    int bufsize = MAX_ARGS, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "shell: erro de alocação\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += MAX_ARGS;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "shell: erro de alocação\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

// Função para tratar o input com pipes
int shell_handle_pipe(char *line) {
    char *cmd1 = strtok(line, "|");
    char *cmd2 = strtok(NULL, "|");

    if (cmd2 == NULL) {
        return 0; // Não há pipe
    }

    char **args1 = shell_split_line(cmd1);
    char **args2 = shell_split_line(cmd2);
    shell_pipe(args1, args2);
    
    free(args1);
    free(args2);
    return 1; // Pipe tratado
}

// Função principal do loop do shell
void shell_loop() {
    char *line = NULL;
    char **args;
    int status = 1;
    size_t bufsize = 0;

    do {
        printf("> ");
        getline(&line, &bufsize, stdin);
        line[strlen(line) - 1] = '\0'; // Remover o '\n'

        // Verificar se há pipe
        if (shell_handle_pipe(line)) {
            continue;
        }

        args = shell_split_line(line);

        // Comando interno ou externo
        if (args[0] == NULL) {
            // Comando vazio
            continue;
        }

        if (strcmp(args[0], "cd") == 0) {
            shell_cd(args);
        } else if (strcmp(args[0], "pwd") == 0) {
            shell_pwd();
        } else if (strcmp(args[0], "CAMINHO") == 0) {
            if (args[1] != NULL) {
                shell_set_caminho(args[1]);
            } else {
                shell_show_caminho();
            }
        } else if (access(args[0], F_OK) == -1) {
            // Tenta tratar como programa C
            shell_check_and_compile(args[0]);
        } else {
            shell_exec(args);
        }

    } while (status);

    free(line);
}

int main(int argc, char **argv) {
    // Inicia o loop do shell
    shell_loop();
    return EXIT_SUCCESS;
}
