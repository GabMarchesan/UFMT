//Alunos:
// Gabriel Gomes Marchesan (202111722011)
// Lui Grosz (202111722018)

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define BLOCK_SIZE 4096  // bytes
#define INODE_BLOCKS 13  // 16 - 3 (superbloco e bitmaps)
#define INODE_SIZE 128   // Tamanho de cada i-node em bytes
#define MAX_INODES 416   // Numero máximo de i-nodes
#define NUM_ARQUIVOS 15

typedef struct {
    int tamanho_disco;  // megabytes
    int onde_comeca_inodes;
    int onde_comeca_dados;
    int num_inodes;
    int num_blocos_dados;
    int blocos_livres;
    int inodes_livres;
} Superbloco;

typedef struct {
    int id;
    int tipo;      // 1 - dir 0 - file
    int tamanho;   // tamanho do arquivo ou dir (em bloco)
    time_t time;   // hora criado
    time_t mtime;  // hora modificado
} Inode;

typedef struct {
    int inum;
    int reclen;
    int strlen;
    char nome[255];
} Arquivo;

typedef struct {
    int inode;
    Arquivo arquivos[NUM_ARQUIVOS];
} Diretorio;

void formatar_disco(int tamanho_disco) {
    int fd = open("disco", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("Erro ao criar disco virtual");
        exit(1);
    }

    int tamanho_disco_bytes = tamanho_disco * 1024 * 1024;

    Superbloco sb;
    sb.onde_comeca_inodes = 3;
    sb.tamanho_disco = tamanho_disco_bytes;
    sb.onde_comeca_dados = 16;
    ftruncate(fd, tamanho_disco_bytes);
    sb.num_inodes = INODE_BLOCKS;

    sb.num_blocos_dados =
        (tamanho_disco_bytes - BLOCK_SIZE * (INODE_BLOCKS + 3)) / BLOCK_SIZE;

    sb.blocos_livres = sb.num_blocos_dados;
    sb.inodes_livres = MAX_INODES;

    write(fd, &sb, sizeof(Superbloco));

    char bitmap_blocos[sb.num_blocos_dados];
    memset(bitmap_blocos, 0, sb.num_blocos_dados);
    char bitmap_inodes[MAX_INODES];
    memset(bitmap_inodes, 0, MAX_INODES);

    lseek(fd, BLOCK_SIZE, SEEK_SET);
    write(fd, bitmap_inodes, MAX_INODES);

    lseek(fd, BLOCK_SIZE * 2, SEEK_SET);
    write(fd, bitmap_blocos, sb.num_blocos_dados);

    time_t tempo;
    time(&tempo);

    Inode novo_inode;
    novo_inode.id = 0;
    novo_inode.mtime = tempo;
    novo_inode.time = tempo;
    novo_inode.tamanho = 1;
    novo_inode.tipo = 1;

    lseek(fd, BLOCK_SIZE * 3, SEEK_SET);
    write(fd, &novo_inode, sizeof(Inode));

    Diretorio root;
    root.inode = novo_inode.id;
    for (int i = 0; i < sizeof(root.arquivos) / sizeof(root.arquivos[0]); i++) {
        root.arquivos[i].inum = -1;
    }

    sb.blocos_livres = sb.num_blocos_dados - 1;
    sb.inodes_livres = MAX_INODES - 1;
    lseek(fd, 0, SEEK_SET);
    write(fd, &sb, sizeof(Superbloco));

    lseek(fd, BLOCK_SIZE, SEEK_SET);
    bitmap_inodes[0] = 1;
    write(fd, bitmap_inodes, MAX_INODES);

    lseek(fd, BLOCK_SIZE * 2, SEEK_SET);
    bitmap_blocos[0] = 1;
    write(fd, bitmap_blocos, sb.num_blocos_dados);

    lseek(fd, BLOCK_SIZE * sb.onde_comeca_dados, SEEK_SET);
    write(fd, &root, sizeof(Diretorio));
    close(fd);
}

