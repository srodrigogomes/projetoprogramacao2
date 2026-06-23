#ifndef PACOTE_H
#define PACOTE_H

#include "figurinha.h"
#include "jogador.h"
#include "telas.h"

/*
    pacote.h

    Esse arquivo reúne as funções relacionadas à abertura de pacotes
    de figurinhas dentro do sistema.

    Aqui acontece uma das partes principais da “experiência do usuário”,
    pois é onde o jogador recebe novas figurinhas para o álbum.

    Esse módulo faz a ligação entre:
    - as figurinhas (figurinha.h)
    - o jogador (jogador.h)
    - o controle de telas e interface (telas.h)
*/

/*
    Função responsável por gerenciar a abertura de pacotes na versão
    com interface gráfica (Raylib).

    Nessa função são controlados:
    - a animação de abertura do pacote;
    - a seleção/geração das figurinhas obtidas;
    - a atualização do álbum do jogador;
    - o retorno para a tela anterior após a abertura.

    Parâmetros:
    - album: vetor de figurinhas do sistema;
    - total: quantidade total de figurinhas existentes;
    - player: ponteiro para o jogador (controle de pacotes e progresso);
    - telaAtual: controla a navegação entre as telas do sistema.

    Essa parte é chamada quando o jogador compra ou ganha um pacote.
*/
void gerenciarAberturaPacote(Figurinha *album, int total, Jogador *player, Tela *telaAtual);

/*
    Função antiga mantida para versão em console.

    Ela representa a lógica básica de abertura de pacote sem interface
    gráfica, usada principalmente para testes ou compatibilidade com
    a versão inicial do CRUD do projeto.

    Essa função não possui animações, apenas realiza a lógica de
    sortear e adicionar figurinhas ao jogador.
*/
void abrirPacoteConsole(Figurinha *album, int total, Jogador *player);

#endif