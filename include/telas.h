#ifndef TELAS_H
#define TELAS_H

#include "raylib.h"
#include "figurinha.h"
#include "jogador.h"

/*
    telas.h

    Esse arquivo é responsável por controlar o sistema de navegação
    entre as telas do projeto.

    Ele funciona como um “controlador de estado” da interface,
    definindo em qual parte do programa o usuário está (menu,
    álbum, loja, mini-game etc).

    Esse módulo é essencial porque organiza toda a parte gráfica
    do sistema e conecta os diferentes módulos do projeto.
*/

/*
    Enum que representa todas as telas do sistema.

    Cada valor do enum representa uma tela diferente da aplicação:

    - TELA_MENU: menu principal
    - TELA_ALBUM: visualização do álbum de figurinhas
    - TELA_PACOTE: abertura de pacotes
    - TELA_MINIGAME: mini-game de pênaltis
    - TELA_ESTATS: tela de estatísticas do álbum
    - TELA_LOJA: loja de pacotes
    - TELA_TROCA: sistema de troca de figurinhas
    - TELA_SAIR: encerramento do programa

    Esse enum evita o uso de números soltos e facilita o controle
    do fluxo do programa.
*/
typedef enum {
    TELA_MENU,
    TELA_ABERTURA_ALBUM,
    TELA_ALBUM,
    TELA_PACOTE,
    TELA_MINIGAME,
    TELA_ESTATS,
    TELA_LOJA,
    TELA_TROCA,
    TELA_CONTROLE,
    TELA_SAIR
} Tela;

/*
    Função responsável por gerenciar o desenho da tela atual.

    Essa função centraliza toda a interface gráfica do sistema,
    decidindo o que deve ser exibido dependendo do estado atual
    do programa.

    Parâmetros:
    - telaAtual: controla qual tela está ativa no momento;
    - album: vetor de figurinhas do sistema;
    - totalFigurinhas: quantidade total de figurinhas existentes;
    - player: dados do jogador (moedas, progresso etc);
    - paginaAtual: controla a navegação dentro do álbum.

    Essa função é chamada continuamente no loop principal
    do Raylib (geralmente dentro do main.c), sendo responsável
    por atualizar toda a interface do sistema.
*/
void desenharTelaAtual(Tela *telaAtual, Figurinha *album, int totalFigurinhas, Jogador *player, int *paginaAtual);

#endif