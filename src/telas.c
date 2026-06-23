#include "../include/telas.h"
#include "../include/album.h"
#include "../include/minigame.h"
#include "../include/estatisticas.h"
#include "../include/loja.h"
#include <stdio.h>

/*
    telas.c

    Esse arquivo funciona como um "roteador de interface gráfica".

    Ou seja:
    ele NÃO implementa lógica de jogo diretamente,
    ele apenas decide QUAL módulo será desenhado na tela
    dependendo do estado atual do sistema (enum Tela).

    Isso ajuda a manter o projeto modular:
    - cada tela tem sua própria responsabilidade
    - evita um main.c muito grande e confuso
*/

/**
 * @brief Despacha o desenho para o módulo responsável pela tela atual
 *
 * Esse é o ponto central da interface gráfica.
 * Aqui o sistema verifica o estado atual (TELA_MENU, TELA_ALBUM, etc)
 * e chama a função correspondente de cada módulo.
 */
void desenharTelaAtual(Tela *telaAtual, Figurinha *album, int totalFigurinhas,
                       Jogador *player, int *paginaAtual) {

    switch (*telaAtual) {

        /*
            TELA MENU
            Aqui é a tela inicial do jogo.

            Mostra:
            - título do sistema
            - informações básicas do jogador
            - opções de navegação

            Essa tela NÃO chama outros módulos,
            apenas exibe informações básicas.
        */
        case TELA_MENU: {
            DrawText("ALBUM COPA 2026", 400, 150, 50, WHITE);

            DrawText(TextFormat("Moedas: %d | Pacotes: %d",
                     player->moedas, player->pacotesDisponiveis),
                     480, 240, 22, GOLD);

            DrawText("[ENTER] Abrir Album",     480, 310, 20, LIGHTGRAY);
            DrawText("[P]     Abrir Pacote",    480, 345, 20, LIGHTGRAY);
            DrawText("[M]     Jogar Penaltis",  480, 380, 20, LIGHTGRAY);
            DrawText("[E]     Estatisticas",    480, 415, 20, LIGHTGRAY);
            DrawText("[L]     Loja de Pacotes", 480, 450, 20, LIGHTGRAY);
            DrawText("[ESC]   Sair e Salvar",   480, 485, 20, LIGHTGRAY);
        } break;

        /*
            TELA ALBUM

            Aqui o sistema chama a função do módulo album.c:
            desenharGrelhaAlbum()

            Essa função:
            - percorre o vetor de figurinhas
            - desenha cards
            - mostra quais já foram coladas
            - controla visual de progresso do álbum
        */
        case TELA_ALBUM: {

            DrawRectangle(80, 50, 1120, 600,
                         (Color){245, 245, 220, 255});

            desenharGrelhaAlbum(album, totalFigurinhas, *paginaAtual);

            int totalPaginas = totalFigurinhas / 8;

            DrawText(TextFormat("Pagina %d de %d",
                     *paginaAtual + 1, totalPaginas + 1),
                     580, 670, 20, WHITE);

            DrawText("[<-][->] Navegar | [BACKSPACE] Voltar",
                     430, 20, 18, LIGHTGRAY);

        } break;

        /*
            TELA PACOTE (versão simples/visual)

            Essa versão aqui é mais visual e não contém lógica completa.

            Na versão real do projeto, a abertura de pacotes
            é controlada pelo pacote.c (gerenciarAberturaPacote),
            mas aqui existe uma versão simplificada com barra de progresso.
        */
        case TELA_PACOTE: {

            DrawRectangle(0, 0, 1280, 720, Fade(BLACK, 0.8f));
            DrawText("ABRINDO PACOTE...", 450, 200, 40, GOLD);

            DrawRectangleLines(440, 300, 400, 50, WHITE);

            static float progresso = 0;

            if (progresso < 1.0f)
                progresso += 0.015f;

            DrawRectangle(445, 305, (int)(progresso * 390),
                         40, LIME);

            if (progresso >= 1.0f) {

                DrawText("7 Figurinhas adicionadas ao album!",
                         420, 400, 20, WHITE);

                DrawText("Pressione ESPACO para voltar",
                         480, 500, 20, LIGHTGRAY);

                if (IsKeyPressed(KEY_SPACE)) {
                    progresso = 0;
                    *telaAtual = TELA_MENU;
                }
            }

        } break;

        /*
            TELA MINI-GAME

            Aqui o controle é delegado para:
            minigame.c → gerenciarMiniGame()

            Esse módulo é responsável por:
            - lógica do jogo
            - animação da bola
            - pontuação
            - recompensa do jogador
        */
        case TELA_MINIGAME: {
            gerenciarMiniGame(player, telaAtual);
        } break;

        /*
            TELA ESTATÍSTICAS

            Aqui é exibido o progresso geral do álbum.

            Função chamada:
            estatisticas.c → desenharEstatisticas()

            Responsável por:
            - porcentagem de conclusão
            - contagem de repetidas
            - figurinhas faltando
        */
        case TELA_ESTATS: {
            desenharEstatisticas(album, totalFigurinhas);
        } break;

        /*
            TELA LOJA

            Aqui o jogador usa moedas para comprar pacotes.

            A lógica completa está em:
            loja.c → gerenciarLoja()

            Esse módulo controla:
            - economia do jogo
            - compra de pacotes
            - validação de saldo
        */
        case TELA_LOJA: {
            gerenciarLoja(player, telaAtual);
        } break;

        /*
            Caso padrão:
            Evita comportamento indefinido se uma tela inválida aparecer
        */
        default:
            break;
    }
}