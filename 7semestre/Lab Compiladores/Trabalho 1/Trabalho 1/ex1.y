%{
    #include <stdio.h>
    #include <stdlib.h>
    void yyerror(char *s);
    int yylex(void);
%}

%token INTEGER

%left '+' '-'
%left '*' '/'
%right UMINUS

%%

program:
    | program expr '\n' { printf("%d\n", $2); }
    ;

expr:
      expr '+' expr { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr { 
        if ($3 == 0) {
            yyerror("Divisão por zero!");
            exit(EXIT_FAILURE);
        } else {
            $$ = $1 / $3;
        }
    }
    | '(' expr ')'  { $$ = $2; }
    | '-' expr %prec UMINUS { $$ = -$2; }
    | INTEGER       { $$ = $1; }
    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "Erro: %s\n", s);
}

int main(void) {
    return yyparse();
}
