#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/figurinha.h"

/*
    figurinha.c

    Esse arquivo implementa todas as operações principais
    relacionadas às figurinhas do sistema.

    Aqui está o CRUD completo:
    - inserir (CREATE)
    - listar (READ)
    - pesquisar (READ filtrado)
    - alterar (UPDATE)
    - remover (DELETE)

    Ele é uma das bases do projeto, pois manipula diretamente
    os dados do álbum.
*/

/*
    Insere uma nova figurinha no vetor dinâmico.

    O vetor é realocado dinamicamente com realloc para
    comportar o novo elemento.

    Essa função representa a criação de novas figurinhas
    dentro do sistema.
*/
static int existeTitulo(Figurinha *figs, int total, const char *titulo, int ignorarIndice) {
    for (int i = 0; i < total; i++) {
        if (i == ignorarIndice) continue;
        if (strcmp(figs[i].titulo, titulo) == 0) {
            return 1; // Título já existe
        }
    }
    return 0; // Título não existe
}

void inserirFigurinha(Figurinha **figs, int *total) {

    (*total)++;

    // aumenta o vetor para caber a nova figurinha
    *figs = realloc(*figs, (*total) * sizeof(Figurinha));

    Figurinha *f = &(*figs)[(*total) - 1];

    printf("\n--- CADASTRAR NOVA FIGURINHA ---\n");

    printf("Codigo (digite 'sair' para cancelar): "); scanf("%s", f->codigo);
    if (strcmp(f->codigo, "sair") == 0) {
        (*total)--;
        *figs = realloc(*figs, (*total) * sizeof(Figurinha));
        printf("Cadastro cancelado.\n");
        return;
    }

    char tempTitulo[60];
    while (1) {
        printf("Titulo (digite 'sair' para cancelar): ");
        scanf(" %[^\n]", tempTitulo);
        if (strcmp(tempTitulo, "sair") == 0) {
            (*total)--; // Desfaz o incremento do total
            *figs = realloc(*figs, (*total) * sizeof(Figurinha)); // Reduz o vetor
            printf("Cadastro cancelado.\n");
            return;
        }
        if (existeTitulo(*figs, (*total) - 1, tempTitulo, -1)) {
            printf("Erro: Ja existe uma figurinha com este titulo. Por favor, digite outro.\n");
        } else {
            strcpy(f->titulo, tempTitulo);
            break;
        }
    }

    printf("Secao/Selecao: "); scanf(" %[^\n]", f->selecao);

    printf("Grupo (ex: Grupo A): "); scanf(" %[^\n]", f->grupo);

    printf("Tipo (Comum/Especial): "); scanf(" %[^\n]", f->tipo);

    // valores padrão ao criar figurinha
    f->colada = 0;
    f->repetidas = 0;

    // define raridade com base no tipo
    f->raridade = (strcmp(f->tipo, "Especial") == 0) ? 1 : 0;

    printf("Figurinha adicionada com sucesso!\n");
}

/*
    Lista todas as figurinhas cadastradas no sistema.

    Essa função serve para o usuário visualizar todo o álbum
    em modo texto (console).
*/
void listarFigurinhas(Figurinha *figs, int total) {

    printf("\n--- LISTA DE FIGURINHAS (%d) ---\n", total);

    for (int i = 0; i < total; i++) {

        printf("[%4d] %-8s %-40s %-25s | %s | Repetidas: %d\n",
               i + 1,
               figs[i].codigo,
               figs[i].titulo,
               figs[i].selecao,
               figs[i].colada ? "Colada " : "Faltando",
               figs[i].repetidas);
    }
}

/*
    Pesquisa figurinhas por código ou parte do título.

    Usa strstr para permitir busca parcial (não precisa ser exata).
*/
void pesquisarFigurinha(Figurinha *figs, int total) {

    char termo[60];

    printf("\n--- PESQUISAR FIGURINHA ---\n");
    printf("Digite o codigo ou parte do titulo: ");
    scanf(" %[^\n]", termo);

    int encontrados = 0;

    for (int i = 0; i < total; i++) {

        if (strstr(figs[i].codigo, termo) ||
            strstr(figs[i].titulo, termo)) {

            printf("[%d] %-8s %-40s (%s) | %s | Repetidas: %d\n",
                   i + 1,
                   figs[i].codigo,
                   figs[i].titulo,
                   figs[i].selecao,
                   figs[i].colada ? "Colada" : "Faltando",
                   figs[i].repetidas);

            encontrados++;
        }
    }

    if (encontrados == 0)
        printf("Nenhuma figurinha encontrada com '%s'.\n", termo);
    else
        printf("Total encontradas: %d\n", encontrados);
}

/*
    Altera os dados de uma figurinha existente.

    A busca é feita pelo código da figurinha.
*/
void alterarFigurinha(Figurinha *figs, int total) {

    char cod[15];

    printf("\n--- ALTERAR FIGURINHA ---\n");
    printf("Digite o codigo da figurinha (digite 'sair' para cancelar): ");
    scanf("%s", cod);
    if (strcmp(cod, "sair") == 0) {
        printf("Alteracao cancelada.\n");
        return;
    }

    for (int i = 0; i < total; i++) {

        if (strcmp(figs[i].codigo, cod) == 0) {

            printf("Figurinha encontrada: [%s] %s\n",
                   figs[i].codigo, figs[i].titulo);

            char tempTitulo[60];
            while (1) {
                printf("Novo titulo (digite 'sair' para cancelar): ");
                scanf(" %[^\n]", tempTitulo);
                if (strcmp(tempTitulo, "sair") == 0) {
                    printf("Alteracao cancelada.\n");
                    return;
                }
                if (existeTitulo(figs, total, tempTitulo, i)) {
                    printf("Erro: Ja existe outra figurinha com este titulo. Por favor, digite outro.\n");
                } else {
                    strcpy(figs[i].titulo, tempTitulo);
                    break;
                }
            }

            printf("Nova secao: ");
            scanf(" %[^\n]", figs[i].selecao);

            printf("Novo tipo: ");
            scanf(" %[^\n]", figs[i].tipo);

            // atualiza raridade caso tipo mude
            figs[i].raridade = (strcmp(figs[i].tipo, "Especial") == 0) ? 1 : 0;

            printf("Figurinha alterada com sucesso!\n");
            return;
        }
    }

    printf("Codigo '%s' nao encontrado.\n", cod);
}

/*
    Remove uma figurinha do sistema.

    Após encontrar o código, desloca os elementos do vetor
    para “fechar o espaço” e depois reduz o tamanho.
*/
void removerFigurinha(Figurinha **figs, int *total) {

    char cod[15];

    printf("\n--- REMOVER FIGURINHA ---\n");
    printf("Digite o codigo da figurinha para remover (digite 'sair' para cancelar): ");
    scanf("%s", cod);
    if (strcmp(cod, "sair") == 0) {
        printf("Remocao cancelada.\n");
        return;
    }

    for (int i = 0; i < *total; i++) {

        if (strcmp((*figs)[i].codigo, cod) == 0) {

            // desloca elementos para remover o índice
            for (int j = i; j < (*total) - 1; j++) {
                (*figs)[j] = (*figs)[j + 1];
            }

            (*total)--;

            // reduz memória do vetor
            *figs = realloc(*figs, (*total) * sizeof(Figurinha));

            printf("Figurinha '%s' removida com sucesso!\n", cod);
            return;
        }
    }

    printf("Codigo '%s' nao encontrado.\n", cod);
}