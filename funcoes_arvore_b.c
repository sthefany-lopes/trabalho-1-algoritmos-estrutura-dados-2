#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "funcoes_arvore_b.h"
#include "funcoes_utilitarias.h"
#include "structs.h"
#include "config.h"

NO_ARVORE_B* arvoreBJoias = NULL;
NO_ARVORE_B* arvoreBPedidos = NULL;

// Função que cria um nó da árvore B.
NO_ARVORE_B* criaNoArvoreB(int ehFolha) {
    NO_ARVORE_B* no = (NO_ARVORE_B*) malloc(sizeof(NO_ARVORE_B));
    no->ehFolha = ehFolha;
    no->n = 0;

    for (int i = 0; i < MAX_FILHOS; i++) {
        no->filhos[i] = NULL;
    }

    return no;
}

// Função que busca por ID na árvore B.
int buscarPosicaoPorIDArvoreB(NO_ARVORE_B* raiz, long long idBuscado) {
    if (!raiz) {
        return -1;
    }

    int i = 0;
    while (i < raiz->n && idBuscado > raiz->ids[i]) {
        i++;
    }

    if (i < raiz->n && idBuscado == raiz->ids[i]) {
        return raiz->posicoes[i];
    }

    if (raiz->ehFolha) {
        return -1;
    }

    return buscarPosicaoPorIDArvoreB(raiz->filhos[i], idBuscado);
}

// Função de split de nó cheio.
void dividirFilhoArvoreB(NO_ARVORE_B* pai, int i, NO_ARVORE_B* cheio) {
    int meio = MAX_CHAVES / 2;
    NO_ARVORE_B* novo = criaNoArvoreB(cheio->ehFolha);

    novo->n = MAX_CHAVES - meio - 1;

    int j;
    for (j = 0; j < novo->n; j++) {
        novo->ids[j] = cheio->ids[meio + 1 + j];
        novo->posicoes[j] = cheio->posicoes[meio + 1 + j];
    }

    if (!cheio->ehFolha) {
        for (j = 0; j < novo->n + 1; j++) {
            novo->filhos[j] = cheio->filhos[meio + 1 + j];
        }
    }

    cheio->n = meio;

    for (j = pai->n; j >= i + 1; j--) {
        pai->filhos[j + 1] = pai->filhos[j];
    }

    pai->filhos[i + 1] = novo;

    for (j = pai->n - 1; j >= i; j--) {
        pai->ids[j + 1] = pai->ids[j];
        pai->posicoes[j + 1] = pai->posicoes[j];
    }

    pai->ids[i] = cheio->ids[meio];
    pai->posicoes[i] = cheio->posicoes[meio];
    pai->n++;
}

// Função de inserção em nó não-cheio.
void inserirNaoCheioArvoreB(NO_ARVORE_B* no, long long id, int posicao) {
    int i = no->n - 1;

    if (no->ehFolha) {
        // Move chaves para abrir espaço.
        while (i >= 0 && id < no->ids[i]) {
            no->ids[i + 1] = no->ids[i];
            no->posicoes[i + 1] = no->posicoes[i];
            i--;
        }

        no->ids[i + 1] = id;
        no->posicoes[i + 1] = posicao;
        no->n++;
    } else {
        while (i >= 0 && id < no->ids[i]) {
            i--;
        }

        i++;
        if (no->filhos[i]->n == MAX_CHAVES) {
            dividirFilhoArvoreB(no, i, no->filhos[i]);

            if (id > no->ids[i]) {
                i++;
            }
        }

        inserirNaoCheioArvoreB(no->filhos[i], id, posicao);
    }
}

// Função que inserção em uma árvore B.
void inserirArvoreB(NO_ARVORE_B** raiz, long long id, int posicao) {
    if (*raiz == NULL) {
        *raiz = criaNoArvoreB(1);
        (*raiz)->ids[0] = id;
        (*raiz)->posicoes[0] = posicao;
        (*raiz)->n = 1;
        return;
    }

    if ((*raiz)->n == MAX_CHAVES) {
        NO_ARVORE_B* novaRaiz = criaNoArvoreB(0);
        novaRaiz->filhos[0] = *raiz;

        dividirFilhoArvoreB(novaRaiz, 0, *raiz);

        int i = 0;
        if (id > novaRaiz->ids[0]) {
            i++;
        }

        inserirNaoCheioArvoreB(novaRaiz->filhos[i], id, posicao);

        *raiz = novaRaiz;
    } else {
        inserirNaoCheioArvoreB(*raiz, id, posicao);
    }
}

