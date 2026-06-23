#include "raylib.h"
#include "../include/minigame.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/*
    minigame.c

    Esse arquivo implementa o mini-game de pênaltis.

    Ele funciona como uma forma de:
    - gerar interação com o jogador;
    - recompensar moedas;
    - liberar pacotes de figurinhas;
    - integrar a economia do jogo com a jogabilidade.

    Esse módulo é importante porque conecta o sistema
    de habilidade do jogador com o sistema de recompensas.
*/

/*
    Estados internos do mini-game.

    O jogo é controlado por uma máquina de estados simples:
    - ESPERANDO: jogador escolhe direção do chute
    - ANIMANDO:  bola se move em direção ao gol
    - RESULTADO: mostra se foi gol ou defesa
    - FIM_JOGO:  exibe resultado final
*/
typedef enum {
    ESPERANDO,
    ANIMANDO,
    RESULTADO,
    FIM_JOGO
} EstadoPenalti;

/* -----------------------------------------------------------------------
   Constantes de layout
   ----------------------------------------------------------------------- */
#define TELA_W        1280
#define TELA_H        720

/* Gol */
#define GOL_X         390
#define GOL_Y         160
#define GOL_W         500
#define GOL_H         220
#define TRAVE_ESP     12

/* Bola */
#define BOLA_RAIO     18
#define BOLA_INICIO_X 640
#define BOLA_INICIO_Y 560

/* Goleiro */
#define GOL_CORPO_W   50
#define GOL_CORPO_H   90
#define GOL_CAB_RAIO  18

/* Posições X do goleiro por direção */
#define GOLX_ESQ  (GOL_X + 60)
#define GOLX_CEN  (GOL_X + GOL_W/2 - GOL_CORPO_W/2)
#define GOLX_DIR  (GOL_X + GOL_W - 60 - GOL_CORPO_W)
#define GOLY_BASE (GOL_Y + GOL_H - GOL_CORPO_H - 10)

/* -----------------------------------------------------------------------
   Funções auxiliares de desenho
   ----------------------------------------------------------------------- */

/* Desenha o campo de futebol simplificado */
static void desenharCampo(void) {
    /* Gramado principal */
    DrawRectangle(0, 0, TELA_W, TELA_H, (Color){34, 139, 34, 255});

    /* Faixas alternadas do gramado */
    for (int i = 0; i < 8; i++) {
        if (i % 2 == 0)
            DrawRectangle(i * 160, 0, 160, TELA_H, (Color){28, 120, 28, 255});
    }

    /* Área do goleiro (grande área) */
    DrawRectangleLinesEx((Rectangle){290, 130, 700, 310}, 3, (Color){255, 255, 255, 180});

    /* Área pequena */
    DrawRectangleLinesEx((Rectangle){420, 155, 440, 200}, 2, (Color){255, 255, 255, 120});

    /* Marca do pênalti */
    DrawCircle(BOLA_INICIO_X, BOLA_INICIO_Y - 10, 5, WHITE);

    /* Semicírculo da área */
    DrawCircleSector(
        (Vector2){BOLA_INICIO_X, BOLA_INICIO_Y - 10},
        90.0f, 200.0f, 340.0f, 30,
        (Color){255, 255, 255, 80}
    );

    /* Linha do fundo do campo */
    DrawLineEx((Vector2){0, TELA_H - 30}, (Vector2){TELA_W, TELA_H - 30}, 3, WHITE);
}

