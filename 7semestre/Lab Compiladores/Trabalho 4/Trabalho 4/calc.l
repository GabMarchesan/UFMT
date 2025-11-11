%{
#include <stdlib.h>
#include <string.h>
#include "calc.tab.h" 

void yyerror(char *);
extern FILE *yyin;
%}

%%

[a-z]       {
                strcpy(yylval.attribute.temp, yytext);
                return VARIABLE;
            }
[0-9]+      {
                strcpy(yylval.attribute.temp, yytext);
                return INTEGER;
            }
[-+()=*/\n] { 
                return *yytext; 
            }
[ \t]       { 
            }
.           { 
                yyerror("invalid character"); 
            }

%%

int yywrap(void) {
    return 1;
}