// Função que exibe os índices de uma árvore B.
void exibirArvoreB(NO_ARVORE_B* raiz) {
    int i = 0;

    if (!raiz) {
        return;
    }

    printf("\nNODO:\n");
    for (i = 0; i < raiz->n; i++) {
        printf("\nID: %lld\n", raiz->ids[i]);
        printf("Posicao: %d\n", raiz->posicoes[i]);
    }

    if (!raiz->ehFolha) {
        for (int i = 0; i <= raiz->n; i++) {
            exibirArvoreB(raiz->filhos[i]);
        }
    }
}

// Função que destrói uma árvore B.
void destruirArvoreB(NO_ARVORE_B* raiz) {
    if (!raiz) {
        return;
    }
    if (!raiz->ehFolha) {
        for (int i = 0; i <= raiz->n; i++) {
            destruirArvoreB(raiz->filhos[i]);
        }
    }
    free(raiz);
}

// Funções para montar a árvore B de índices de joias e pedidos.
void montarArvoreBIndicesJoias() {
    clock_t inicio = clock();

    FILE *arquivoJoias = fopen(ARQUIVO_JOIAS, "rb");
    if (arquivoJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de joias.\n");
        exit(1);
    }

    JOIA joia;
    int posicao = 0;

    arvoreBJoias = NULL;
    while (fread(&joia, sizeof(JOIA), 1, arquivoJoias) == 1) {
        inserirArvoreB(&arvoreBJoias, joia.id, posicao);
        posicao++;
    }

    fclose(arquivoJoias);

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo para montar a arvore B de indices de joias: %.10f segundo(s).\n", tempoGasto);
}

void montarArvoreBIndicesPedidos() {
    clock_t inicio = clock();

    FILE *arquivoPedidos = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arquivoPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de pedidos.\n");
        exit(1);
    }

    PEDIDO pedido;
    int posicao = 0;

    arvoreBPedidos = NULL;
    while (fread(&pedido, sizeof(PEDIDO), 1, arquivoPedidos) == 1) {
        inserirArvoreB(&arvoreBPedidos, pedido.id, posicao);
        posicao++;
    }

    fclose(arquivoPedidos);

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo para montar a arvore B de indices de pedidos: %.10f segundo(s).\n", tempoGasto);
}

// Funções que percorrem as árvores B e imprimem todos os índices.
void exibirTodosIndicesJoiasArvoreB() {
    printf("\nTODOS OS INDICES DE JOIAS (ARVORE B):\n");
    if (arvoreBJoias == NULL) {
        printf("\nNenhum indice armazenado.\n");
    } else {
        exibirArvoreB(arvoreBJoias);
    }
}

void exibirTodosIndicesPedidosArvoreB() {
    printf("\nTODOS OS INDICES DE PEDIDOS (ARVORE B):\n");
    if (arvoreBPedidos == NULL) {
        printf("\nNenhum indice armazenado.\n");
    } else {
        exibirArvoreB(arvoreBPedidos);
    }
}

// Funções exibem os índices via ID recebido.
void exibirIndiceJoiaArvoreB(long long id) {
    clock_t inicio = clock();

    int posicao = buscarPosicaoPorIDArvoreB(arvoreBJoias, id);

    printf("\nINDICE DE JOIA ENCONTRADO PARA O ID %lld:\n", id);
    if (posicao == -1) {
        printf("\nIndice nao encontrado.\n");
    } else {
        printf("\nPosicao encontrada: %d\n", posicao);
    }

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.10f segundo(s).\n", tempoGasto);
}

void exibirIndicePedidoArvoreB(long long id) {
    clock_t inicio = clock();

    int posicao = buscarPosicaoPorIDArvoreB(arvoreBPedidos, id);

    printf("\nINDICE DE PEDIDO ENCONTRADO PARA O ID %lld:\n", id);
    if (posicao == -1) {
        printf("\nIndice nao encontrado.\n");
    } else {
        printf("\nPosicao encontrada: %d\n", posicao);
    }

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.10f segundo(s).\n", tempoGasto);
}

