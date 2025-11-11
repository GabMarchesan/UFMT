
comp:  sintatico.tab.o  lex.yy.o main.o codeGeneration.o symbolTable.o
	gcc -g lex.yy.o sintatico.tab.o main.o codeGeneration.o symbolTable.o -o comp

lex.yy.o:   lex.yy.c
	gcc -g -c lex.yy.c
	
sintatico.tab.o:  sintatico.tab.c
	gcc -g -c sintatico.tab.c

lex.yy.c:  lexico.l
	flex lexico.l
	
sintatico.tab.c:   sintatico.y
	bison -d sintatico.y -o sintatico.tab.c
	
main.o:  main.c
	gcc -g -c main.c 

codeGeneration.o: codeGeneration.c 
	gcc -g -c codeGeneration.c  
	
symbolTable.o: symbolTable.c
	gcc -g -c symbolTable.c

clean:
	rm -f *.o comp sintatico.tab.c sintatico.tab.h lex.yy.c teste.asm core *.asm

run:
	./comp teste1.txt