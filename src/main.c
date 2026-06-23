#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "raylib.h"
#include "../include/figurinha.h"
#include "../include/jogador.h"
#include "../include/telas.h"
#include "../include/album.h"
#include "../include/pacote.h"
#include "../include/minigame.h"
#include "../include/estatisticas.h"
#include "../include/arquivo.h"
#include "../include/loja.h"
#include "../include/troca.h"

// Cores ANSI para o menu de console
#define ANSI_RESET   "\033[0m"
#define ANSI_NEGRITO "\033[1m"
#define ANSI_VERDE   "\033[1;32m"
#define ANSI_AMARELO "\033[1;33m"
#define ANSI_AZUL    "\033[1;34m"
#define ANSI_CIANO   "\033[1;36m"
#define ANSI_VERMELHO "\033[1;31m"
#define ANSI_BRANCO  "\033[1;37m"

#define TOTAL_PARTICULAS_MENU 90
#define TOTAL_BOTOES_MENU 8

// Variáveis de efeito removidas para evitar warnings não utilizados

typedef struct {
    Vector2 posicao;
    Vector2 velocidade;
    float tamanho;
    float rotacao;
    float giro;
    Color cor;
    int confete;
} ParticulaMenu;

static float numeroAleatorio(float minimo, float maximo) {
    return minimo + ((float)rand() / (float)RAND_MAX) * (maximo - minimo);
}

static void reiniciarParticulaMenu(ParticulaMenu *p, int topo) {
    p->posicao.x = numeroAleatorio(20.0f, 770.0f);
    p->posicao.y = topo ? numeroAleatorio(-80.0f, -5.0f) : numeroAleatorio(0.0f, 720.0f);
    p->velocidade.x = numeroAleatorio(-10.0f, 18.0f);
    p->velocidade.y = numeroAleatorio(12.0f, 42.0f);
    p->tamanho = numeroAleatorio(2.0f, 6.0f);
    p->rotacao = numeroAleatorio(0.0f, 360.0f);
    p->giro = numeroAleatorio(-80.0f, 80.0f);
    p->confete = rand() % 3 == 0;
    p->cor = p->confete ? (Color){255, (unsigned char)(120 + rand() % 120), 70, 190}
                        : (Color){255, 215, 90, (unsigned char)(120 + rand() % 100)};
}



static void inicializarParticulasMenu(ParticulaMenu particulas[]) {
    for (int i = 0; i < TOTAL_PARTICULAS_MENU; i++) {
        reiniciarParticulaMenu(&particulas[i], 0);
    }
}

static void desenharParticulasMenu(ParticulaMenu particulas[]) {
    float dt = GetFrameTime();
    float tempo = (float)GetTime();

    for (int i = 0; i < TOTAL_PARTICULAS_MENU; i++) {
        ParticulaMenu *p = &particulas[i];
        p->posicao.x += (p->velocidade.x + sinf(tempo + i * 0.35f) * 16.0f) * dt;
        p->posicao.y += p->velocidade.y * dt;
        p->rotacao += p->giro * dt;

        if (p->posicao.y > 735.0f || p->posicao.x < -30.0f || p->posicao.x > 810.0f) {
            reiniciarParticulaMenu(p, 1);
        }

        if (p->confete) {
            Rectangle confete = {p->posicao.x, p->posicao.y, p->tamanho * 1.8f, p->tamanho * 0.8f};
            DrawRectanglePro(confete, (Vector2){confete.width / 2.0f, confete.height / 2.0f},
                             p->rotacao, p->cor);
        } else {
            DrawCircleV(p->posicao, p->tamanho, p->cor);
            DrawCircleV(p->posicao, p->tamanho * 2.4f, Fade(GOLD, 0.12f));
        }
    }
}

static void executarAcaoMenu(int indice, Tela *telaAtual, Jogador *player) {
    switch (indice) {
        case 0: *telaAtual = TELA_ABERTURA_ALBUM; break;
        case 1:
            if (player->pacotesDisponiveis > 0) *telaAtual = TELA_PACOTE;
            break;
        case 2: *telaAtual = TELA_MINIGAME; break;
        case 3: *telaAtual = TELA_ESTATS; break;
        case 4: *telaAtual = TELA_LOJA; break;
        case 5: *telaAtual = TELA_TROCA; break;
        case 6: *telaAtual = TELA_CONTROLE; break;
        case 7: *telaAtual = TELA_SAIR; break;
        default: break;
    }
}