// Funções que encontram o índice via ID recebido e exibem os dados do item.
void exibirJoiaArvoreB(long long id) {
    clock_t inicio = clock();

    FILE *arquivoJoias = fopen(ARQUIVO_JOIAS, "rb");
    if (arquivoJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de joias.\n");
        exit(1);
    }

    int posicao = buscarPosicaoPorIDArvoreB(arvoreBJoias, id);

    printf("\nJOIA COM O ID %lld:\n", id);
    if (posicao == -1) {
        printf("\nJoia nao encontrada.\n");
    } else {
        JOIA joia;
        fseek(arquivoJoias, posicao * sizeof(JOIA), SEEK_SET);
        fread(&joia, sizeof(JOIA), 1, arquivoJoias);

        printf("\nPosicao: %d\n", posicao);
        printf("ID: %lld\n", joia.id);

        if (joia.id_categoria != -1) {
            printf("ID da Categoria: %lld\n", joia.id_categoria);
        } else {
            printf("ID da Categoria: Sem ID\n");
        }

        printf("Alias da Categoria: %s\n", joia.alias_categoria);

        if (joia.id_marca != -1) {
            printf("ID da Marca: %lld\n", joia.id_marca);
        } else {
            printf("ID da Marca: Sem ID\n");
        }

        printf("Genero: %s\n", joia.genero);

        if (joia.elo >= 0) {
            printf("Elo: %d\n", joia.elo);
        } else {
            printf("Elo: Sem elo\n");
        }

        printf("Marcada para exclusao: %s\n", joia.fl_exclusao ? "Sim" : "Nao");
    }

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.10f segundo(s).\n", tempoGasto);

    fclose(arquivoJoias);
}

void exibirPedidoArvoreB(long long id) {
    clock_t inicio = clock();

    int posicao = buscarPosicaoPorIDArvoreB(arvoreBPedidos, id);

    FILE *arquivoPedidos = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arquivoPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de pedidos.\n");
        exit(1);
    }

    printf("\nPEDIDO COM O ID %lld:\n", id);

    if (posicao == -1) {
        printf("\nPedido nao encontrada.\n");
    } else {
        PEDIDO pedido;
        fseek(arquivoPedidos, posicao * sizeof(PEDIDO), SEEK_SET);
        fread(&pedido, sizeof(PEDIDO), 1, arquivoPedidos);

        printf("\nPosicao: %d\n", posicao);
        printf("ID do Pedido: %lld\n", pedido.id);
        printf("ID do Produto: %lld\n", pedido.id_produto);
        printf("Data e Hora: %s\n", pedido.date_time);
        printf("Quantidade SKU: %d\n", pedido.quantidade_sku);

        if (pedido.preco != -1.0f) {
            printf("Preco: $ %.2f\n", pedido.preco);
        } else {
            printf("Preco: Sem preco\n");
        }

        if (pedido.id_usuario != -1) {
            printf("ID do Usuario: %lld\n", pedido.id_usuario);
        } else {
            printf("ID do Usuario: Sem ID\n");
        }

        if (pedido.elo >= 0) {
            printf("Elo: %d\n", pedido.elo);
        } else {
            printf("Elo: Sem elo\n");
        }

        printf("Marcado para exclusao: %s\n", pedido.fl_exclusao ? "Sim" : "Nao");
    }

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.10f segundo(s).\n", tempoGasto);

    fclose(arquivoPedidos);
}

// Funções de inserção na Árvore B de joias e pedidos.
void inserirIndiceJoiaArvoreB(long long id, int posicao) {
    inserirArvoreB(&arvoreBJoias, id, posicao);
}

void inserirIndicePedidoArvoreB(long long id, int posicao) {
    inserirArvoreB(&arvoreBPedidos, id, posicao);
}

// Funções para destruição de árvores B (liberação de memória alocada) para a reorganização geral dos arquivos.
void destruirArvoreBIndicesJoias() {
    clock_t inicio = clock();

    if (arvoreBJoias != NULL) {
        destruirArvoreB(arvoreBJoias);
        arvoreBJoias = NULL;
    }

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo para destruir a arvore B de indices de joias: %.10f segundo(s).\n", tempoGasto);
}

void destruirArvoreBIndicesPedidos() {
    clock_t inicio = clock();

    if (arvoreBPedidos != NULL) {
        destruirArvoreB(arvoreBPedidos);
        arvoreBPedidos = NULL;
    }

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo para destruir a arvore B de indices de pedidos: %.10f segundo(s).\n", tempoGasto);
}
