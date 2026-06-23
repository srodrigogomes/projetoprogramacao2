#include "raylib.h"
#include "../include/estatisticas.h"
#include <stdio.h>

/*
    estatisticas.c

    Esse arquivo é responsável por calcular e exibir na tela
    as estatísticas gerais do álbum de figurinhas.

    Ele funciona como um “painel de progresso”, mostrando ao
    jogador como está a evolução da sua coleção.
*/

void desenharEstatisticas(Figurinha *figs, int total) {

    /*
        Variáveis de contagem usadas para gerar os dados do painel
    */
    int coladas = 0;
    int repetidasTotais = 0;
    int especiaisColadas = 0;
    int totalEspeciais = 0;

    /*
        Percorre todas as figurinhas para calcular estatísticas gerais
    */
    for (int i = 0; i < total; i++) {

        // Conta quantas já foram coladas no álbum
        if (figs[i].colada) coladas++;

        // Soma todas as figurinhas repetidas
        repetidasTotais += figs[i].repetidas;

        /*
            Se raridade > 0, consideramos como figurinha especial
        */
        if (figs[i].raridade > 0) {
            totalEspeciais++;

            // Conta quantas especiais já foram coladas
            if (figs[i].colada) especiaisColadas++;
        }
    }

    /*
        Cálculo do percentual de completude do álbum
    */
    float percentagem = ((float)coladas / total) * 100.0f;

    /*
        --- INTERFACE GRÁFICA (Raylib) ---
        Aqui são desenhados os elementos visuais da tela
        de estatísticas.
    */

    // Fundo do painel
    DrawRectangle(100, 80, 1080, 560, Fade(SKYBLUE, 0.1f));
    DrawRectangleLines(100, 80, 1080, 560, DARKBLUE);

    // Título da tela
    DrawText("ESTATÍSTICAS DO ÁLBUM - COPA 2026", 350, 120, 30, DARKBLUE);

    /*
        Barra de progresso geral do álbum
    */
    DrawText("Progresso Geral:", 150, 200, 20, BLACK);

    DrawRectangle(150, 230, 980, 40, LIGHTGRAY);

    // barra preenchida proporcional ao percentual
    DrawRectangle(150, 230, (int)(percentagem * 9.8f), 40, GREEN);

    DrawRectangleLines(150, 230, 980, 40, BLACK);

    DrawText(TextFormat("%.2f%%", percentagem), 600, 240, 20, WHITE);

    /*
        Informações gerais do álbum
    */
    DrawText(TextFormat("Total de Figurinhas no Álbum: %d", total), 150, 320, 22, DARKGRAY);
    DrawText(TextFormat("Figurinhas Coladas: %d", coladas), 150, 360, 22, DARKGREEN);
    DrawText(TextFormat("Figurinhas em falta: %d", total - coladas), 150, 400, 22, MAROON);

    DrawText(TextFormat("Total de Repetidas: %d", repetidasTotais), 650, 320, 22, DARKGRAY);

    DrawText(TextFormat("Especiais (Efeito): %d / %d", especiaisColadas, totalEspeciais),
             650, 360, 22, ORANGE);

    /*
        Instrução de navegação
    */
    DrawText("Pressione [BACKSPACE] para voltar ao menu", 420, 600, 18, GRAY);
}

#include <string.h>
#include <ctype.h>

