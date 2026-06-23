#ifndef MINIGAME_H
#define MINIGAME_H

#include "jogador.h"
#include "telas.h"

/*
    minigame.h

    Esse arquivo declara as funções relacionadas ao mini-game
    de pênaltis do sistema.

    O mini-game é uma funcionalidade extra do projeto, criada
    para permitir que o jogador ganhe recompensas (como pacotes
    de figurinhas) ao jogar.
*/

/*
    Enumeração que representa as direções possíveis do chute
    no mini-game.

    Isso facilita a leitura do código, pois em vez de usar
    números “soltos”, usamos nomes mais claros:

    - ESQUERDA
    - CENTRO
    - DIREITA
*/
typedef enum {
    ESQUERDA,
    CENTRO,
    DIREITA
} Direcao;

/*
    Função responsável por controlar toda a lógica e a
    interface do mini-game.

    Dentro dela acontecem:
    - desenho da tela do jogo;
    - escolha da direção do chute;
    - definição do resultado (gol ou defesa);
    - atualização do jogador (recompensa);
    - retorno para outras telas do sistema.

    Parâmetros:
    - player: ponteiro para o jogador, usado para dar recompensa;
    - telaAtual: controla a navegação entre as telas do sistema.

    Essa função é chamada a partir do menu principal ou da
    interface de navegação.
*/
void gerenciarMiniGame(Jogador *player, Tela *telaAtual);

#endif