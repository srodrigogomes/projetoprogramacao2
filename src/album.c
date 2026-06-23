#include "raylib.h"
#include "../include/figurinha.h"
#include "../include/album.h"
#include "../include/utils.h"
#include <stdio.h>

/*
    desenharGrelhaAlbum

    Essa função é responsável por desenhar a interface visual do álbum
    de figurinhas na tela utilizando Raylib.

    Ela organiza as figurinhas em forma de grade (grid), simulando
    um álbum de verdade, onde cada figurinha ocupa um “card”.
*/


void desenharGrelhaAlbum(Figurinha *figs, int total, int pagina) {

    static float alphaAnim[1000] = {0};

    // Define quantas figurinhas aparecem por página
    int figsPorPagina = 8; 

    // Calcula o intervalo de figurinhas que serão exibidas
    int inicio = pagina * figsPorPagina;
    int fim = inicio + figsPorPagina;
    
    // Evita ultrapassar o total de figurinhas existentes
    if (fim > total) fim = total;

    /*
        Configuração visual da grade (layout da tela)

        Aqui são definidos os espaçamentos e tamanhos dos cards,
        para organizar a exibição das figurinhas na tela.
    */
    int posX_inicial = 150;
    int posY_inicial = 120;
    int espacamentoX = 250;
    int espacamentoY = 260;
    int larguraCard = 180;
    int alturaCard = 220;

    /*
        Loop principal de desenho das figurinhas da página atual
    */
    for (int i = inicio; i < fim; i++) {

        alphaAnim[i] += 0.05f;
        if (alphaAnim[i] > 1.0f) alphaAnim[i] = 1.0f;

        // Define posição relativa dentro da página (grid 4 colunas)
        int indiceNaPagina = i - inicio;
        int coluna = indiceNaPagina % 4;
        int linha = indiceNaPagina / 4;

        int x = posX_inicial + (coluna * espacamentoX);
        int y = posY_inicial + (linha * espacamentoY);

        // Define o retângulo do card da figurinha
        Rectangle card = { (float)x, (float)y, (float)larguraCard, (float)alturaCard };

        if (alphaAnim[i] < 1.0f) {
        alphaAnim[i] += 0.05f;
        if (alphaAnim[i] > 1.0f) alphaAnim[i] = 1.0f;
}

        /*
            Define a cor do card com base na raridade da figurinha

            Isso melhora a experiência visual e ajuda o jogador
            a identificar rapidamente o nível da figurinha.
        */
        Color corCard = COR_COMUM;
        if (figs[i].raridade == 1) corCard = COR_RARA;
        else if (figs[i].raridade == 2) corCard = COR_LENDARIA;

        /*
            Desenha o fundo do card:
            - se já foi colada → usa cor da raridade
            - se não foi colada → branco (slot vazio)
        */
        DrawRectangleRec(card, Fade(figs[i].colada ? corCard : RAYWHITE, alphaAnim[i]));
        
        // Desenha a borda do card
        Color base = figs[i].colada ? corCard : RAYWHITE;
        base.a = (unsigned char)(255 * alphaAnim[i]);

         DrawRectangleRec(card, base);

        /*
            Se a figurinha já foi obtida (colada), mostramos os dados
        */
        if (figs[i].colada) {
            // Se a textura foi carregada, exibe a imagem
            if (figs[i].textura.id != 0) {
                // Desenha a imagem da figurinha escalada para caber no card
                float escalaX = (float)larguraCard / figs[i].textura.width;
                float escalaY = (float)(alturaCard - 50) / figs[i].textura.height;
                float escala = (escalaX < escalaY) ? escalaX : escalaY; // Usa a menor escala
                
                int imgLargura = (int)(figs[i].textura.width * escala);
                int imgAltura = (int)(figs[i].textura.height * escala);
                int imgX = x + (larguraCard - imgLargura) / 2;
                int imgY = y + 10 + ((alturaCard - 50 - imgAltura) / 2);
                
                DrawTextureEx(figs[i].textura, (Vector2){(float)imgX, (float)imgY}, 0.0f, escala, Fade(WHITE, alphaAnim[i]));
            } else {
                // Se não houver imagem, mostra os dados no centro como era antes
                DrawText(figs[i].titulo, x + 10, y + 80, 18, Fade(BLACK, alphaAnim[i]));
            }
            
            // Desenha o código e seleção na parte inferior do card
            DrawText(figs[i].codigo, x + 10, y + 10, 12, Fade(DARKGRAY, alphaAnim[i]));
            DrawText(figs[i].selecao, x + 10, y + 190, 12, Fade(DARKBLUE, alphaAnim[i]));

            /*
                Se houver repetidas, exibimos um contador no canto
                superior direito do card.
            */
            if (figs[i].repetidas > 0) {
                DrawCircle(x + larguraCard - 15, y + 15, 12, Fade(RED, alphaAnim[i]));

                const char* txtRepetidas = TextFormat("%d", figs[i].repetidas);
                DrawText(txtRepetidas, x + larguraCard - 22, y + 8, 15, Fade(WHITE, alphaAnim[i]));
            }

        } else {
            /*
                Caso a figurinha ainda não tenha sido obtida,
                mostramos apenas o código como dica do slot vazio.
            */
           DrawText(figs[i].codigo, x + 40, y + 95, 30, Fade(LIGHTGRAY, alphaAnim[i]));
        }
    }
}