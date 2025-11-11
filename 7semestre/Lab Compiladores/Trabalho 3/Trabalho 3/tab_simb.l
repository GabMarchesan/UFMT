%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tab_simb.tab.h"
%}

%%
[ \t\n]+                  ;
[0-9]+                    { yylval.valor = atoi(yytext); return NUM; }
[a-zA-Z][a-zA-Z0-9_]*      { yylval.id = strdup(yytext); return ID; }
.                         { return *yytext; }
%%

int yywrap() {
    return 1;
}
