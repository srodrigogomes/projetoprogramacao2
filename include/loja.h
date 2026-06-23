#ifndef LOJA_H
#define LOJA_H

#include "jogador.h"
#include "telas.h"

/*
    loja.h

    Esse arquivo declara as funções responsáveis pela
    loja de pacotes dentro do sistema do álbum.

    A loja é uma das partes da interface gráfica (Raylib),
    onde o jogador pode gastar moedas para comprar pacotes
    de figurinhas.

    Essa parte do sistema conecta diretamente:
    - o perfil do jogador (jogador.h)
    - o controle de telas do sistema (telas.h)

    Ou seja, ela faz a ligação entre economia do jogo e navegação.
*/

/*
    Função responsável por gerenciar toda a tela da loja.

    Nessa função acontece:
    - exibição da interface gráfica da loja;
    - verificação de moedas do jogador;
    - compra de pacotes de figurinhas;
    - atualização dos dados do jogador;
    - navegação de volta para outras telas.

    Parâmetros:
    - player: ponteiro para o jogador, usado para alterar moedas
      e quantidade de pacotes disponíveis;
    - telaAtual: controla em qual tela o sistema está,
      permitindo voltar ao menu principal ou trocar de interface.

    Essa função é chamada a partir do fluxo principal do sistema
    (geralmente dentro do main.c ou do controlador de telas).
*/
void gerenciarLoja(Jogador *player, Tela *telaAtual);

#endif