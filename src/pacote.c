#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "../include/pacote.h"

/*
    pacote.c

    Esse arquivo implementa o sistema de abertura de pacotes de figurinhas.

    Ele possui duas versões:
    - versão gráfica (Raylib)
    - versão em console (CRUD/testes)

    A principal função do módulo é simular o “sorteio” de figurinhas,
    atualizando automaticamente o álbum do jogador.
*/

/*
    Função auxiliar de sorteio.

    Ela escolhe uma figurinha aleatória do álbum.

    Existe uma pequena lógica de raridade:
    - figurinhas raras têm menor chance de aparecer
    - isso simula um sistema de probabilidade simples
*/
static int sortearIndice(Figurinha *album, int total) {

    int idx = rand() % total;

    // tentativa de reduzir chance de raras
    if (album[idx].raridade > 0 && (rand() % 100) < 70) {
        idx = rand() % total;
    }

    return idx;
}

/*
    Função principal de abertura de pacote (versão gráfica).

    Responsável por:
    - sortear 7 figurinhas;
    - atualizar o álbum automaticamente;
    - marcar novas e repetidas;
    - dar recompensa visual ao jogador;
    - controlar o fluxo de animação (uma por vez).
*/
void gerenciarAberturaPacote(Figurinha *album, int total,
                             Jogador *player, Tela *telaAtual) {

    /*
        Variáveis estáticas

        Mantêm o estado da abertura entre frames da Raylib.
    */
    static float delay = 0;
    static int indices[7] = {-1, -1, -1, -1, -1, -1, -1};
    static bool nova[7] = {false};
    static int figurinhaAtual = -1;
    static bool pacoteSorteado = false;
    static int novas = 0;
    static int repetidas = 0;

    /*
        Inicializa o pacote quando o jogador abre pela primeira vez
    */
    if (!pacoteSorteado && player->pacotesDisponiveis > 0) {

        for (int i = 0; i < 7; i++) {

            indices[i] = sortearIndice(album, total);

            // verifica se já tinha a figurinha
            if (album[indices[i]].colada == 0) {
                nova[i] = true;
                novas++;
                album[indices[i]].colada = 1; // cola automaticamente
            } else {
                nova[i] = false;
                album[indices[i]].repetidas++; // vai para repetidas
            }
        }

        player->pacotesDisponiveis--;

        pacoteSorteado = true;
        figurinhaAtual = 0;

        // Timer removido por não ser utilizado na lógica atual
    }

    /*
        Fundo escuro da abertura
    */
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                  Fade(BLACK, 0.85f));

    DrawText("ABRINDO PACOTE...", 480, 50, 30, GOLD);

    /*
        Mostra cada figurinha uma por vez
    */
    if (figurinhaAtual >= 0 && figurinhaAtual < 7) {

        int idx = indices[figurinhaAtual];

        Rectangle card = { 490, 150, 300, 420 };

        Color corCard = nova[figurinhaAtual]
            ? (Color){50, 150, 50, 255}
            : (Color){150, 50, 50, 255};

        DrawRectangleRec(card, corCard);
        DrawRectangleLinesEx(card, 3, GOLD);

        // Se houver textura, exibe a imagem no centro do card
        if (album[idx].textura.id != 0) {
            float escalaX = (float)(card.width - 40) / album[idx].textura.width;
            float escalaY = (float)(card.height - 150) / album[idx].textura.height;
            float escala = (escalaX < escalaY) ? escalaX : escalaY;
            
            int imgLargura = (int)(album[idx].textura.width * escala);
            int imgAltura = (int)(album[idx].textura.height * escala);
            int imgX = (int)card.x + (int)(card.width - imgLargura) / 2;
            int imgY = (int)card.y + 100 + ((int)(card.height - 200 - imgAltura) / 2);
            
            DrawTextureEx(album[idx].textura, (Vector2){(float)imgX, (float)imgY}, 0.0f, escala, WHITE);
        }

        DrawText(album[idx].codigo,  card.x + 20, card.y + 20, 20, WHITE);
        DrawText(album[idx].titulo,  card.x + 20, card.y + 50, 22, YELLOW);
        DrawText(album[idx].selecao, card.x + 20, card.y + 80, 18, LIGHTGRAY);
        DrawText(album[idx].tipo,    card.x + 20, card.y + 380, 18,
                 nova[figurinhaAtual] ? GREEN : MAROON);

        /*
            Feedback visual do resultado
        */
        if (nova[figurinhaAtual]) {
            if (album[idx].textura.id != 0) DrawRectangle(card.x, card.y + 150, card.width, 100, Fade(BLACK, 0.5f));
            DrawText("NOVA!", card.x + 100, card.y + 170, 40, GREEN);
            DrawText("Colada no album!", card.x + 60, card.y + 210, 20, WHITE);

        } else {
            if (album[idx].textura.id != 0) DrawRectangle(card.x, card.y + 150, card.width, 100, Fade(BLACK, 0.5f));
            DrawText("REPETIDA", card.x + 60, card.y + 170, 40, RED);
            DrawText("Enviada para trocas", card.x + 50, card.y + 210, 20, LIGHTGRAY);
        }

        DrawText(TextFormat("Figurinha %d de 7", figurinhaAtual + 1),
                 560, 600, 20, WHITE);

        static float blink = 0.0f;
blink += GetFrameTime() * 5.0f;

if ((int)blink % 2 == 0) {
    DrawText("Pressione [ESPACO] para proxima", 490, 650, 20, GRAY);
}

        /*
            Avança para próxima figurinha
        */
        delay += GetFrameTime();

        if (delay >= 0.5f && IsKeyPressed(KEY_SPACE)) {
        figurinhaAtual++;
         delay = 0;
}
    }

    /*
        Fim do pacote
    */
    else if (figurinhaAtual >= 7) {

        DrawText("PACOTE CONCLUIDO!", 460, 260, 40, GOLD);

        DrawText(TextFormat("Resumo: %d novas / %d repetidas", novas, repetidas),
         430, 340, 25, WHITE);

        DrawText("Pressione [ESPACO] para voltar ao menu", 440, 420, 22, LIGHTGRAY);

        if (IsKeyPressed(KEY_SPACE)) {
            pacoteSorteado = false;
            figurinhaAtual = -1;
            *telaAtual = TELA_MENU;
            novas = 0;
        repetidas = 0;
        }
    }

    /*
        Saída forçada (BACKSPACE)
    */
    if (IsKeyPressed(KEY_BACKSPACE)) {
        pacoteSorteado = false;
        figurinhaAtual = -1;
        *telaAtual = TELA_MENU;
    }
}

/*
    Versão em console do sistema de abertura de pacotes.

    Usada para testes e integração com o CRUD.
*/
void abrirPacoteConsole(Figurinha *album, int total, Jogador *player) {

    if (player->pacotesDisponiveis <= 0) {
        printf("Voce nao tem pacotes disponiveis!\n");
        return;
    }

    printf("\n\033[1;33m=== ABRINDO PACOTE! ===\033[0m\n");

    for (int i = 0; i < 7; i++) {

        int idx = sortearIndice(album, total);

        printf("\033[1;36m-- Figurinha %d/7 --\033[0m\n", i + 1);
        printf("  Codigo : %s | Titulo : %s\n",
               album[idx].codigo, album[idx].titulo);

        if (album[idx].colada) {

            printf("  \033[1;31mREPETIDA! Enviada para trocas.\033[0m\n");
            album[idx].repetidas++;

        } else {

            album[idx].colada = 1;
            printf("  \033[1;32mNOVA! Colada no album.\033[0m\n");
        }
    }

    player->pacotesDisponiveis--;
}