void desenharControleFigurinhas(Figurinha *figs, int total, int *scroll, char *busca) {
    DrawRectangle(50, 20, 1180, 680, Fade(RAYWHITE, 0.95f));
    DrawRectangleLines(50, 20, 1180, 680, DARKGRAY);
    
    DrawText("ÁREA DE CONTROLE DE FIGURINHAS", 380, 40, 30, DARKBLUE);
    
    // Barra de Busca
    DrawRectangle(100, 90, 400, 40, LIGHTGRAY);
    DrawRectangleLines(100, 90, 400, 40, DARKGRAY);
    DrawText("Busca (Código ou Nome):", 100, 70, 16, DARKGRAY);
    
    // Captura entrada de texto para busca
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (strlen(busca) < 19)) {
            int len = strlen(busca);
            busca[len] = (char)key;
            busca[len + 1] = '\0';
            *scroll = 0; // Reseta scroll ao buscar
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && strlen(busca) > 0) {
        busca[strlen(busca) - 1] = '\0';
        *scroll = 0;
    }
    
    DrawText(busca, 110, 100, 20, BLACK);
    if (strlen(busca) == 0) DrawText("Digite para buscar...", 110, 100, 20, Fade(GRAY, 0.5f));
    
    // Instrução de Saída em destaque
    DrawRectangle(850, 85, 280, 50, Fade(RED, 0.1f));
    DrawRectangleLines(850, 85, 280, 50, RED);
    DrawText("VOLTAR: [ESC]", 920, 102, 18, MAROON);

    // Filtragem das figurinhas
    int indicesFiltrados[1000];
    int totalFiltrados = 0;
    
    char buscaLower[20];
    strcpy(buscaLower, busca);
    for(int i = 0; buscaLower[i]; i++) buscaLower[i] = tolower(buscaLower[i]);

    for (int i = 0; i < total; i++) {
        char codigoLower[20], tituloLower[60];
        strcpy(codigoLower, figs[i].codigo);
        strcpy(tituloLower, figs[i].titulo);
        for(int j = 0; codigoLower[j]; j++) codigoLower[j] = tolower(codigoLower[j]);
        for(int j = 0; tituloLower[j]; j++) tituloLower[j] = tolower(tituloLower[j]);

        if (strlen(busca) == 0 || strstr(codigoLower, buscaLower) || strstr(tituloLower, buscaLower)) {
            indicesFiltrados[totalFiltrados++] = i;
        }
    }

    // Configurações da lista
    int itemHeight = 38;
    int itemsVisible = 13;
    int startX = 100;
    int startY = 170;
    
    // Captura entrada de scroll
    float wheel = GetMouseWheelMove();
    if (wheel != 0) *scroll -= (int)(wheel * 3);
    if (IsKeyDown(KEY_DOWN)) *scroll += 1;
    if (IsKeyDown(KEY_UP)) *scroll -= 1;
    
    if (*scroll < 0) *scroll = 0;
    int maxScroll = totalFiltrados - itemsVisible;
    if (maxScroll < 0) maxScroll = 0;
    if (*scroll > maxScroll) *scroll = maxScroll;

    // Cabeçalho da tabela
    DrawRectangle(startX, startY - 30, 1080, 30, DARKBLUE);
    DrawText("CÓDIGO", startX + 10, startY - 25, 18, WHITE);
    DrawText("SELEÇÃO", startX + 150, startY - 25, 18, WHITE);
    DrawText("TÍTULO", startX + 400, startY - 25, 18, WHITE);
    DrawText("STATUS", startX + 800, startY - 25, 18, WHITE);
    DrawText("REPETIDAS", startX + 950, startY - 25, 18, WHITE);

    for (int i = 0; i < itemsVisible; i++) {
        int listIdx = *scroll + i;
        if (listIdx >= totalFiltrados) break;
        int idx = indicesFiltrados[listIdx];

        int y = startY + (i * itemHeight);
        if (i % 2 == 0) DrawRectangle(startX, y, 1080, itemHeight, Fade(LIGHTGRAY, 0.3f));
        
        DrawText(figs[idx].codigo, startX + 10, y + 10, 17, BLACK);
        DrawText(figs[idx].selecao, startX + 150, y + 10, 17, DARKGRAY);
        DrawText(figs[idx].titulo, startX + 400, y + 10, 17, BLACK);
        
        if (figs[idx].colada) {
            DrawText("OBTIDA", startX + 800, y + 10, 17, GREEN);
        } else {
            DrawText("FALTANDO", startX + 800, y + 10, 17, RED);
        }
        
        DrawText(TextFormat("%d", figs[idx].repetidas), startX + 1000, y + 10, 17, figs[idx].repetidas > 0 ? ORANGE : DARKGRAY);
    }
    
    // Barra de rolagem visual
    if (totalFiltrados > itemsVisible) {
        DrawRectangle(startX + 1090, startY, 10, itemsVisible * itemHeight, LIGHTGRAY);
        float scrollPos = (float)*scroll / maxScroll;
        DrawRectangle(startX + 1090, startY + (int)(scrollPos * (itemsVisible - 1) * itemHeight), 10, itemHeight, DARKBLUE);
    }
    
    DrawText(TextFormat("Mostrando %d de %d figurinhas", totalFiltrados, total), startX, 675, 16, DARKGRAY);
}