/* Desenha a trave e rede do gol */
static void desenharGol(void) {
    /* Sombra/profundidade da rede */
    DrawRectangle(GOL_X + TRAVE_ESP, GOL_Y + TRAVE_ESP,
                  GOL_W - TRAVE_ESP * 2, GOL_H - TRAVE_ESP,
                  (Color){0, 0, 0, 80});

    /* Rede (linhas verticais e horizontais) */
    Color corRede = (Color){220, 220, 220, 100};
    for (int x = GOL_X + TRAVE_ESP; x <= GOL_X + GOL_W - TRAVE_ESP; x += 25)
        DrawLine(x, GOL_Y + TRAVE_ESP, x, GOL_Y + GOL_H, corRede);
    for (int y = GOL_Y + TRAVE_ESP; y <= GOL_Y + GOL_H; y += 20)
        DrawLine(GOL_X + TRAVE_ESP, y, GOL_X + GOL_W - TRAVE_ESP, y, corRede);

    /* Postes (brancos com sombra) */
    /* Poste esquerdo */
    DrawRectangle(GOL_X - TRAVE_ESP, GOL_Y, TRAVE_ESP, GOL_H + TRAVE_ESP, DARKGRAY);
    DrawRectangle(GOL_X - TRAVE_ESP, GOL_Y, TRAVE_ESP, GOL_H + TRAVE_ESP, WHITE);
    /* Poste direito */
    DrawRectangle(GOL_X + GOL_W, GOL_Y, TRAVE_ESP, GOL_H + TRAVE_ESP, DARKGRAY);
    DrawRectangle(GOL_X + GOL_W, GOL_Y, TRAVE_ESP, GOL_H + TRAVE_ESP, WHITE);
    /* Travessão */
    DrawRectangle(GOL_X - TRAVE_ESP, GOL_Y - TRAVE_ESP, GOL_W + TRAVE_ESP * 2, TRAVE_ESP, DARKGRAY);
    DrawRectangle(GOL_X - TRAVE_ESP, GOL_Y - TRAVE_ESP, GOL_W + TRAVE_ESP * 2, TRAVE_ESP, WHITE);
}

/* Desenha o goleiro na posição X dada, com cor de camisa */
static void desenharGoleiro(int x, Color corCamisa) {
    int y = GOLY_BASE;

    /* Sombra */
    DrawEllipse(x + GOL_CORPO_W / 2, y + GOL_CORPO_H + 8, 28, 8, (Color){0, 0, 0, 60});

    /* Pernas */
    DrawRectangle(x + 8,  y + GOL_CORPO_H - 20, 14, 28, (Color){30, 30, 80, 255});
    DrawRectangle(x + 28, y + GOL_CORPO_H - 20, 14, 28, (Color){30, 30, 80, 255});

    /* Chuteiras */
    DrawRectangle(x + 6,  y + GOL_CORPO_H + 6, 16, 8, BLACK);
    DrawRectangle(x + 26, y + GOL_CORPO_H + 6, 16, 8, BLACK);

    /* Corpo (camisa) */
    DrawRectangleRounded((Rectangle){(float)x, (float)y, GOL_CORPO_W, GOL_CORPO_H - 20},
                         0.25f, 8, corCamisa);

    /* Número na camisa */
    DrawText("1", x + GOL_CORPO_W / 2 - 5, y + 20, 22, WHITE);

    /* Braços */
    DrawRectangle(x - 14, y + 10, 16, 10, corCamisa);
    DrawRectangle(x + GOL_CORPO_W - 2, y + 10, 16, 10, corCamisa);

    /* Luvas */
    DrawCircle(x - 10, y + 15, 9, (Color){255, 200, 50, 255});
    DrawCircle(x + GOL_CORPO_W + 8, y + 15, 9, (Color){255, 200, 50, 255});

    /* Pescoço */
    DrawRectangle(x + GOL_CORPO_W / 2 - 6, y - GOL_CAB_RAIO - 4, 12, 10,
                  (Color){220, 180, 140, 255});

    /* Cabeça */
    DrawCircle(x + GOL_CORPO_W / 2, y - GOL_CAB_RAIO - 4, GOL_CAB_RAIO,
               (Color){220, 180, 140, 255});

    /* Cabelo */
    DrawCircle(x + GOL_CORPO_W / 2, y - GOL_CAB_RAIO - 10, GOL_CAB_RAIO - 2,
               (Color){80, 50, 20, 255});

    /* Olhos */
    DrawCircle(x + GOL_CORPO_W / 2 - 6, y - GOL_CAB_RAIO - 2, 3, WHITE);
    DrawCircle(x + GOL_CORPO_W / 2 + 6, y - GOL_CAB_RAIO - 2, 3, WHITE);
    DrawCircle(x + GOL_CORPO_W / 2 - 6, y - GOL_CAB_RAIO - 2, 2, BLACK);
    DrawCircle(x + GOL_CORPO_W / 2 + 6, y - GOL_CAB_RAIO - 2, 2, BLACK);
}

