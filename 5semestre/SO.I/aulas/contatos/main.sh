#!/bin/bash

while true; do
    clear
    printf "O fabuloso Gestor de Contatos - powered by AI\n"
    printf "...........................................\n\n"
    ncontatos=$(wc -l < contatos.txt)
    printf "Você tem %d contatos em sua lista\n" $ncontatos
    printf "1 - Novo contato\n"
    printf "2 - Pesquisar contato\n"
    printf "3 - Listar contato\n"
    printf "4 - Sair\n"

    read opcao
    case $opcao in
        1) ./novo.sh ;;
        2) ./pesquisa.sh ;;
        3) cat contatos.txt ;;
        4) exit ;;
        *) echo "Opção inválida";;
    esac
    read -p "Pressione Enter para continuar"
done