void consultar_estado_disco() {
    int fd = open("disco", O_RDONLY);
    if (fd < 0) {
        perror("Erro ao abrir o disco virtual");
        return;
    }

    Superbloco sb;
    read(fd, &sb, sizeof(Superbloco));

    int espaco_ocupado =
        ((sb.num_inodes + sb.onde_comeca_inodes) * BLOCK_SIZE) +
        ((sb.num_blocos_dados - sb.blocos_livres) * BLOCK_SIZE);

    int espaco_livre = sb.tamanho_disco - espaco_ocupado;

    printf("\nEstado do Disco:\n\n");
    printf("Tamanho do disco: %d bytes\n", sb.tamanho_disco);
    printf("Tamanho do bloco: %d bytes\n", BLOCK_SIZE);
    printf("Espaco ocupado: %d bytes\n", espaco_ocupado);
    printf("Espaco livre: %d bytes\n", espaco_livre);
    printf("Espaco ocupado com arquivos: %d bytes\n",
           (sb.num_blocos_dados - sb.blocos_livres) * BLOCK_SIZE);
    printf("Blocos livres: %d\n", sb.blocos_livres);
    printf("Inodes livres: %d\n", sb.inodes_livres);
    close(fd);
}

int encontrarInodeVazio() {
    int fd = open("disco", O_RDONLY);

    lseek(fd, BLOCK_SIZE, SEEK_SET);

    for (int i = 0; i < MAX_INODES; i++) {
        char status[1];
        read(fd, &status, 1);

        if (status[0] == 0) {
            close(fd);
            return i;
        }
    }
    close(fd);
    return -1;
}

int encontrarBlocoVazio() {
    int fd = open("disco", O_RDONLY);

    Superbloco sb;
    read(fd, &sb, sizeof(Superbloco));

    lseek(fd, BLOCK_SIZE * 2, SEEK_SET);

    for (int i = 0; i < sb.num_blocos_dados; i++) {
        char status[1];
        read(fd, &status, 1);

        if (status[0] == 0) {
            close(fd);
            return i;
        }
    }
    close(fd);
    return -1;
}

void criarDiretorio(const char *caminho_absoluto) {
    int fd = open("disco", O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("Erro ao abrir o disco virtual");
        return;
    }

    Superbloco sb;
    read(fd, &sb, sizeof(Superbloco));

    char barra = '/';
    if (caminho_absoluto[0] != barra) {
        close(fd);
        printf("Erro: Caminho invalido\n");
        return;
    }
    char caminho[255];
    strcpy(caminho, caminho_absoluto);
    char *lastToken = strrchr(caminho, '/');

    char *token = strtok(caminho, "/");

    int inode_atual = 0;
    int inode_encontrado = -1;
    Diretorio dir;
    Inode dir_inode;
    dir_inode.id = 0;

    while (strcmp(token, lastToken + 1) != 0) {
        lseek(fd, BLOCK_SIZE * 3, SEEK_SET);
        lseek(fd, INODE_SIZE * inode_atual, SEEK_CUR);

        read(fd, &dir_inode, sizeof(Inode));

        if (dir_inode.tipo != 1) {
            printf("Erro: Caminho inválido, '%s' não é um diretório.\n", token);
            close(fd);
            return;
        }

        lseek(fd, BLOCK_SIZE * (sb.onde_comeca_dados + dir_inode.id), SEEK_SET);
        read(fd, &dir, sizeof(Diretorio));

        for (int i = 0; i < NUM_ARQUIVOS; i++) {
            if (strcmp(dir.arquivos[i].nome, token) == 0) {
                inode_encontrado = dir.arquivos[i].inum;
                break;
            } else {
                inode_encontrado = -1;
            }
        }

        if (inode_encontrado == -1) {
            printf("ERRO: Diretorio %s nao encontrado.", token);
            close(fd);
            return;
        }

        inode_atual = inode_encontrado;
        token = strtok(NULL, "/");
    }

    int novo_inode_id = encontrarInodeVazio();
    int novo_bloco_id = encontrarBlocoVazio();

    if (novo_inode_id == -1 || novo_bloco_id == -1) {
        printf("Espaço insuficiente para criar diretório.\n");
        close(fd);
        return;
    }

    lseek(fd, BLOCK_SIZE * (sb.onde_comeca_dados + inode_atual), SEEK_SET);
    read(fd, &dir, sizeof(Diretorio));

    int flag = 0;
    for (int i = 0; i < NUM_ARQUIVOS; i++) {
        printf("novo_inode: %d, index: %d", novo_inode_id, i);
        if (dir.arquivos[i].inum == -1) {
            dir.arquivos[i].inum = novo_inode_id;
            dir.arquivos[i].reclen = sizeof(Diretorio);
            dir.arquivos[i].strlen = strlen(token);
            strcpy(dir.arquivos[i].nome, token);
            flag = 1;
            break;
        }
    }

    if (flag == 0) {
        printf("Diretorio pai cheio\n");
        close(fd);
    }

    Inode novo_inode;
    novo_inode.id = novo_inode_id;
    novo_inode.tipo = 1;
    novo_inode.tamanho = 1;

    time_t t;
    time(&t);
    novo_inode.time = t;
    novo_inode.mtime = t;

    lseek(fd, BLOCK_SIZE * sb.onde_comeca_inodes, SEEK_SET);
    lseek(fd, INODE_SIZE * novo_inode_id, SEEK_CUR);
    write(fd, &novo_inode, sizeof(Inode));

    lseek(fd, BLOCK_SIZE * (sb.onde_comeca_dados + inode_atual), SEEK_SET);
    write(fd, &dir, sizeof(Diretorio));

    Diretorio newDir;
    newDir.inode = novo_inode.id;
    for (int i = 0; i < sizeof(newDir.arquivos) / sizeof(newDir.arquivos[0]);
         i++) {
        newDir.arquivos[i].inum = -1;
    }

    lseek(fd, BLOCK_SIZE * (sb.onde_comeca_dados + novo_bloco_id), SEEK_SET);
    write(fd, &newDir, sizeof(Diretorio));

    char ch = 1;
    lseek(fd, BLOCK_SIZE + novo_inode_id, SEEK_SET);
    write(fd, &ch, 1);
    lseek(fd, BLOCK_SIZE * 2 + novo_bloco_id, SEEK_SET);
    write(fd, &ch, 1);

    sb.inodes_livres--;
    sb.blocos_livres--;
    lseek(fd, 0, SEEK_SET);
    write(fd, &sb, sizeof(Superbloco));

    close(fd);
    printf("\nDiretório criado com sucesso.\n");
}

