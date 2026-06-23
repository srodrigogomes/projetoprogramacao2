#ifndef FIGURINHA_H
#define FIGURINHA_H

/*
    figurinha.h

    Esse arquivo é um dos principais do projeto, pois aqui está
    definida a estrutura base do sistema: a figurinha.

    A struct Figurinha representa cada item do álbum e guarda
    todas as informações necessárias para o controle da coleção.

    A partir dessa estrutura, todas as outras partes do sistema
    (main, estatísticas, arquivos e interface) conseguem trabalhar
    com os mesmos dados de forma organizada.
*/

/*
    Estrutura que representa uma figurinha do álbum.

    Cada figurinha possui informações como:
    - código identificador (ex: BRA1, FWC00);
    - título ou nome do jogador;
    - seleção ou categoria;
    - grupo (quando aplicável);
    - tipo (comum ou especial);
    - raridade;
    - estado se já foi colada no álbum;
    - quantidade de repetidas.

    Essa struct é a base de todo o sistema.
*/
#include "raylib.h"

typedef struct {
    char codigo[15];
    char titulo[60];
    char selecao[40];
    char grupo[15];
    char tipo[20];
    int raridade;   // 0: Comum, 1: Rara, 2: Lendária
    int colada;     // 0: Não, 1: Sim
    int repetidas;
    Texture2D textura; // Textura da figurinha (carregada dinamicamente)
} Figurinha;

/*
    Funções principais de manipulação do álbum.

    Essas funções representam as operações básicas do sistema:

    - inserir: adiciona uma nova figurinha;
    - listar: mostra todas as figurinhas cadastradas;
    - pesquisar: busca uma figurinha específica;
    - alterar: modifica dados de uma figurinha;
    - remover: exclui uma figurinha do sistema.

    Todas elas são chamadas a partir do menu principal (main.c),
    que controla o fluxo do programa.
*/
void inserirFigurinha(Figurinha **figs, int *total);
void listarFigurinhas(Figurinha *figs, int total);
void pesquisarFigurinha(Figurinha *figs, int total);
void alterarFigurinha(Figurinha *figs, int total);
void removerFigurinha(Figurinha **figs, int *total);

#endif