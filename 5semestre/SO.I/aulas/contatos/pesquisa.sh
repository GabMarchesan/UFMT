#!/bin/bash

printf "Digite o nome da pesquisa: "
read nome

if [[ $nome == "" ]]
then
    echo "String de pesquisa não informada"
    exit # encerra o script
fi 

grep -i $nome contatos.txt