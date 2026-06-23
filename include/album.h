#ifndef ALBUM_H
#define ALBUM_H

#include "figurinha.h"

/*
    album.h

    Esse arquivo é responsável por declarar as funções relacionadas
    à parte visual do álbum de figurinhas.

    Aqui ficam apenas as assinaturas das funções, enquanto a lógica
    completa fica implementada no arquivo album.c.

    O include "figurinha.h" é necessário porque a função utiliza
    a struct Figurinha como parâmetro.
*/

/*
    Função responsável por desenhar na tela a grade de figurinhas
    do álbum.

    Ela recebe:
    - o vetor com todas as figurinhas;
    - a quantidade total de figurinhas existentes;
    - a página atual do álbum.

    A main utiliza essa função para exibir visualmente o álbum
    enquanto o usuário navega entre as páginas.

    Essa separação ajuda a deixar o projeto organizado:
    - main.c controla o fluxo geral;
    - album.c cuida da interface do álbum.
*/
void desenharGrelhaAlbum(Figurinha *figs, int total, int pagina);

#endif