/* Desenha a bola de futebol na posição dada */
static void desenharBola(float bx, float by) {
    DrawCircle((int)bx, (int)by, BOLA_RAIO, WHITE);
    DrawCircle((int)bx, (int)by, BOLA_RAIO, (Color){255, 255, 255, 220});

    /* Manchas pretas da bola */
    DrawCircle((int)bx,              (int)by - 8,  5, BLACK);
    DrawCircle((int)bx - 8,          (int)by + 5,  5, BLACK);
    DrawCircle((int)bx + 8,          (int)by + 5,  5, BLACK);

    /* Contorno */
    DrawCircleLines((int)bx, (int)by, BOLA_RAIO, BLACK);
}

/* -----------------------------------------------------------------------
   Lógica do goleiro com "memória" de padrão do jogador
   ----------------------------------------------------------------------- */

/*
    O goleiro usa um sistema de pesos para decidir para onde pular.
    Cada direção tem um contador de quantas vezes o jogador chutou
    para lá. O goleiro tem uma probabilidade maior de ir para a
    direção mais chutada, mas ainda erra para parecer humano.

    Isso torna o goleiro mais interessante do que puro aleatório:
    - Se o jogador chuta sempre para a esquerda, o goleiro começa
      a aprender e vai cada vez mais para lá.
    - Se o jogador varia, o goleiro fica mais equilibrado.
*/
static int sortearDirecaoGoleiro(int histEsq, int histCen, int histDir) {
    /* Total de chutes já realizados */
    int total = histEsq + histCen + histDir;

    if (total == 0) {
        /* Primeira jogada: completamente aleatório */
        return rand() % 3;
    }

    /*
        Pesos: 60% de chance de ir para a direção mais chutada,
        distribuindo os 40% restantes entre as outras duas.
        Isso dá ao goleiro uma leve "inteligência" sem ser injusto.
    */
    int pesos[3];
    pesos[ESQUERDA] = 10 + (histEsq * 60 / total);
    pesos[CENTRO]   = 10 + (histCen * 60 / total);
    pesos[DIREITA]  = 10 + (histDir * 60 / total);

    int soma = pesos[0] + pesos[1] + pesos[2];
    int sorteio = rand() % soma;

    if (sorteio < pesos[ESQUERDA]) return ESQUERDA;
    if (sorteio < pesos[ESQUERDA] + pesos[CENTRO]) return CENTRO;
    return DIREITA;
}

/* -----------------------------------------------------------------------
   Função principal do mini-game
   ----------------------------------------------------------------------- */