static void desenharBotaoMenu(Rectangle base, const char *atalho, const char *texto,
                              Color cor, int habilitado, int selecionado,
                              int indice, int *indiceSelecionado,
                              Tela *telaAtual, Jogador *player, float animacao[]) {
    Vector2 mouse = GetMousePosition();
    int mouseSobre = CheckCollisionPointRec(mouse, base);
    int ativo = habilitado && (selecionado || mouseSobre);
    float alvo = ativo ? 1.0f : 0.0f;

    animacao[indice] += (alvo - animacao[indice]) * 0.18f;
    if (mouseSobre && habilitado) {
        *indiceSelecionado = indice;
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }

    float escala = 1.0f + animacao[indice] * 0.055f;
    Rectangle botao = {
        base.x - (base.width * (escala - 1.0f)) / 2.0f,
        base.y - (base.height * (escala - 1.0f)) / 2.0f,
        base.width * escala,
        base.height * escala
    };

    Color corBase = habilitado ? cor : GRAY;
    DrawRectangleRounded(botao, 0.3f, 10, Fade(corBase, 0.10f + animacao[indice] * 0.18f));
    if (ativo) {
        DrawRectangleRounded((Rectangle){botao.x - 8, botao.y - 8, botao.width + 16, botao.height + 16},
                             0.3f, 10, Fade(corBase, 0.16f));
        DrawRectangleRoundedLinesEx(botao, 0.3f, 10, 2.0f, Fade(corBase, 0.9f));
    }

    DrawText(atalho, (int)botao.x + 15, (int)botao.y + 10, 20, habilitado ? corBase : DARKGRAY);
    DrawText(texto, (int)botao.x + 110, (int)botao.y + 10, 20, habilitado ? WHITE : GRAY);

    if (habilitado && mouseSobre && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        executarAcaoMenu(indice, telaAtual, player);
    }
}

/**
 * @brief Exibe o menu principal no console com cores e aguarda a escolha do usuário
 * @param pacotes Quantidade de pacotes disponíveis (exibida no menu)
 * @return Opção escolhida pelo usuário
 */
static int exibirMenu(int pacotes) {
    int opcao = -1;
    printf("\a"); // beep

    printf(ANSI_AMARELO
       "\n+--------------------------------------+\n"
       "|   ALBUM DE FIGURINHAS - COPA 2026    |\n"
       "|          UTFPR - Apucarana           |\n"
       "+--------------------------------------+\n"
       ANSI_RESET);

    printf(ANSI_CIANO "  1." ANSI_RESET " Listar Figurinhas\n");
    printf(ANSI_CIANO "  2." ANSI_RESET " Cadastrar Figurinha\n");
    printf(ANSI_CIANO "  3." ANSI_RESET " Pesquisar Figurinha\n");
    printf(ANSI_CIANO "  4." ANSI_RESET " Alterar Figurinha\n");
    printf(ANSI_CIANO "  5." ANSI_RESET " Remover Figurinha\n");
    printf(ANSI_CIANO "  6." ANSI_RESET " Abrir Pacote "
           ANSI_AMARELO "(Pacotes disponiveis: %d)" ANSI_RESET "\n", pacotes);
    printf(ANSI_CIANO "  7." ANSI_RESET " Exportar para CSV\n");
    printf(ANSI_VERDE  "  8." ANSI_NEGRITO " INICIAR INTERFACE GRAFICA (Raylib)" ANSI_RESET "\n");
    printf(ANSI_VERMELHO "  0." ANSI_RESET " Sair e Salvar\n");
    printf(ANSI_BRANCO "Escolha: " ANSI_RESET);

    if (scanf("%d", &opcao) != 1) {
        // Limpa o buffer em caso de entrada inválida
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return -1;
    }
    return opcao;
}

