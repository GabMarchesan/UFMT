CC = gcc

# Nome do executável final
TARGET = analisador

# --- Regras ---

# Regra padrão: compila tudo
all: $(TARGET)

# Regra para criar o executável final
$(TARGET): ex2.tab.o lex.yy.o
	$(CC) -o $(TARGET) ex2.tab.o lex.yy.o

# Regra para gerar os arquivos do Bison
ex2.tab.c ex2.tab.h: ex2.y
	bison -d ex2.y

# Regra para gerar o arquivo do Flex
lex.yy.c: ex2.l ex2.tab.h
	flex -o lex.yy.c ex2.l

# Regra para compilar o código gerado pelo Bison
ex2.tab.o: ex2.tab.c
	$(CC) -c ex2.tab.c

# Regra para compilar o código gerado pelo Flex
lex.yy.o: lex.yy.c
	$(CC) -c lex.yy.c

# Regra para executar o programa com o arquivo de teste
run: all
	./$(TARGET) < code_test.txt

# Regra para limpar os arquivos gerados
clean:
	rm -f $(TARGET) ex2.tab.c ex2.tab.h lex.yy.c *.o