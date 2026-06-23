#ifndef TROCA_H
#define TROCA_H

#include "figurinha.h"
#include "jogador.h"
#include "telas.h"

/*
    troca.h

    Esse arquivo é responsável pelo sistema de troca de figurinhas
    do projeto.

    Aqui o jogador pode trocar figurinhas repetidas com um sistema
    virtual, ajudando na evolução do álbum.

    Esse módulo conecta diretamente:
    - o álbum de figurinhas;
    - os dados do jogador;
    - o sistema de navegação entre telas.

    Ele representa uma mecânica de progressão do sistema.
*/

/*
    Função responsável por gerenciar toda a lógica e interface
    do sistema de trocas.

    Nessa função acontecem:
    - seleção de figurinhas repetidas do jogador;
    - execução da troca com sistema virtual;
    - atualização do álbum;
    - controle da tela atual do sistema.

    Parâmetros:
    - album: vetor com todas as figurinhas do sistema;
    - total: quantidade total de figurinhas;
    - player: ponteiro com os dados do jogador;
    - telaAtual: controla a navegação entre as telas.

    Essa função é chamada a partir do menu principal do sistema.
*/
void gerenciarTrocas(Figurinha *album, int total, Jogador *player, Tela *telaAtual);

#endif