int main(void) {
    srand((unsigned int)time(NULL));

    // Carregamento de dados (binário primeiro; fallback para CSV)
    Figurinha *album = NULL;
    int totalFigurinhas = 0;

    if (carregarDadosGerais(&album, &totalFigurinhas) == 0) {
        printf(ANSI_VERMELHO
               "Erro fatal: arquivos de dados nao encontrados.\n"
               ANSI_RESET);
        return 1;
    }

    printf(ANSI_VERDE "%d figurinhas carregadas.\n" ANSI_RESET, totalFigurinhas);

    Jogador player;
    carregarDadosJogador(&player); // Carrega os dados do jogador ou inicializa com padroes

    int opcao = -1;

    // ── MENU DE CONSOLE (CRUD obrigatório) ──────────────────────────────────
    do {
        opcao = exibirMenu(player.pacotesDisponiveis);

        switch (opcao) {
            case 1: listarFigurinhas(album, totalFigurinhas);             break;
            case 2: inserirFigurinha(&album, &totalFigurinhas);           break;
            case 3: pesquisarFigurinha(album, totalFigurinhas);           break;
            case 4: alterarFigurinha(album, totalFigurinhas);             break;
            case 5: removerFigurinha(&album, &totalFigurinhas);           break;
            case 6: abrirPacoteConsole(album, totalFigurinhas, &player);  break;
            case 7: exportarCSV(album, totalFigurinhas);                  break;
            case 8: /* sai do loop e entra na interface gráfica */        break;
            case 0:
                salvarDadosBinario(album, totalFigurinhas);
                salvarDadosJogador(&player);
                free(album);
                printf(ANSI_VERDE "Dados salvos. Ate logo!\n" ANSI_RESET);
                return 0;
            default:
                printf(ANSI_VERMELHO
                       "Opcao invalida. Tente novamente.\n"
                       ANSI_RESET);
                break;
        }
    } while (opcao != 8);

    // ── INTERFACE GRÁFICA RAYLIB ─────────────────────────────────────────────
    Tela   telaAtual   = TELA_MENU;
    int    paginaAtual = 0;
    int    scrollControle = 0;
    char   textoBusca[20] = "";
    int    totalPaginas = totalFigurinhas / 8;

	    InitWindow(1280, 720, "UTFPR - Album da Copa 2026");
	    InitAudioDevice(); // Inicializa o dispositivo de áudio
	    SetTargetFPS(60);
	
	    // Carrega a imagem de fundo. Se falhar, o jogo continua com fundo verde.
	    Texture2D fundoMenu = LoadTexture("assets/fundo_menu.png");
	    Texture2D capaAlbum = LoadTexture("assets/capa_album.png");
	    
    // Carrega sons e música com múltiplas tentativas de caminho
    const char* caminhosMusica[] = {"assets/sounds/musica_fundo.mp3", "sounds/musica_fundo.mp3", "musica_fundo.mp3"};
    const char* caminhosClique[] = {"assets/sounds/clique.mp3", "sounds/clique.mp3", "clique.mp3"};
    const char* caminhosPagina[] = {"assets/sounds/virar_pagina.mp3", "sounds/virar_pagina.mp3", "virar_pagina.mp3"};

    Music musicaFundo = {0};
    for(int i=0; i<3; i++) {
        musicaFundo = LoadMusicStream(caminhosMusica[i]);
        if (musicaFundo.stream.buffer != NULL) break;
    }

    Sound somClique = {0};
    for(int i=0; i<3; i++) {
        somClique = LoadSound(caminhosClique[i]);
        if (somClique.frameCount > 0) break;
    }

    Sound somVirarPagina = {0};
    for(int i=0; i<3; i++) {
        somVirarPagina = LoadSound(caminhosPagina[i]);
        if (somVirarPagina.frameCount > 0) break;
    }
    
    if (IsAudioDeviceReady()) {
        if (musicaFundo.stream.buffer != NULL) {
            PlayMusicStream(musicaFundo);
            SetMusicVolume(musicaFundo, 0.5f);
            TraceLog(LOG_INFO, "Musica de fundo carregada com sucesso.");
        } else TraceLog(LOG_WARNING, "Falha ao carregar musica_fundo.mp3");
        
        if (somClique.frameCount == 0) TraceLog(LOG_WARNING, "Falha ao carregar clique.mp3");
        if (somVirarPagina.frameCount == 0) TraceLog(LOG_WARNING, "Falha ao carregar virar_pagina.mp3");
    } else {
        TraceLog(LOG_ERROR, "Dispositivo de audio nao esta pronto!");
    }
    if (capaAlbum.id == 0) capaAlbum = LoadTexture("capa_album.png"); // Tenta na raiz se falhar em assets
    
    // Carregamento dinâmico de imagens das figurinhas (DEVE ser após InitWindow)
    TraceLog(LOG_INFO, "Carregando imagens das figurinhas...");
    for (int i = 0; i < totalFigurinhas; i++) {
        char path[100];
        sprintf(path, "assets/figurinhas/%s.png", album[i].codigo);
        
        if (FileExists(path)) {
            album[i].textura = LoadTexture(path);
        } else {
            album[i].textura.id = 0; // Garante que o ID seja 0 se não houver imagem
        }
    }
    TraceLog(LOG_INFO, "Carregamento de imagens concluido.");

    float timerAbertura = 0.0f;
    if (fundoMenu.id == 0) TraceLog(LOG_WARNING, "Erro ao carregar fundo_menu.png");
    if (capaAlbum.id == 0) TraceLog(LOG_WARNING, "AVISO: capa_album.png nao encontrada em assets/ ou na raiz.");

    ParticulaMenu particulasMenu[TOTAL_PARTICULAS_MENU];
    float animacaoMenu[TOTAL_BOTOES_MENU] = {0};
    int indiceMenuSelecionado = 0;
    inicializarParticulasMenu(particulasMenu);

	    SetExitKey(KEY_NULL); // Desabilita o ESC fechar o programa globalmente
		    while (!WindowShouldClose() && telaAtual != TELA_SAIR) {
		        // Toca a música no Menu e na Introdução, mas para no Álbum e outras telas
		        if (telaAtual == TELA_MENU || telaAtual == TELA_ABERTURA_ALBUM) {
		            if (!IsMusicStreamPlaying(musicaFundo)) PlayMusicStream(musicaFundo);
		            UpdateMusicStream(musicaFundo);
		        } else {
		            if (IsMusicStreamPlaying(musicaFundo)) StopMusicStream(musicaFundo);
		        }
		        
		        if (telaAtual == TELA_MENU && IsKeyPressed(KEY_ESCAPE)) telaAtual = TELA_SAIR;
        // Salva os dados do jogador e do álbum antes de fechar a janela Raylib
        if (WindowShouldClose() || telaAtual == TELA_SAIR) {
            salvarDadosBinario(album, totalFigurinhas);
            salvarDadosJogador(&player);
        }

        // ── Entradas de navegação ────────────────────────────────────────────
        if (telaAtual == TELA_MENU) {
	            if (IsKeyPressed(KEY_DOWN)) {
	                indiceMenuSelecionado = (indiceMenuSelecionado + 1) % TOTAL_BOTOES_MENU;
	                PlaySound(somClique);
	            }
	            if (IsKeyPressed(KEY_UP)) {
	                indiceMenuSelecionado = (indiceMenuSelecionado - 1 + TOTAL_BOTOES_MENU) % TOTAL_BOTOES_MENU;
	                PlaySound(somClique);
	            }
	            if (IsKeyPressed(KEY_ENTER)) {
	                PlaySound(somClique);
	                executarAcaoMenu(indiceMenuSelecionado, &telaAtual, &player);
	            }
            if (IsKeyPressed(KEY_E))     telaAtual = TELA_ESTATS;
            if (IsKeyPressed(KEY_M))     telaAtual = TELA_MINIGAME;
            if (IsKeyPressed(KEY_L))     telaAtual = TELA_LOJA;
            if (IsKeyPressed(KEY_T))     telaAtual = TELA_TROCA;
            if (IsKeyPressed(KEY_P) && player.pacotesDisponiveis > 0) {
                telaAtual = TELA_PACOTE;
            }
	        } else if (telaAtual == TELA_ALBUM) {
	            if (IsKeyPressed(KEY_RIGHT) && paginaAtual < totalPaginas) {
	                paginaAtual++;
	                PlaySound(somVirarPagina);
	            }
	            if (IsKeyPressed(KEY_LEFT)  && paginaAtual > 0) {
	                paginaAtual--;
	                PlaySound(somVirarPagina);
	            }
	            if (IsKeyPressed(KEY_BACKSPACE)) {
	                PlaySound(somClique);
	                telaAtual = TELA_MENU;
	            }
        } else if (telaAtual == TELA_ESTATS) {
            if (IsKeyPressed(KEY_BACKSPACE)) telaAtual = TELA_MENU;
        } else if (telaAtual == TELA_CONTROLE) {
            if (IsKeyPressed(KEY_ESCAPE)) telaAtual = TELA_MENU;
        }
        // TELA_MINIGAME e TELA_LOJA gerenciam sua própria navegação internamente

        // ── Desenho ──────────────────────────────────────────────────────────
        BeginDrawing();
            ClearBackground(DARKGREEN);
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);

           switch (telaAtual) {

    case TELA_MENU: {
        DrawTexture(fundoMenu, 0, 0, WHITE);
        desenharParticulasMenu(particulasMenu);

        int menuX = 830;

        DrawRectangle(menuX - 30, 0, 480, 720, Fade(BLACK, 0.7f));
        DrawRectangleGradientH(menuX - 100, 0, 70, 720, Fade(BLACK, 0.0f), Fade(BLACK, 0.7f));
        DrawLineEx((Vector2){menuX - 30, 0}, (Vector2){menuX - 30, 720}, 3, GOLD);

        DrawText("ALBUM COPA 2026", menuX + 2, 82, 38, BLACK);
        DrawText("ALBUM COPA 2026", menuX, 80, 38, GOLD);

        DrawRectangle(menuX, 130, 350, 3, GOLD);

        DrawRectangleRounded((Rectangle){(float)menuX, 160, 350, 80}, 0.2f, 10, Fade(DARKGRAY, 0.3f));
        DrawText("STATUS DO COLECIONADOR", menuX + 15, 170, 15, LIGHTGRAY);
        DrawText(TextFormat("Moedas: %d", player.moedas), menuX + 15, 195, 24, YELLOW);
        DrawText(TextFormat("Pacotes: %d", player.pacotesDisponiveis), menuX + 180, 195, 24, GREEN);

        int startY = 300;
        int spacing = 50;

        Rectangle botao = {(float)menuX, (float)startY, 350, 40};

        desenharBotaoMenu(botao, "[ENTER]", "Visualizar Album", GOLD, 1,
            indiceMenuSelecionado == 0, 0,
            &indiceMenuSelecionado, &telaAtual, &player, animacaoMenu);

        botao.y += spacing;
        desenharBotaoMenu(botao, "[P]", "Abrir Pacote", GREEN,
            player.pacotesDisponiveis > 0,
            indiceMenuSelecionado == 1, 1,
            &indiceMenuSelecionado, &telaAtual, &player, animacaoMenu);

        botao.y += spacing;
        desenharBotaoMenu(botao, "[M]", "Mini-game Penaltis", SKYBLUE, 1,
            indiceMenuSelecionado == 2, 2,
            &indiceMenuSelecionado, &telaAtual, &player, animacaoMenu);

        botao.y += spacing;
        desenharBotaoMenu(botao, "[E]", "Estatisticas", ORANGE, 1,
            indiceMenuSelecionado == 3, 3,
            &indiceMenuSelecionado, &telaAtual, &player, animacaoMenu);

        botao.y += spacing;
        desenharBotaoMenu(botao, "[L]", "Loja de Pacotes", PINK, 1,
            indiceMenuSelecionado == 4, 4,
            &indiceMenuSelecionado, &telaAtual, &player, animacaoMenu);

        botao.y += spacing;
        desenharBotaoMenu(botao, "[T]", "Sistema de Trocas", LIME, 1,
            indiceMenuSelecionado == 5, 5,
            &indiceMenuSelecionado, &telaAtual, &player, animacaoMenu);

        botao.y += spacing;
        desenharBotaoMenu(botao, "[C]", "Controle Figurinhas", VIOLET, 1,
            indiceMenuSelecionado == 6, 6,
            &indiceMenuSelecionado, &telaAtual, &player, animacaoMenu);

        botao.y += spacing + 20;
        desenharBotaoMenu(botao, "[ESC]", "Sair do Jogo", RED, 1,
            indiceMenuSelecionado == 7, 7,
            &indiceMenuSelecionado, &telaAtual, &player, animacaoMenu);

        DrawText("UTFPR - Apucarana 2026", menuX + 80, 700, 15, GRAY);

    } break;

                    case TELA_ABERTURA_ALBUM: {
                        timerAbertura += GetFrameTime();
                        float progresso = timerAbertura / 2.5f; // 2.5 segundos de animação
                        if (progresso > 1.0f) {
                            telaAtual = TELA_ALBUM;
                            timerAbertura = 0.0f;
                        }

                        // Desenha fundo do álbum
                        DrawRectangle(80, 50, 1120, 600, (Color){245, 245, 220, 255});
                        
                        // Animação da capa
                        float escala = 0.8f + (progresso * 0.4f); // Começa menor e cresce
                        float alpha = 1.0f;
                        if (progresso > 0.7f) alpha = 1.0f - ((progresso - 0.7f) * 3.33f); // Fade out suave no final

                        float larguraCapa = (float)capaAlbum.width * escala;
                        float alturaCapa = (float)capaAlbum.height * escala;
                        
                        Rectangle dest = { 640, 350, larguraCapa, alturaCapa };
                        Vector2 origin = { larguraCapa / 2, alturaCapa / 2 };
                        
                        if (capaAlbum.id != 0) {
                            DrawTexturePro(capaAlbum, (Rectangle){0, 0, (float)capaAlbum.width, (float)capaAlbum.height}, 
                                           dest, origin, 0.0f, Fade(WHITE, alpha));
                        }
                        
                        DrawText("Abrindo Álbum Oficial...", 540, 620, 18, Fade(DARKGRAY, alpha));
                        
                        // Atalho para pular animação
                        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                            telaAtual = TELA_ALBUM;
                            timerAbertura = 0.0f;
                        }
                    } break;

	                case TELA_ALBUM:
	                    DrawRectangle(80, 50, 1120, 600, (Color){245, 245, 220, 255});
	                    desenharGrelhaAlbum(album, totalFigurinhas, paginaAtual);
	                    DrawText(TextFormat("Pagina %d de %d",
	                             paginaAtual + 1, totalPaginas + 1),
	                             560, 670, 20, WHITE);
	                    DrawText("[<-][->] Navegar | [BACKSPACE] Voltar", 420, 22, 18, LIGHTGRAY);
	                    break;

                case TELA_PACOTE:
                    gerenciarAberturaPacote(album, totalFigurinhas, &player, &telaAtual);
                    break;

                case TELA_MINIGAME:
                    gerenciarMiniGame(&player, &telaAtual);
                    break;

                case TELA_ESTATS:
                    desenharEstatisticas(album, totalFigurinhas);
                    break;

                case TELA_CONTROLE:
                    desenharControleFigurinhas(album, totalFigurinhas, &scrollControle, textoBusca);
                    break;

                case TELA_LOJA:
                    gerenciarLoja(&player, &telaAtual);
                    break;

                case TELA_TROCA:
                    gerenciarTrocas(album, totalFigurinhas, &player, &telaAtual);
                    break;

                default:
                    break;
            }
        EndDrawing();
    }

		    UnloadTexture(fundoMenu);
		    UnloadTexture(capaAlbum);
		    
		    // Descarrega as texturas das figurinhas
		    for (int i = 0; i < totalFigurinhas; i++) {
		        if (album[i].textura.id != 0) {
		            UnloadTexture(album[i].textura);
		        }
		    }
		    
		    UnloadMusicStream(musicaFundo);
		    UnloadSound(somClique);
		    UnloadSound(somVirarPagina);
		    CloseAudioDevice(); // Fecha o dispositivo de áudio
		    
		    CloseWindow();

    // Salvamento final
    salvarDadosBinario(album, totalFigurinhas);
    free(album);
    return 0;
}
