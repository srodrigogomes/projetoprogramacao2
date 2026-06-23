#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "figurinha.h"

/*
    arquivo.h

    Esse arquivo reúne as funções responsáveis pelo gerenciamento
    de arquivos do projeto.

    Aqui ficam declaradas as funções usadas para:
    - carregar os dados do álbum;
    - salvar progresso;
    - exportar informações;
    - ler o arquivo CSV inicial.

    Essas funções ajudam a manter os dados persistentes,
    ou seja, o usuário pode fechar o programa e continuar
    depois sem perder o progresso.
*/

/*
    Tenta carregar os dados salvos do álbum através do
    arquivo binário.

    Caso o binário ainda não exista (primeira execução),
    o sistema carrega as figurinhas a partir do CSV inicial.

    A função recebe:
    - ponteiro para o vetor de figurinhas;
    - ponteiro para a variável de total.

    Isso permite modificar os dados diretamente na main.
*/
int carregarDadosGerais(Figurinha **figs, int *total);

/*
    Salva todas as informações atuais do álbum em um
    arquivo binário.

    Essa função é chamada normalmente quando o usuário
    fecha o programa ou realiza alterações importantes,
    garantindo que o progresso fique armazenado.
*/
void salvarDadosBinario(Figurinha *figs, int total);

/*
    Exporta os dados atuais do álbum para um arquivo CSV.

    Isso pode ser usado para visualizar os dados fora
    do programa ou gerar relatórios simples da coleção.
*/
void exportarCSV(Figurinha *figs, int total);

/*
    Função responsável por fazer a leitura inicial
    das figurinhas através do arquivo CSV.

    Ela é usada principalmente na primeira vez em que
    o programa é executado, antes de existir um arquivo
    binário salvo.

    Depois que o binário é criado, o sistema passa
    a carregar os dados diretamente dele.
*/
int carregarFigurinhasCSV(Figurinha **figs, int *total);

/*
    Salva os dados do jogador (moedas, pacotes, gols) em um arquivo binário.
*/
void salvarDadosJogador(Jogador *player);

/*
    Carrega os dados do jogador de um arquivo binário.
    Se o arquivo não existir, inicializa com valores padrão.
*/
void carregarDadosJogador(Jogador *player);

#endif