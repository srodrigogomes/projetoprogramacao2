#include <stdlib.h>
#include "raylib.h"
#include "../include/troca.h"
#include "../include/utils.h"

/*
    troca.c

    Esse módulo implementa o sistema de trocas de figurinhas.
    Atualizado para mostrar as imagens das figurinhas sendo trocadas.
*/

typedef enum {
    BUSCANDO_OFERTA,
    MOSTRANDO_OFERTA,
    SUCESSO_TROCA,
    SEM_REPETIDAS
} EstadoTroca;

/**
 * Helper para desenhar uma figurinha em destaque na tela de trocas
 */
static void desenharFigurinhaTroca(Figurinha fig, int x, int y, int largura, int altura, Color corBorda) {
    Rectangle card = { (float)x, (float)y, (float)largura, (float)altura };
    
    // Fundo do card
    DrawRectangleRec(card, RAYWHITE);
    DrawRectangleLinesEx(card, 2.0f, corBorda);
    
    if (fig.textura.id != 0) {
        float escalaX = (float)(largura - 10) / fig.textura.width;
        float escalaY = (float)(altura - 40) / fig.textura.height;
        float escala = (escalaX < escalaY) ? escalaX : escalaY;
        
        int imgLargura = (int)(fig.textura.width * escala);
        // int imgAltura = (int)(fig.textura.height * escala);
        int imgX = x + (largura - imgLargura) / 2;
        int imgY = y + 10;
        
        DrawTextureEx(fig.textura, (Vector2){(float)imgX, (float)imgY}, 0.0f, escala, WHITE);
    }
    
    // Rodapé com nome/código
    DrawRectangle(x, y + altura - 25, largura, 25, corBorda);
    int tw = MeasureText(fig.codigo, 15);
    DrawText(fig.codigo, x + (largura - tw)/2, y + altura - 20, 15, WHITE);
}

void gerenciarTrocas(Figurinha *album, int total,
                     Jogador *player, Tela *telaAtual) {
    (void)player;

    static EstadoTroca estado = BUSCANDO_OFERTA;
    static int indiceOferecido = -1;
    static int indiceDesejado = -1;
    static float timerMsg = 0;

    if (IsKeyPressed(KEY_BACKSPACE)) {
        estado = BUSCANDO_OFERTA;
        *telaAtual = TELA_MENU;
    }

    // Fundo elegante
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));
    DrawRectangle(150, 80, 980, 560, (Color){25, 25, 35, 255});
    DrawRectangleLinesEx((Rectangle){150, 80, 980, 560}, 2, GOLD);

    DrawText("MERCADO DE TROCAS", 480, 110, 35, GOLD);

    switch (estado) {
        case BUSCANDO_OFERTA: {
            DrawText("O Jogador Virtual esta analisando seu album...", 420, 340, 20, LIGHTGRAY);
            
            // Simulação de busca
            if (GetTime() > timerMsg) {
                int temRepetida = 0;
                for (int i = 0; i < total; i++) {
                    if (album[i].repetidas > 0) {
                        temRepetida = 1;
                        indiceOferecido = i;
                        break;
                    }
                }

                if (!temRepetida) {
                    estado = SEM_REPETIDAS;
                } else {
                    int tentativas = 0;
                    indiceDesejado = -1;
                    while (tentativas < 200) {
                        int r = rand() % total;
                        if (album[r].colada == 0) {
                            indiceDesejado = r;
                            break;
                        }
                        tentativas++;
                    }

                    if (indiceDesejado != -1) {
                        estado = MOSTRANDO_OFERTA;
                    } else {
                        estado = SEM_REPETIDAS; // Nao há nada novo para ele te dar
                    }
                }
            }
        } break;

        case MOSTRANDO_OFERTA: {
            DrawText("VOCE RECEBEU UMA PROPOSTA!", 450, 180, 25, GREEN);

            // Desenhar Figurinha que você ENTREGA (Esquerda)
            DrawText("Voce entrega:", 320, 240, 20, WHITE);
            desenharFigurinhaTroca(album[indiceOferecido], 300, 280, 180, 240, RED);
            DrawText(album[indiceOferecido].titulo, 300, 530, 15, YELLOW);

            // Seta de troca
            DrawText(">>>>>", 600, 380, 40, GOLD);

            // Desenhar Figurinha que você RECEBE (Direita)
            DrawText("Voce recebe:", 780, 240, 20, WHITE);
            desenharFigurinhaTroca(album[indiceDesejado], 760, 280, 180, 240, GREEN);
            DrawText(album[indiceDesejado].titulo, 760, 530, 15, GREEN);

            DrawRectangleRounded((Rectangle){440, 580, 400, 40}, 0.5f, 10, Fade(DARKGRAY, 0.5f));
            DrawText("[T] ACEITAR   [R] RECUSAR", 500, 590, 20, WHITE);

            if (IsKeyPressed(KEY_T)) {
                album[indiceOferecido].repetidas--;
                album[indiceDesejado].colada = 1;
                estado = SUCESSO_TROCA;
                timerMsg = GetTime() + 2.0f;
            }

            if (IsKeyPressed(KEY_R)) {
                estado = BUSCANDO_OFERTA;
                timerMsg = GetTime() + 0.8f;
            }
        } break;

        case SUCESSO_TROCA: {
            DrawText("TROCA REALIZADA COM SUCESSO!", 420, 320, 28, GREEN);
            DrawCircle(640, 420, 50, Fade(GREEN, 0.3f));
            DrawText("V", 625, 400, 50, GREEN);

            if (GetTime() > timerMsg)
                estado = BUSCANDO_OFERTA;
        } break;

        case SEM_REPETIDAS: {
            DrawText("Sem figurinhas repetidas para trocar.", 400, 320, 22, RED);
            DrawText("Abra mais pacotes para conseguir repetidas!", 390, 360, 18, LIGHTGRAY);
            DrawText("[BACKSPACE] Voltar ao Menu", 500, 500, 20, GRAY);
        } break;
    }

    DrawText("[BACKSPACE] Sair", 20, 20, 20, GRAY);
}
