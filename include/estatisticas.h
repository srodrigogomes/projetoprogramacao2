#ifndef ESTATISTICAS_H
#define ESTATISTICAS_H

#include "figurinha.h"

/*
    estatisticas.h

    Esse arquivo é responsável por declarar as funções ligadas
    às estatísticas do álbum de figurinhas.

    A ideia aqui é mostrar ao usuário informações gerais
    sobre sua coleção, como progresso e evolução do álbum.

    Essa parte ajuda a dar uma “visão geral” do sistema,
    funcionando como um painel de acompanhamento do jogador.
*/

/*
    Função responsável por calcular e desenhar na tela
    o progresso do álbum.

    Ela percorre o vetor de figurinhas e verifica quais
    já foram coletadas pelo usuário.

    Com base nisso, o sistema calcula o percentual de
    completude do álbum e exibe essas informações
    na interface.

    Parâmetros:
    - figs: vetor com todas as figurinhas do sistema
    - total: quantidade total de figurinhas (ex: 980)

    Essa função é chamada a partir da interface principal
    do jogo/aplicação.
*/
void desenharEstatisticas(Figurinha *figs, int total);
void desenharControleFigurinhas(Figurinha *figs, int total, int *scroll, char *busca);

#endif