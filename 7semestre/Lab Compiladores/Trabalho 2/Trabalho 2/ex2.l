%{ 
    #include <stdlib.h> 
    #include <string.h>
    #include "ex2.tab.h" 

    void yyerror(char *s); 
%} 

%% 

[ \t]+

"a"             { strcpy(yylval.str, yytext); return ARTIGO_A; } 
"o"             { strcpy(yylval.str, yytext); return ARTIGO_O; }

"gato"          { strcpy(yylval.str, yytext); return SUBSTANTIVO_GATO; }
"cachorro"      { strcpy(yylval.str, yytext); return SUBSTANTIVO_CACHORRO; }
"comida"        { strcpy(yylval.str, yytext); return SUBSTANTIVO_COMIDA; }
"biscoito"      { strcpy(yylval.str, yytext); return SUBSTANTIVO_BISCOITO; }

"e"             { strcpy(yylval.str, yytext); return VERBO_E; }
"come"          { strcpy(yylval.str, yytext); return VERBO_COME; }

"bonito"        { strcpy(yylval.str, yytext); return ADJETIVO_BONITO; }
"feio"          { strcpy(yylval.str, yytext); return ADJETIVO_FEIO; }

\n              { return '\n'; }

<<EOF>>         { 
                  static int eof_flag = 0;
                  if (!eof_flag) {
                      eof_flag = 1;
                      return '\n';
                  }
                  return 0;
                }

.               { yyerror("Caractere invalido"); }

%% 

int yywrap(void) { 
    return 1; 
}