familia(
   pessoa(pedro,silva,data(1,abril,1930),empregado(cnn,1000)), 
   pessoa(maria,silva,data(1,maio,1945),empregado(bnpp,1230)),
   [
      pessoa(manoel,silva,data(10,setembro,1975),desempregado),
      pessoa(ana,silva,data(12,novembro,1981),empregado(bcc,800))
   ]
).

familia(
   pessoa(paulo,santos,data(1,janeiro,1955),empregado(abb,1200)), 
   pessoa(ana,santos,data(1,maio,1955),desempregado),
   [
      pessoa(manoela,santos,data(10,setembro,1981),desempregado),
      pessoa(andre,santos,data(18,novembro,1978),empregado(bb,1100)), 
      pessoa(carlos,santos,data(02,fevereiro,1981),empregado(cc,3200))
   ]
).

familia(
   pessoa(eddard,stark,data(31,03,1948),morto),
   pessoa(catelyn,stark,data(11,08,1953),morto),
   [
      pessoa(robb,stark,data(09,12,1977),morto), 
      pessoa(sansa,stark,data(01,06,1979),empregado(conselheira,11000)),
      pessoa(arya,stark,data(06,01,1991),empregado(assasina, 8800)),
      pessoa(bran,stark,data(23,10,1993),empregado(vidente, 1)),
      pessoa(rickon,stark,data(29,04,1999),morto)
   ]
).

marido(X) :- familia(X,_,_).
esposa(X) :- familia(_,X,_).
filhos(X) :- familia(_,_,Filhos), member(X,Filhos).
existe(Pessoa) :- marido(Pessoa); esposa(Pessoa); filhos(Pessoa).
dataDeNasc(pessoa(_,_,Data,_),Data).
dt(Data) :- existe(X), dataDeNasc(X,Data).
salario(pessoa(_,_,_,empregado(_,S)),S).
salario(pessoa(_,_,_,desempregado),0).

nomes(Nome_Sobrenome) :-
    existe(pessoa(Nome, Sobrenome, _, _)),
    atom_concat(Nome, ' ', Temp),
    atom_concat(Temp, Sobrenome, Nome_Sobrenome).

nomes(X) :-
    nomes(Nome_Sobrenome),
    X = Nome_Sobrenome.

filhosBydt(Nome_Sobrenome, Ano) :-
    filhos(pessoa(Nome, Sobrenome, data(_, _, Ano), _)),
    atom_concat(Nome, ' ', Temp),
    atom_concat(Temp, Sobrenome, Nome_Sobrenome).

esposaEmpregada(Nome_Sobrenome) :-
    esposa(pessoa(Nome, Sobrenome, _, empregado(_, _))),
    atom_concat(Nome, ' ', Temp),
    atom_concat(Temp, Sobrenome, Nome_Sobrenome).

pessoasDesempregada(Nome_Sobrenome, Ano) :-
    existe(pessoa(Nome, Sobrenome, data(_, _, AnoNasc), desempregado)),
    AnoNasc < Ano,
    atom_concat(Nome, ' ', Temp),
    atom_concat(Temp, Sobrenome, Nome_Sobrenome).

nascidoAntesSalario(Nome_Sobrenome, Ano, Salario) :-
    existe(pessoa(Nome, Sobrenome, data(_, _, AnoNasc), empregado(_, Sal))),
    AnoNasc < Ano,
    Sal < Salario,
    atom_concat(Nome, ' ', Temp),
    atom_concat(Temp, Sobrenome, Nome_Sobrenome).

familiaMinimFilhos(Sobrenome, MinFilhos) :-
    familia(_, pessoa(_, Sobrenome, _, _), Filhos),
    length(Filhos, NumFilhos),
    NumFilhos >= MinFilhos.
    
rendaFamilia(Sobrenome, RendaTotal) :-
    familia(_, pessoa(_, Sobrenome, _, _), Filhos),
    findall(Salario, (member(Filho, Filhos), salario(Filho, Salario)), Salarios),
    sum_list(Salarios, RendaTotal).

familiaSemFilhos(Sobrenome) :-
    familia(_, pessoa(_, Sobrenome, _, _), []),
    not((familia(_, pessoa(_, Sobrenome, _, _), Filhos), Filhos \= [])).

filhosDesempregados(Nome_Sobrenome) :-
    filhos(pessoa(Nome, Sobrenome, _, desempregado)),
    atom_concat(Nome, ' ', Temp),
    atom_concat(Temp, Sobrenome, Nome_Sobrenome).

diferencaIdadePais(DataPai, DataMae, Diferenca) :-
    idade(DataPai, IdadePai),
    idade(DataMae, IdadeMae),
    Diferenca is abs(IdadePai - IdadeMae).

paisIdade(Filho, Diferenca) :-
    familia(pessoa(Pai, _, DataPai, _), pessoa(Mae, _, DataMae, _), _),
    filhos(pessoa(Filho, _, _, _)),
    diferencaIdadePais(DataPai, DataMae, Diferenca),
    Diferenca >= 15.




