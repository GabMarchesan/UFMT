all: comp

comp: ex1.tab.o lex.yy.o
	gcc -o comp ex1.tab.o lex.yy.o -lm

ex1.tab.c ex1.tab.h: ex1.y
	bison -d ex1.y

lex.yy.c: ex1.l ex1.tab.h
	flex ex1.l

ex1.tab.o: ex1.tab.c
	gcc -c ex1.tab.c

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

run:
	./comp < code_test.txt

clean:
	rm -f comp ex1.tab.* lex.yy.c *.o
