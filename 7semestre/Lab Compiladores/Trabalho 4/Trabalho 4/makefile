# Compilador e ferramentas
CC = gcc
BISON = bison
FLEX = flex

# Nome do executável final
TARGET = calc

# Arquivos fonte
BISON_SRC = calc.y
FLEX_SRC = calc.l

# Arquivos gerados
BISON_HEADER = calc.tab.h
BISON_OUTPUT = calc.tab.c
FLEX_OUTPUT = lex.yy.c

# Arquivos objeto
OBJS = $(FLEX_OUTPUT:.c=.o) $(BISON_OUTPUT:.c=.o)

# Alvo padrão: compila tudo
all: $(TARGET)

# Regra para linkar o executável final
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Regra de compilação genérica para arquivos .c -> .o
%.o: %.c
	$(CC) -c $< -o $@

# O código do Flex (lex.yy.c) depende do header do Bison (calc.tab.h)
$(FLEX_OUTPUT): $(FLEX_SRC) $(BISON_HEADER)
	$(FLEX) -o $(FLEX_OUTPUT) $(FLEX_SRC)

# O código e o header do Bison são gerados a partir do arquivo .y
$(BISON_OUTPUT) $(BISON_HEADER): $(BISON_SRC)
	$(BISON) -d -o $(BISON_OUTPUT) $(BISON_SRC)

# Alvo para executar o programa com um arquivo de teste
run: all
	@echo "a = 2 + 3 * 4" > input.txt
	@echo "b = (5 + 2) * 3" >> input.txt
	@echo "c = 10 / 5" >> input.txt
	@echo "\n--- Executando o compilador com o arquivo input.txt ---"
	./$(TARGET) input.txt
	@echo "--- Execução finalizada ---\n"

# Alvo para limpar todos os arquivos gerados
clean:
	rm -f $(TARGET) $(OBJS) $(FLEX_OUTPUT) $(BISON_OUTPUT) $(BISON_HEADER) input.txt