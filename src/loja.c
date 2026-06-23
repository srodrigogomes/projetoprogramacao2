#include "raylib.h"
#include "../include/loja.h"

#define PACOTE_NORMAL 20
#define PACOTE_PREMIUM 50

void gerenciarLoja(Jogador *player, Tela *telaAtual) {

    static int tipoSelecionado = 0;

    // navegação
    if (IsKeyPressed(KEY_LEFT))  tipoSelecionado = 0;
    if (IsKeyPressed(KEY_RIGHT)) tipoSelecionado = 1;

    // Preço calculado internamente na lógica de compra

    // compra
    if (IsKeyPressed(KEY_ENTER)) {

        if (tipoSelecionado == 0 && player->moedas >= PACOTE_NORMAL) {
            player->moedas -= PACOTE_NORMAL;
            player->pacotesDisponiveis++;
        }
        else if (tipoSelecionado == 1 && player->moedas >= PACOTE_PREMIUM) {
            player->moedas -= PACOTE_PREMIUM;
            player->pacotesDisponiveis += 2;
        }
    }

    // sair
    if (IsKeyPressed(KEY_BACKSPACE)) {
        *telaAtual = TELA_MENU;
        return;
    }

    // UI
    DrawRectangle(180, 90, 920, 540, Fade(DARKBLUE, 0.9f));
    DrawRectangleLinesEx((Rectangle){180, 90, 920, 540}, 3, GOLD);

    DrawText("LOJA DE PACOTES", 455, 115, 35, GOLD);

    DrawText(TextFormat("Moedas: %d", player->moedas), 220, 185, 24, YELLOW);
    DrawText(TextFormat("Pacotes: %d", player->pacotesDisponiveis),
             680, 185, 22, SKYBLUE);

    // cards
    Color c1 = (tipoSelecionado == 0) ? GREEN : GRAY;
    Color c2 = (tipoSelecionado == 1) ? GOLD : GRAY;

    DrawRectangle(250, 260, 350, 200, Fade(c1, 0.3f));
    DrawRectangleLinesEx((Rectangle){250, 260, 350, 200}, 3, c1);
    DrawText("PACOTE NORMAL", 290, 280, 22, WHITE);
    DrawText("20 moedas", 290, 320, 20, GOLD);

    DrawRectangle(650, 260, 350, 200, Fade(c2, 0.3f));
    DrawRectangleLinesEx((Rectangle){650, 260, 350, 200}, 3, c2);
    DrawText("PACOTE PREMIUM", 680, 280, 22, WHITE);
    DrawText("50 moedas", 680, 320, 20, GOLD);

    // feedback
    bool podeComprar =
        (tipoSelecionado == 0 && player->moedas >= PACOTE_NORMAL) ||
        (tipoSelecionado == 1 && player->moedas >= PACOTE_PREMIUM);

    DrawText(
        podeComprar ? "[ENTER] Comprar" : "Moedas insuficientes",
        450, 500, 22,
        podeComprar ? GREEN : RED
    );

    DrawText("[BACKSPACE] Voltar", 480, 560, 18, LIGHTGRAY);
}