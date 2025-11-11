# Alunos: 
- Gabriel Gomes Marchesan
- Lui Grosz

##  Sistema de Arquivo Virtual 

## Execute o arquivo
Abra o arquivo main.c no terminal linux 

- Compilar o Código:
gcc main.c -o main

- Execute o arquivo gerado: 
./main 

Ao executar o arquivo vai aparecer o Menu com opções de 1 á 8

### 1 -  Formatar o disco virtual

- Ao escolher a opção 1, você está escolhendo a opção de Formatar Disco Virtual,
- em seguida o usuário digita o tamanho do disco virtual em MB, Exemplo: 2
- Caso de tudo certo aparecerá a mensagem:
Disco Virtual formatado com sucesso.
- Em seguida vão aparecer as 8 opções novamente.

### 2 - Consultar estado do disco

- Ao escolher a opção 2, você está escolhendo a opção de Consultar estado do disco obtendo as seguintes informações:

- Tamanho do Disco: 
- Tamanho do Bloco: 
- Espaço Ocupado: 
- Espaço Livre:
- Espaço ocupado com arquivos:
- Blocos Livres:
- Inodes Livres:

- Em seguida Aparecerá novamente o Menu com as 8 opções disponíveis 

### 3 - Criar Diretório

- Ao selecionar a opção 3, você está escolhendo a opção de criar diretório, sem seguida aparecerá a mesnagem:
Digite o caminho absoluto do novo diretório: Exemplo: /teste
vai aparecer em seguida as mensganes do novo_inode: e index:
e a mensagem de Direório criado com sucesso.
- Em seguida vai aparecer novamente o Menu com as 8 opções.

### 4 - Remoção do diretório

- Ao selecionar a opção 4, o usuário está selecioando a opção de Remover Diretório, somente os diretórios vazios podem ser removidos.
Digite o caminho absoluto do diretório: Exemplo: /teste como no caso anterior,
- Caso o usuário tente remover um dirrtório não vazio vai aparecer a mensagem de
Diretório não vazio.
- Em seguida vai aparecer novamente o Menu com as 8 opções.

### 5 - Cópia de um arquivo do sistema de arquivos do SO para dentro do DV

- Ao selecionar a opção 5, o usuário está selecionando a opção de todo: Copia de arquivo, caso tenha espaço suficiente no DV vão aparecer as seguintes mesnagens:
Digite o nome do arquivo: Exemplo: test.txt caso ele exita no diretório
Digite o caminho absoluto de onde o arquivo vai ficar: Exemplo: /teste criado na opção 3
- Caso de certo vai aparecer a mensagem:
Arquivo copiado com sucesso.
- Caso tenha algum erro vai aparecer a mensagem:
Erro ao abrir o arquivo para leitura: No such file or directory
- Em seguida vai aparecer novamente o Menu com as 8 opções.

### 6 - Extração de um arquivo do DV para o SO

- Ao selecionar a opção 6, o usuário está selecionando a opção todo: Extração de arquivo, em seguida aparecerá a mesnagem:
Digite o caminho absoluto de onde o arquivo vai ficar: Exemplo: /teste/teste.txt
- Caso de certo vai aparecer a mesnagem:
Arquivo extraido com sucesso para 'teste.txt'
- Caso ocorra erro de arquivo vai aparecer a mensagem:
Erro: '/teste não é um arquivo
- Caso ocorra erro de Caminho absoluto vai aparecer a mensagem:
Erro: Caminho invalido
- Em seguida vai aparecer novamente o Menu com as 8 opções.

### 7 -  Listagem de arquivos em um diretório do DV

- Ao selecionar a opção 7, o usuário está selecionando a opção Lista Arquivos, em seguida vai aparecer a mensagem:
Digite o caminho absoluto do diretório: Exemplo: /teste
- Caso o diretório exita vai aparecer a mensagem:
Conteúdo de '/teste': em seguida mostrando todos os conteúdos
- Em seguida vai aparecer novamente o Menu com as 8 opções.

### 8 - Sair

- Ao selecionar a opção 8, o usuário está selecionando a opção Sair, aparecendo em seguida a mensagem:
saindo...
- Assim terminando a execução do código.