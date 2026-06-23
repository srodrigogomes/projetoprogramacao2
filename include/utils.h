#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"

/*
    utils.h

    Esse arquivo reúne pequenas definições auxiliares
    usadas em diferentes partes do projeto.

    Ele funciona como um arquivo de “constantes gerais”,
    evitando repetição de valores espalhados pelo código.
*/

/*
    Definição das cores utilizadas no sistema de raridade.

    Essas cores são usadas principalmente na interface gráfica
    para diferenciar visualmente cada tipo de figurinha:

    - Comum: cor padrão mais simples
    - Rara: destaque intermediário
    - Lendária: destaque máximo (mais rara do jogo)
    - Borda: usada nos slots do álbum

    Isso melhora a experiência visual do usuário e facilita
    a identificação das figurinhas.
*/

// Cor de figurinhas comuns
#define COR_COMUM       LIGHTGRAY

// Cor de figurinhas raras
#define COR_RARA        SKYBLUE

// Cor de figurinhas lendárias
#define COR_LENDARIA    GOLD

// Cor das bordas dos slots do álbum
#define COR_BORDA_SLOT  DARKGRAY

#endif