/*
    Função principal do mini-game de pênaltis.

    Regras:
    - o jogador tem 5 tentativas;
    - marcar 3 ou mais gols garante recompensa;
    - recompensa: 1 pacote + 30 moedas;
    - o jogador pode sair e voltar ao menu.

    O sistema usa variáveis estáticas para manter estado
    entre os frames da Raylib.
*/
void gerenciarMiniGame(Jogador *player, Tela *telaAtual) {

    /*
        Variáveis estáticas (persistem entre frames)
    */
    static EstadoPenalti estado    = ESPERANDO;
    static int  gols               = 0;
    static int  tentativas         = 5;
    static int  dirJogador         = -1;
    static int  dirGoleiro         = -1;
    static float bolaX             = BOLA_INICIO_X;
    static float bolaY             = BOLA_INICIO_Y;
    static float goleiroX          = GOLX_CEN;
    static float goleiroAlvo       = GOLX_CEN;
    static char  msg[50]           = "";
    static float blinkTimer        = 0.0f;
    static float animTimer         = 0.0f;

    /* Histórico de chutes do jogador (para IA do goleiro) */
    static int histEsq = 0;
    static int histCen = 0;
    static int histDir = 0;

    /* Destino X da bola no gol (calculado ao chutar) */
    static float bolaDestinoX = BOLA_INICIO_X;

    float dt = GetFrameTime();

    /* ----------------------------------------------------------------
       LÓGICA DO JOGO
       ---------------------------------------------------------------- */

    if (estado == ESPERANDO) {

        /* Reseta posição da bola e goleiro para o centro */
        bolaX = BOLA_INICIO_X;
        bolaY = BOLA_INICIO_Y;
        goleiroAlvo = GOLX_CEN;

        /* Jogador escolhe direção do chute */
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
            dirJogador = ESQUERDA;
            histEsq++;
        } else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            dirJogador = CENTRO;
            histCen++;
        } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            dirJogador = DIREITA;
            histDir++;
        }

        if (dirJogador != -1) {
            /* Goleiro decide para onde vai ANTES de ver o chute */
            dirGoleiro = sortearDirecaoGoleiro(histEsq, histCen, histDir);

            /* Define destino X da bola conforme direção do chute */
            if (dirJogador == ESQUERDA)
                bolaDestinoX = GOL_X + 80.0f;
            else if (dirJogador == CENTRO)
                bolaDestinoX = GOL_X + GOL_W / 2.0f;
            else
                bolaDestinoX = GOL_X + GOL_W - 80.0f;

            /* Define posição alvo do goleiro */
            if (dirGoleiro == ESQUERDA)
                goleiroAlvo = GOLX_ESQ;
            else if (dirGoleiro == CENTRO)
                goleiroAlvo = GOLX_CEN;
            else
                goleiroAlvo = GOLX_DIR;

            animTimer = 0.0f;
            estado = ANIMANDO;
        }
    }

    else if (estado == ANIMANDO) {

        animTimer += dt;

        /* Duração da animação: 0.7 segundos */
        float progresso = animTimer / 0.7f;
        if (progresso > 1.0f) progresso = 1.0f;

        /* Bola: trajetória em arco (parábola) */
        bolaX = BOLA_INICIO_X + (bolaDestinoX - BOLA_INICIO_X) * progresso;

        /* Arco vertical: começa em BOLA_INICIO_Y, sobe no meio, chega ao gol */
        float destinoY = (float)(GOL_Y + GOL_H / 2);
        float arco = -120.0f * sinf(progresso * 3.14159f); /* pico no meio */
        bolaY = BOLA_INICIO_Y + (destinoY - BOLA_INICIO_Y) * progresso + arco;

        /* Goleiro se move suavemente para o alvo */
        goleiroX += (goleiroAlvo - goleiroX) * 0.18f;

        /* Quando a animação termina, calcula resultado */
        if (progresso >= 1.0f) {
            estado = RESULTADO;
            tentativas--;
            blinkTimer = 0.0f;

            if (dirJogador != dirGoleiro) {
                gols++;
                player->totalGols++;
                sprintf(msg, "GOOOOOL!");
            } else {
                sprintf(msg, "DEFENDEU!");
            }

            dirJogador = -1;
        }
    }

    else if (estado == RESULTADO) {

        blinkTimer += dt;

        /* Espera jogador avançar para próxima jogada */
        if (IsKeyPressed(KEY_SPACE)) {

            if (tentativas <= 0) {
                estado = FIM_JOGO;

                /* Recompensa final */
                if (gols >= 3) {
                    player->pacotesDisponiveis++;
                    player->moedas += 30;
                }

            } else {
                bolaX = BOLA_INICIO_X;
                bolaY = BOLA_INICIO_Y;
                goleiroX = GOLX_CEN;
                estado = ESPERANDO;
            }
        }
    }

    else if (estado == FIM_JOGO) {

        /* Volta para o menu e reseta o mini-game */
        if (IsKeyPressed(KEY_BACKSPACE)) {
            estado    = ESPERANDO;
            gols      = 0;
            tentativas = 5;
            bolaX     = BOLA_INICIO_X;
            bolaY     = BOLA_INICIO_Y;
            goleiroX  = GOLX_CEN;
            histEsq   = 0;
            histCen   = 0;
            histDir   = 0;
            dirJogador = -1;
            dirGoleiro = -1;

            *telaAtual = TELA_MENU;
        }
    }

    /* ----------------------------------------------------------------
       DESENHO
       ---------------------------------------------------------------- */

    /* Fundo: campo de futebol */
    desenharCampo();

    /* Gol (rede + traves) */
    desenharGol();

    /* Goleiro */
    desenharGoleiro((int)goleiroX, (Color){255, 165, 0, 255}); /* camisa laranja */

    /* Bola */
    desenharBola(bolaX, bolaY);

    /* ── HUD superior ── */
    DrawRectangle(0, 0, TELA_W, 55, (Color){0, 0, 0, 160});
    DrawText("PENALTIS", 20, 12, 28, GOLD);
    DrawText(TextFormat("Gols: %d", gols),         300, 14, 26, GREEN);
    DrawText(TextFormat("Tentativas: %d", tentativas), 500, 14, 26, YELLOW);
    DrawText(TextFormat("Moedas: %d", player->moedas), 750, 14, 26, (Color){255, 215, 0, 255});
    DrawText("[BACKSPACE] Sair", 1000, 18, 20, LIGHTGRAY);

    /* ── Indicadores de direção (estado ESPERANDO) ── */
    if (estado == ESPERANDO) {

        /* Painel de instrução */
        DrawRectangle(300, 620, 680, 70, (Color){0, 0, 0, 170});
        DrawRectangleLinesEx((Rectangle){300, 620, 680, 70}, 2, GOLD);

        DrawText("Escolha a direcao do chute:", 340, 630, 20, WHITE);

        /* Botões de direção */
        Color cEsq = SKYBLUE, cCen = SKYBLUE, cDir = SKYBLUE;
        DrawRectangleRounded((Rectangle){330, 650, 140, 30}, 0.4f, 8, cEsq);
        DrawText("[<- / A]  Esquerda", 335, 655, 17, WHITE);

        DrawRectangleRounded((Rectangle){490, 650, 140, 30}, 0.4f, 8, cCen);
        DrawText("[Cima / W]  Centro", 495, 655, 17, WHITE);

        DrawRectangleRounded((Rectangle){650, 650, 140, 30}, 0.4f, 8, cDir);
        DrawText("[-> / D]  Direita", 655, 655, 17, WHITE);

        /* Seta indicando a bola */
        DrawText("v", BOLA_INICIO_X - 6, BOLA_INICIO_Y - 40, 28, YELLOW);
    }

    /* ── Mensagem de resultado ── */
    if (estado == RESULTADO) {

        int ehGol = (strcmp(msg, "GOOOOOL!") == 0);
        Color corMsg = ehGol ? GREEN : RED;

        /* Painel de resultado */
        DrawRectangle(380, 390, 520, 120, (Color){0, 0, 0, 200});
        DrawRectangleLinesEx((Rectangle){380, 390, 520, 120}, 3, corMsg);

        /* Texto principal */
        int largMsg = MeasureText(msg, 52);
        DrawText(msg, 640 - largMsg / 2, 400, 52, corMsg);

        /* Indicação de onde o goleiro foi */
        const char *nomesDir[] = {"Esquerda", "Centro", "Direita"};
        int idxJog = (dirJogador >= 0 && dirJogador <= 2) ? dirJogador : 1;
        int idxGol = (dirGoleiro >= 0 && dirGoleiro <= 2) ? dirGoleiro : 1;
        DrawText(TextFormat("Voce: %s  |  Goleiro: %s", nomesDir[idxJog], nomesDir[idxGol]),
                 400, 455, 18, LIGHTGRAY);

        /* Prompt piscante */
        if ((int)(blinkTimer * 3.0f) % 2 == 0) {
            DrawText("Pressione [ESPACO] para continuar",
                     420, 490, 20, YELLOW);
        }
    }

    /* ── Tela de fim de jogo ── */
    if (estado == FIM_JOGO) {

        DrawRectangle(0, 0, TELA_W, TELA_H, (Color){0, 0, 0, 200});

        /* Card central */
        DrawRectangleRounded((Rectangle){340, 200, 600, 320}, 0.12f, 12,
                             (Color){20, 20, 40, 240});
        DrawRectangleRoundedLinesEx((Rectangle){340, 200, 600, 320}, 0.12f, 12, 3,
                                   gols >= 3 ? GOLD : GRAY);

        DrawText("FIM DE JOGO!", 490, 225, 42, WHITE);

        DrawText(TextFormat("Gols marcados: %d / 5", gols), 430, 290, 28, GOLD);

        if (gols >= 3) {
            DrawText("PARABENS! Voce ganhou:", 430, 340, 22, GREEN);
            DrawText("+1 Pacote de Figurinhas", 430, 370, 22, GREEN);
            DrawText("+30 Moedas",              430, 398, 22, GREEN);
        } else {
            DrawText("Nao foi dessa vez...", 430, 350, 24, RED);
            DrawText("Precisa de 3 gols para ganhar.", 400, 385, 20, LIGHTGRAY);
        }

        DrawText("Pressione [BACKSPACE] para voltar ao menu",
                 370, 480, 20, LIGHTGRAY);
    }
}
