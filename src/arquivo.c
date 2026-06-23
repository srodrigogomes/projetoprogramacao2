#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/figurinha.h"
#include "../include/jogador.h"

/*
    arquivo.c

    Esse arquivo é responsável pelo sistema de persistência de dados
    do álbum de figurinhas.

    Aqui ficam todas as funções que lidam com:
    - leitura do CSV inicial (primeira execução);
    - salvamento em arquivo binário;
    - carregamento do progresso salvo;
    - exportação dos dados para CSV.

    Ou seja, ele garante que o progresso do usuário não se perca
    ao fechar o programa.
*/

#define CSV_PATH    "assets/palavras.csv"
#define BIN_PATH    "assets/save.bin"
#define EXPORT_PATH "assets/exportado.csv"
#define PLAYER_BIN_PATH "assets/player_save.bin"

/*
    Função auxiliar interna para limpar strings.

    Ela remove espaços e caracteres de controle (\n, \r, \t)
    que podem vir do arquivo CSV.

    Isso evita erros de comparação e deixa os dados padronizados.
*/
static void limparString(char *str) {
    int i, j;

    // Remove espaços iniciais
    for (i = 0; str[i] == ' ' || str[i] == '\t'; i++);

    if (i > 0) {
        for (j = 0; str[i + j] != '\0'; j++)
            str[j] = str[i + j];
        str[j] = '\0';
    }

    // Remove espaços e quebras no final da string
    for (i = (int)strlen(str) - 1;
         i >= 0 && (str[i] == ' ' || str[i] == '\r' || str[i] == '\n' || str[i] == '\t');
         i--);

    str[i + 1] = '\0';
}

/*
    Carrega as figurinhas a partir do arquivo CSV.

    Essa função é usada na primeira execução do sistema,
    quando ainda não existe arquivo de salvamento binário.

    Ela lê linha por linha, separa os dados e preenche
    dinamicamente o vetor de figurinhas.
*/
int carregarFigurinhasCSV(Figurinha **figs, int *total) {
    FILE *arquivo = fopen(CSV_PATH, "r");
    if (!arquivo) {
        printf("Erro: CSV '%s' nao encontrado.\n", CSV_PATH);
        return 0;
    }

    char linha[256];
    *total = 0;

    // pula cabeçalho do CSV
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo)) {

        // aumenta dinamicamente o vetor
        *figs = realloc(*figs, (*total + 1) * sizeof(Figurinha));
        Figurinha *f = &(*figs)[*total];

        /*
            Separa os campos do CSV usando vírgula
        */
        char *token = strtok(linha, ",");

        if (token) { strcpy(f->codigo, token);  limparString(f->codigo); }

        token = strtok(NULL, ",");
        if (token) { strcpy(f->titulo, token);  limparString(f->titulo); }

        token = strtok(NULL, ",");
        if (token) { strcpy(f->selecao, token); limparString(f->selecao); }

        token = strtok(NULL, ",");
        if (token) { strcpy(f->grupo, token);   limparString(f->grupo); }

        token = strtok(NULL, ",");
        if (token) {
            strcpy(f->tipo, token);
            limparString(f->tipo);

            /*
                Converte tipo em raridade:
                - Especial = 1 (mais rara)
                - comum = 0
            */
            f->raridade = (strcmp(f->tipo, "Especial") == 0) ? 1 : 0;
        }

        // valores padrão ao carregar
        f->colada = 0;
        f->repetidas = 0;
        f->textura.id = 0; // Inicializa sem textura

        (*total)++;
    }

    fclose(arquivo);
    return 1;
}

/*
    Salva o estado atual do álbum em arquivo binário.

    Isso permite que o jogador feche o programa e continue
    depois sem perder progresso.
*/
void salvarDadosBinario(Figurinha *figs, int total) {
    FILE *f = fopen(BIN_PATH, "wb");

    if (!f) {
        printf("Erro: nao foi possivel salvar em %s.\n", BIN_PATH);
        return;
    }

    fwrite(&total, sizeof(int), 1, f);
    fwrite(figs, sizeof(Figurinha), total, f);

    fclose(f);
}

void salvarDadosJogador(Jogador *player) {
    FILE *f = fopen(PLAYER_BIN_PATH, "wb");
    if (!f) {
        printf("Erro: Nao foi possivel salvar os dados do jogador em %s.\n", PLAYER_BIN_PATH);
        return;
    }
    fwrite(player, sizeof(Jogador), 1, f);
    fclose(f);
}

void carregarDadosJogador(Jogador *player) {
    FILE *f = fopen(PLAYER_BIN_PATH, "rb");
    if (f) {
        fread(player, sizeof(Jogador), 1, f);
        fclose(f);
    } else {
        // Valores padrao se o arquivo nao existir
        player->moedas = 100;
        player->pacotesDisponiveis = 5;
        player->totalGols = 0;
    }
}

/*
    Tenta carregar o progresso salvo no arquivo binário.

    Se não existir, o sistema automaticamente cai no CSV inicial.

    Isso garante que o programa funcione tanto na primeira execução
    quanto nas seguintes.
*/
int carregarDadosGerais(Figurinha **figs, int *total) {
    FILE *f = fopen(BIN_PATH, "rb");

    if (f) {
        if (fread(total, sizeof(int), 1, f) == 1 && *total > 0) {

            *figs = malloc((*total) * sizeof(Figurinha));

            if (fread(*figs, sizeof(Figurinha), *total, f) == (size_t)*total) {
                fclose(f);
                
                // Inicializa as texturas como vazias após carregar do binário
                // (O ID da textura não é persistente entre execuções)
                for (int i = 0; i < *total; i++) {
                    (*figs)[i].textura.id = 0;
                }
                
                return 1;
            }

            free(*figs);
            *figs = NULL;
            *total = 0;
        }

        fclose(f);
    }

    // fallback para CSV
    int res = carregarFigurinhasCSV(figs, total);
    
    // Inicializa texturas se carregou do CSV
    if (res) {
        for (int i = 0; i < *total; i++) {
            (*figs)[i].textura.id = 0;
        }
    }
    
    return res;
}

/*
    Exporta o estado atual do álbum para um CSV.

    Isso serve para:
    - depuração;
    - backup simples;
    - visualização externa dos dados.
*/
void exportarCSV(Figurinha *figs, int total) {
    FILE *f = fopen(EXPORT_PATH, "w");

    if (!f) {
        printf("Erro: nao foi possivel criar '%s'.\n", EXPORT_PATH);
        return;
    }

    fprintf(f, "codigo,titulo,secao,grupo,tipo,colada,repetidas\n");

    for (int i = 0; i < total; i++) {
        fprintf(f, "%s,%s,%s,%s,%s,%d,%d\n",
                figs[i].codigo,
                figs[i].titulo,
                figs[i].selecao,
                figs[i].grupo,
                figs[i].tipo,
                figs[i].colada,
                figs[i].repetidas);
    }

    fclose(f);

    printf("Dados exportados para '%s'.\n", EXPORT_PATH);
}