void removerDiretorio(const char *caminho_absoluto) {
    int fd = open("disco", O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("Erro ao abrir o disco virtual");
        return;
    }

    Superbloco sb;
    read(fd, &sb, sizeof(Superbloco));

    if (caminho_absoluto[0] != '/') {
        close(fd);
        printf("Erro: Caminho invalido\n");
        return;
    }

    char caminho[255];
    strcpy(caminho, caminho_absoluto);
    char *lastToken = strrchr(caminho, '/');
    char *token = strtok(caminho, "/");

    int inode_atual = 0;
    int inode_encontrado = -1;
    Diretorio dir;
    Inode dir_inode;
    dir_inode.id = 0;

    while (strcmp(token, lastToken + 1) != 0) {
        lseek(fd, BLOCK_SIZE * 3, SEEK_SET);
        lseek(fd, INODE_SIZE * inode_atual, SEEK_CUR);
        read(fd, &dir_inode, sizeof(Inode));

        if (dir_inode.tipo != 1) {
            printf("Erro: '%s' nao e um diretorio.\n", token);
            close(fd);
            return;
        }

        lseek(fd, BLOCK_SIZE * (sb.onde_comeca_dados + dir_inode.id), SEEK_SET);
        read(fd, &dir, sizeof(Diretorio));

        inode_encontrado = -1;
        for (int i = 0; i < NUM_ARQUIVOS; i++) {
            if (strcmp(dir.arquivos[i].nome, token) == 0) {
                inode_encontrado = dir.arquivos[i].inum;
                break;
            }
        }

        if (inode_encontrado == -1) {
            printf("ERRO: Diretorio %s nao encontrado.\n", token);
            close(fd);
            return;
        }

        inode_atual = inode_encontrado;
        token = strtok(NULL, "/");
    }

    lseek(fd, BLOCK_SIZE * (sb.onde_comeca_dados + inode_atual), SEEK_SET);
    read(fd, &dir, sizeof(Diretorio));

    int vazio = 1;
    for (int i = 0; i < NUM_ARQUIVOS; i++) {
        if (dir.arquivos[i].inum != -1) {
            vazio = 0;
            break;
        }
    }

    if (!vazio) {
        printf("Diretorio nao vazio.\n");
        close(fd);
        return;
    }

    for (int i = 0; i < NUM_ARQUIVOS; i++) {
        if (strcmp(dir.arquivos[i].nome, lastToken + 1) == 0) {
            dir.arquivos[i].inum = -1;
            break;
        }
    }

    char ch = 0;
    lseek(fd, BLOCK_SIZE + inode_atual, SEEK_SET);
    write(fd, &ch, 1);
    lseek(fd, BLOCK_SIZE * 2 + inode_atual, SEEK_SET);
    write(fd, &ch, 1);

    sb.blocos_livres++;
    sb.inodes_livres++;
    lseek(fd, 0, SEEK_SET);
    write(fd, &sb, sizeof(Superbloco));

    close(fd);
    printf("Diretorio apagado com sucesso.\n");
}

