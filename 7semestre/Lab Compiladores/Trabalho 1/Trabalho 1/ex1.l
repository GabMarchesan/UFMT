%{ 
    #include <stdlib.h> 
    #include "ex1.tab.h" 
    void yyerror(char *);
%} 

%% 
[0-9]+  { yylval = atoi(yytext); return INTEGER; }
[+\-*/()] { return *yytext; }
[ \t]    ; /* ignora espaço e tab */
\n       { return '\n'; }
.        { yyerror("Caractere inválido"); }
<<EOF>>  { return '\n'; }
%%

int yywrap(void) { return 1; }