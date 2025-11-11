all: tab_simb

tab_simb: tab_simb.tab.c lex.yy.c
	gcc -o tab_simb tab_simb.tab.c lex.yy.c -lfl

tab_simb.tab.c tab_simb.tab.h: tab_simb.y
	bison -d tab_simb.y

lex.yy.c: tab_simb.l tab_simb.tab.h
	flex tab_simb.l

run: all
	./tab_simb

clean:
	rm -f tab_simb.tab.c tab_simb.tab.h lex.yy.c tab_simb
