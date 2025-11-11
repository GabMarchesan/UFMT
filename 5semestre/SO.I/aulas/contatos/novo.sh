#!/bin/bash

printf "Digite o nome do contato: "
read nome # Variável que guarda o nome
printf "Digite seu Telefone: "
read fone # Variável que guarda o fone

# Insere novo contato no final do arquivo
printf "$nome \t\t - $fone\n" >> contatos.txt

# Ordenar a lista de contatos
sort contatos.txt > tmp.txt
mv tmp.txt contatos.txt