void copiaArquivo(const char *caminho_absoluto, const char *nome_arquivo) {
    int fd_disco = open("disco", O_RDWR, S_IRUSR | S_IWUSR);
    if (fd_disco < 0) {
        perror("Erro ao abrir o disco virtual");
        return;
    }

    Superbloco sb;
    read(fd_disco, &sb, sizeof(Superbloco));

    if (caminho_absoluto[0] != '/') {
        printf("Erro: Caminho invalido\n");
        close(fd_disco);
        return;
    }

    int inode_diretorio = 0;
    Diretorio dir;
    Inode dir_inode;
    dir_inode.id = 0;

    char caminho[255];
    strcpy(caminho, caminho_absoluto);
    char *token = strtok(caminho, "/");

    while (token != NULL) {
        lseek(fd_disco, BLOCK_SIZE * 3, SEEK_SET);
        lseek(fd_disco, INODE_SIZE * inode_diretorio, SEEK_CUR);
        read(fd_disco, &dir_inode, sizeof(Inode));

        if (dir_inode.tipo != 1) {
            printf("Erro: '%s' nao e um diretorio.\n", token);
            close(fd_disco);
            return;
        }

        lseek(fd_disco, BLOCK_SIZE * (sb.onde_comeca_dados + dir_inode.id),
              SEEK_SET);
        read(fd_disco, &dir, sizeof(Diretorio));

        int encontrado = 0;
        for (int i = 0; i < NUM_ARQUIVOS; i++) {
            if (strcmp(dir.arquivos[i].nome, token) == 0) {
                inode_diretorio = dir.arquivos[i].inum;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            printf("Erro: Diretorio %s nao encontrado.\n", token);
            close(fd_disco);
            return;
        }

        token = strtok(NULL, "/");
    }

    int inode_arquivo = encontrarInodeVazio();
    int bloco_arquivo = encontrarBlocoVazio();

    if (inode_arquivo == -1 || bloco_arquivo == -1) {
        printf("Erro: Espaço insuficiente para copiar o arquivo.\n");
        close(fd_disco);
        return;
    }

    int fd_arquivo = open(nome_arquivo, O_RDONLY);
    if (fd_arquivo < 0) {
        perror("Erro ao abrir o arquivo para leitura");
        close(fd_disco);
        return;
    }

    Inode novo_inode;
    novo_inode.id = inode_arquivo;
    novo_inode.tipo = 0;

    time_t t;
    time(&t);
    novo_inode.time = t;
    novo_inode.mtime = t;

    struct stat st;
    fstat(fd_arquivo, &st);
    novo_inode.tamanho = (st.st_size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    lseek(fd_disco,
          BLOCK_SIZE * sb.onde_comeca_inodes + inode_arquivo * INODE_SIZE,
          SEEK_SET);
    write(fd_disco, &novo_inode, sizeof(Inode));

    lseek(fd_disco, BLOCK_SIZE * (sb.onde_comeca_dados + bloco_arquivo),
          SEEK_SET);
    char buffer[BLOCK_SIZE];
    int bytes_lidos;
    while ((bytes_lidos = read(fd_arquivo, buffer, BLOCK_SIZE)) > 0) {
        write(fd_disco, buffer, bytes_lidos);
    }

    lseek(fd_disco, BLOCK_SIZE * (sb.onde_comeca_dados + inode_diretorio),
          SEEK_SET);
    read(fd_disco, &dir, sizeof(Diretorio));

    for (int i = 0; i < NUM_ARQUIVOS; i++) {
        if (dir.arquivos[i].inum == -1) {
            dir.arquivos[i].inum = inode_arquivo;
            dir.arquivos[i].reclen = sizeof(Arquivo);
            dir.arquivos[i].strlen = strlen(nome_arquivo);
            strcpy(dir.arquivos[i].nome, nome_arquivo);
            break;
        }
    }

    lseek(fd_disco, BLOCK_SIZE * (sb.onde_comeca_dados + inode_diretorio),
          SEEK_SET);
    write(fd_disco, &dir, sizeof(Diretorio));

    char ch = 1;
    lseek(fd_disco, BLOCK_SIZE + inode_arquivo, SEEK_SET);
    write(fd_disco, &ch, 1);
    lseek(fd_disco, BLOCK_SIZE * 2 + bloco_arquivo, SEEK_SET);
    write(fd_disco, &ch, 1);

    sb.inodes_livres--;
    sb.blocos_livres--;
    lseek(fd_disco, 0, SEEK_SET);
    write(fd_disco, &sb, sizeof(Superbloco));

    close(fd_arquivo);
    close(fd_disco);
    printf("Arquivo copiado com sucesso.\n");
}

void extrairArquivo(const char *caminho_absoluto, const char *nome_destino) {
    int fd_disco = open("disco", O_RDONLY);
    if (fd_disco < 0) {
        perror("Erro ao abrir o disco virtual");
        return;
    }

    Superbloco sb;
    read(fd_disco, &sb, sizeof(Superbloco));

    if (caminho_absoluto[0] != '/') {
        close(fd_disco);
        printf("Erro: Caminho invalido\n");
        return;
    }

    int inode_diretorio = 0;
    Diretorio dir;
    Inode dir_inode;
    dir_inode.id = 0;

    char caminho[255];
    strcpy(caminho, caminho_absoluto);
    char *token = strtok(caminho, "/");

    while (token != NULL) {
        lseek(fd_disco, BLOCK_SIZE * 3, SEEK_SET);
        lseek(fd_disco, INODE_SIZE * inode_diretorio, SEEK_CUR);
        read(fd_disco, &dir_inode, sizeof(Inode));

        if (dir_inode.tipo != 1) {
            printf("Erro: '%s' nao e um diretorio.\n", token);
            close(fd_disco);
            return;
        }

        lseek(fd_disco, BLOCK_SIZE * (sb.onde_comeca_dados + dir_inode.id),
              SEEK_SET);
        read(fd_disco, &dir, sizeof(Diretorio));

        int encontrado = 0;
        for (int i = 0; i < NUM_ARQUIVOS; i++) {
            if (strcmp(dir.arquivos[i].nome, token) == 0) {
                inode_diretorio = dir.arquivos[i].inum;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            printf("Erro: Diretorio %s nao encontrado.\n", token);
            close(fd_disco);
            return;
        }

        token = strtok(NULL, "/");
    }

    lseek(fd_disco, BLOCK_SIZE * 3 + inode_diretorio * INODE_SIZE, SEEK_SET);
    Inode inode_arquivo;
    read(fd_disco, &inode_arquivo, sizeof(Inode));

    if (inode_arquivo.tipo != 0) {
        printf("Erro: '%s' nao e um arquivo.\n", caminho_absoluto);
        close(fd_disco);
        return;
    }

    int fd_destino = open(nome_destino, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd_destino < 0) {
        perror("Erro ao criar o arquivo destino");
        close(fd_disco);
        return;
    }

    char buffer[BLOCK_SIZE];
    int bytes_restantes = inode_arquivo.tamanho * BLOCK_SIZE;
    lseek(fd_disco, BLOCK_SIZE * (sb.onde_comeca_dados + inode_arquivo.id),
          SEEK_SET);

    while (bytes_restantes > 0) {
        int bytes_a_ler =
            (bytes_restantes < BLOCK_SIZE) ? bytes_restantes : BLOCK_SIZE;
        int bytes_lidos = read(fd_disco, buffer, bytes_a_ler);
        if (bytes_lidos <= 0) {
            perror("Erro ao ler o disco virtual");
            close(fd_disco);
            close(fd_destino);
            return;
        }
        write(fd_destino, buffer, bytes_lidos);
        bytes_restantes -= bytes_lidos;
    }

    close(fd_destino);
    close(fd_disco);
    printf("Arquivo extraido com sucesso para '%s'.\n", nome_destino);
}

void listaArquivos(const char *caminho_absoluto) {
    int fd = open("disco", O_RDONLY);
    if (fd < 0) {
        perror("Erro ao abrir o disco virtual");
        return;
    }

    Superbloco sb;
    read(fd, &sb, sizeof(Superbloco));

    if (caminho_absoluto[0] != '/') {
        close(fd);
        printf("Erro: Caminho invalido\n");
        return;
    }

    char caminho[255];
    strcpy(caminho, caminho_absoluto);
    char *lastToken = strrchr(caminho, '/');
    char *token = strtok(caminho, "/");

    int inode_atual = 0;
    int inode_encontrado = -1;
    Diretorio dir;
    Inode dir_inode;
    dir_inode.id = 0;

    while (strcmp(token, lastToken + 1) != 0) {
        lseek(fd, BLOCK_SIZE * 3, SEEK_SET);
        lseek(fd, INODE_SIZE * inode_atual, SEEK_CUR);
        read(fd, &dir_inode, sizeof(Inode));

        if (dir_inode.tipo != 1) {
            printf("Erro: '%s' nao e um diretorio.\n", token);
            close(fd);
            return;
        }

        lseek(fd, BLOCK_SIZE * (sb.onde_comeca_dados + dir_inode.id), SEEK_SET);
        read(fd, &dir, sizeof(Diretorio));

        inode_encontrado = -1;
        for (int i = 0; i < NUM_ARQUIVOS; i++) {
            if (strcmp(dir.arquivos[i].nome, token) == 0) {
                inode_encontrado = dir.arquivos[i].inum;
                break;
            }
        }

        if (inode_encontrado == -1) {
            printf("ERRO: Diretorio %s nao encontrado.\n", token);
            close(fd);
            return;
        }

        inode_atual = inode_encontrado;
        token = strtok(NULL, "/");
    }

    lseek(fd, BLOCK_SIZE * (sb.onde_comeca_dados + inode_atual), SEEK_SET);
    read(fd, &dir, sizeof(Diretorio));

    int inode_alvo;
    for (int i = 0; i < NUM_ARQUIVOS; i++) {
        if (strcmp(token, dir.arquivos[i].nome) == 0) {
            inode_alvo = dir.arquivos[i].inum;
        }
    }

    lseek(fd, BLOCK_SIZE * (sb.onde_comeca_dados + inode_alvo), SEEK_SET);
    read(fd, &dir, sizeof(Diretorio));

    printf("Conteudo de '%s':\n", caminho_absoluto);
    for (int i = 0; i < NUM_ARQUIVOS; i++) {
        if (dir.arquivos[i].inum != -1) {
            printf("%s\n", dir.arquivos[i].nome);
        }
    }

    close(fd);
}

int main() {
    int tamanho_disco;
    char caminho_absoluto[255];
    char arquivo[255];

    if (access("disco", F_OK) != 0) formatar_disco(4);

    while (1) {
        int opcao;
        printf("\nMenu:\n");
        printf("1. Formatar disco virtual\n");
        printf("2. Consultar estado do disco\n");
        printf("3. Criar diretorio\n");
        printf("4. Remover diretorio\n");
        printf("5. todo: Copia de arquivo\n");
        printf("6. todo: Extracao de arquivo\n");
        printf("7. Lista Arquivos\n");
        printf("8. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o tamanho do disco virtual (MB): ");
                scanf("%d", &tamanho_disco);
                formatar_disco(tamanho_disco);
                printf("Disco virtual formatado com sucesso.\n");
                break;
            case 2:
                consultar_estado_disco();
                break;
            case 3: {
                printf("Digite o caminho absoluto do novo diretorio: ");
                scanf("%s", caminho_absoluto);
                criarDiretorio(caminho_absoluto);
                break;
            }
            case 4:
                printf("Digite o caminho absoluto do diretorio: ");
                scanf("%s", caminho_absoluto);
                removerDiretorio(caminho_absoluto);
                break;
            case 5:
                printf("Digite o nome do arquivo: ");
                scanf("%s", arquivo);
                printf(
                    "Digite o caminho absoluto de onde o arquivo vai "
                    "ficar: ");
                scanf("%s", caminho_absoluto);
                copiaArquivo(caminho_absoluto, arquivo);
                break;
            case 6:
                printf(
                    "Digite o caminho absoluto de onde o arquivo vai "
                    "ficar: ");
                scanf("%s", caminho_absoluto);
                extrairArquivo(caminho_absoluto, arquivo);
                break;
            case 7:
                printf("Digite o caminho absoluto do diretorio: ");
                scanf("%s", caminho_absoluto);
                listaArquivos(caminho_absoluto);
                break;
            case 8:
                printf("Saindo...\n");
                exit(0);
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }

    return 0;
}