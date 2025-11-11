homem(anibal).
homem(jose).
homem(fernando).
homem(carlos).
homem(ricardo).
homem(daniel).
homem(paulo).

mulher(alzira).
mulher(marta).
mulher(diana).
mulher(zulmira).
mulher(fatima).
mulher(sara).
mulher(adriana).

casados(anibal, alzira).
casados(jose, marta).
casados(fernando, diana).
casados(carlos, zulmira).
casados(ricardo, fatima).
casados(paulo, sara).
casados(daniel, adriana).

descendente_direto(francisco, anibal, alzira).
descendente_direto(alvaro, anibal, alzira).
descendente_direto(celia, anibal, alzira).
descendente_direto(delfina, jose, marta).
descendente_direto(zulmira, jose, marta).
descendente_direto(carlos, fernando, diana).
descendente_direto(paulo, fernando, diana).
descendente_direto(ricardo, carlos, zulmira).
descendente_direto(daniel, carlos, zulmira).
descendente_direto(adriana, ricardo, fatima).
descendente_direto(samuel, adriana, fatima).

% Filho
filho(X, Y) :- 
    homem(X),
    casados(Y, Z),
    descendente_direto(X, Y, Z).

% Filha
filha(X, Y) :- 
    mulher(X),
    casados(Y, Z),
    descendente_direto(X, Y, Z).

% Pai
pai(X, Y) :- 
    homem(X),
    casados(X, Y),
    descendente_direto(Y, X, _).

% Mãe
mae(X, Y) :- 
    mulher(X),
    casados(X, Y),
    descendente_direto(Y, X, _).

% Avô paterno
avo_paterno(X, Y) :- 
    homem(X),
    pai(X, Z),
    descendente_direto(Y, Z, _).

% Avó paterna
avo_paterna(X, Y) :- 
    mulher(X),
    mae(X, Z),
    descendente_direto(Y, Z, _).

% Avô materno
avo_materno(X, Y) :- 
    homem(X),
    pai(X, Z),
    descendente_direto(Y, _, Z).

% Avó materna
avo_materna(X, Y) :- 
    mulher(X),
    mae(X, Z),
    descendente_direto(Y, _, Z).

% Neto
neto(X, Y) :- 
    descendente_direto(X, _, Z),
    descendente_direto(Z, Y, _).

% Neta
neta(X, Y) :- 
    descendente_direto(X, _, Z),
    descendente_direto(Z, Y, _),
    mulher(X).

% Irmão
irmao(X, Y) :- 
    homem(X),
    homem(Y),
    casados(Z, _),
    descendente_direto(X, Z, _),
    descendente_direto(Y, Z, _),
    X \= Y.

% Irmã
irma(X, Y) :- 
    mulher(X),
    mulher(Y),
    casados(Z, _),
    descendente_direto(X, Z, _),
    descendente_direto(Y, Z, _),
    X \= Y.

% Tio
tio(X, Y) :- 
    homem(X),
    irmao(X, Z),
    descendente_direto(Y, Z, _).

% Tia
tia(X, Y) :- 
    mulher(X),
    irma(X, Z),
    descendente_direto(Y, Z, _).

% Sobrinho
sobrinho(X, Y) :- 
    homem(X),
    (tio(Y, X); tia(Y, X)).

% Sobrinha
sobrinha(X, Y) :- 
    mulher(X),
    (tio(Y, X); tia(Y, X)).

% Primo
primo(X, Y) :- 
    homem(X),
    descendente_direto(X, Z, _),
    (tio(Z, Y); tia(Z, Y)),
    X \= Y.

% Prima
prima(X, Y) :- 
    mulher(X),
    descendente_direto(X, Z, _),
    (tio(Z, Y); tia(Z, Y)),
    X \= Y.

% Cunhado
cunhado(X, Y) :- 
    homem(X),
    casados(Z, _),
    (irmao(X, Z); irmao(Z, X)),
    descendente_direto(Y, Z, _),
    X \= Y.

% Cunhada
cunhada(X, Y) :- 
    mulher(X),
    casados(Z, _),
    (irma(X, Z); irma(Z, X)),
    descendente_direto(Y, Z, _),
    X \= Y.

