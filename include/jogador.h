#ifndef JOGADOR_H
#define JOGADOR_H

/*
    jogador.h

    Esse arquivo define a estrutura que representa o jogador
    dentro do sistema do álbum de figurinhas.

    Diferente das figurinhas, aqui são armazenados os dados
    do usuário, como recursos e progresso em atividades extras
    (por exemplo o mini-game).
*/

/*
    Estrutura que representa o perfil do jogador.

    Essa struct guarda informações que mudam conforme o uso
    do sistema, como:

    - moedas: usada na loja para comprar pacotes;
    - pacotesDisponiveis: quantidade de pacotes que o jogador possui;
    - totalGols: estatística do mini-game (caso exista no projeto).

    Essa estrutura ajuda a manter o controle do estado do usuário
    durante toda a execução do programa.
*/
typedef struct {
    int moedas;
    int pacotesDisponiveis;
    int totalGols;
} Jogador;

#endif