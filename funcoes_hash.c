#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "structs.h"
#include "funcoes_hash.h"
#include "funcoes_utilitarias.h"
#include "config.h"

// Tabelas hash.
TABELA_HASH_INDICE_JOIA tabelaJoia;
TABELA_HASH_INDICE_PEDIDO tabelaPedido;

// Funções de cálculo hash.
unsigned int hashLongLong(long long x) {
    return (unsigned long long)x % TAM_HASH;
}

unsigned int hashStr(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % TAM_HASH;
}

// Funções para inicialização das tabelas hash.
void inicializarTabelaHashJoia() {
    int i;
    for (i = 0; i < TAM_HASH; i++) {
        tabelaJoia.bucket[i] = NULL;
    }
}

void inicializarTabelaHashPedido() {
    int i;
    for (i = 0; i < TAM_HASH; i++) {
        tabelaPedido.bucket[i] = NULL;
    }
}

// Funções para inserção de índice nas tabelas hash.
void inserirIndiceJoiaTabelaHash(long long id_categoria, int posicao) {
    unsigned int h = hashLongLong(id_categoria);

    HASH_INDICE_JOIA *novoIndiceJoia = malloc(sizeof(HASH_INDICE_JOIA));

    novoIndiceJoia->id_categoria = id_categoria;
    novoIndiceJoia->posicao = posicao;

    // Inserção sempre no início.
    novoIndiceJoia->prox = tabelaJoia.bucket[h];
    tabelaJoia.bucket[h] = novoIndiceJoia;
}

void inserirIndicePedidoTabelaHash(const char *date_time, int posicao) {
    unsigned int h = hashStr(date_time);

    HASH_INDICE_PEDIDO *novoIndicePedido = malloc(sizeof(HASH_INDICE_PEDIDO));
    strcpy(novoIndicePedido->date_time, date_time);
    novoIndicePedido->posicao = posicao;

    // Inserção sempre no início.
    novoIndicePedido->prox = tabelaPedido.bucket[h];
    tabelaPedido.bucket[h] = novoIndicePedido;
}

// Funções para a montagem inicial das tabelas hash.
void montarTabelaHashIndicesJoias() {
    clock_t inicio = clock();

    inicializarTabelaHashJoia();

    FILE *arquivoJoias = fopen(ARQUIVO_JOIAS, "rb");
    if (arquivoJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de joias.\n");
        exit(1);
    }

    JOIA joia;
    int posicao = 0;

    while (fread(&joia, sizeof(JOIA), 1, arquivoJoias) == 1) {
        inserirIndiceJoiaTabelaHash(joia.id_categoria, posicao);
        posicao++;
    }

    fclose(arquivoJoias);

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo para montar tabela hash de indices de joias: %.10f segundo(s).\n", tempoGasto);
}

void montarTabelaHashIndicesPedidos() {
    clock_t inicio = clock();

    inicializarTabelaHashPedido();

    FILE *arquivoPedidos = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arquivoPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de pedidos.\n");
        exit(1);
    }

    PEDIDO pedido;
    int posicao = 0;

    while (fread(&pedido, sizeof(PEDIDO), 1, arquivoPedidos) == 1) {
        inserirIndicePedidoTabelaHash(pedido.date_time, posicao);
        posicao++;
    }

    fclose(arquivoPedidos);

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo para montar tabela hash de indices de pedidos: %.10f segundo(s).\n", tempoGasto);
}

// Funções que percorrem a tabela hash e imprimem todos os índices.
void exibirTodosIndicesJoiasTabelaHash() {
    int i;
    int contador = 0;

    printf("\nTODOS OS INDICES DE JOIAS (TABELA HASH):\n");
    for (i = 0; i < TAM_HASH; i++) {
        printf("\nBUCKET %d:\n", i);

        HASH_INDICE_JOIA *indiceJoia = tabelaJoia.bucket[i];

        if (!indiceJoia) {
            printf("\nNenhum indice armazenado.\n");
        } else {
            while (indiceJoia) {
                printf("\nID da Categoria: %lld\n", indiceJoia->id_categoria);
                printf("Posicao: %d\n", indiceJoia->posicao);
                indiceJoia = indiceJoia->prox;
            }
        }

        contador++;
        if (contador % TAM_PAGINA_BUCKETS == 0) {
            if (!aguardarAcaoPaginacao()) {
                return;
            }
        }
    }
}

void exibirTodosIndicesPedidosTabelaHash() {
    int i;
    int contador = 0;

    printf("\nTODOS OS INDICES DE PEDIDOS (TABELA HASH):\n");
    for (i = 0; i < TAM_HASH; i++) {

        printf("\nBUCKET %d:\n", i);

        HASH_INDICE_PEDIDO *indicePedido = tabelaPedido.bucket[i];

        if (!indicePedido) {
            printf("\nNenhum indice armazenado.\n");
        } else {
            while (indicePedido) {
                printf("\nData e Hora: %s\n", indicePedido->date_time);
                printf("Posicao: %d\n", indicePedido->posicao);
                indicePedido = indicePedido->prox;
            }
        }

        contador++;
        if (contador % TAM_PAGINA_BUCKETS == 0) {
            if (!aguardarAcaoPaginacao()) {
                return;
            }
        }
    }
}

// Funções exibem os índices via chave hash recebida.
void exibirIndicesJoiasViaChaveHash(long long id_categoria) {
    clock_t inicio = clock();

    unsigned int h = hashLongLong(id_categoria);

    HASH_INDICE_JOIA *indiceJoia = tabelaJoia.bucket[h];
    int encontrados = 0;
    int comparacoes = 0;

    printf("\nINDICES DE JOIAS ENCONTRADOS PARA O ID DE CATEGORIA %lld:\n", id_categoria);
    while (indiceJoia) {
        comparacoes++;
        if (indiceJoia->id_categoria == id_categoria) {
            printf("\nPosicao: %d\n", indiceJoia->posicao);
            encontrados++;
        }
        indiceJoia = indiceJoia->prox;
    }

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\nTotal encontrados: %d - Comparacoes: %d\n", encontrados, comparacoes);
    printf("Tempo gasto: %.10f segundo(s).\n", tempoGasto);
}

void exibirIndicesPedidosViaChaveHash(const char *date_time) {
    clock_t inicio = clock();

    unsigned int h = hashStr(date_time);

    HASH_INDICE_PEDIDO *indicePedido = tabelaPedido.bucket[h];
    int encontrados = 0;
    int comparacoes = 0;

    printf("\nINDICES DE PEDIDOS ENCONTRADOS PARA A DATA E HORA %s:\n", date_time);
    while (indicePedido) {
        comparacoes++;
        if (strcasecmp(indicePedido->date_time, date_time) == 0) {
            printf("\nPosicao: %d\n", indicePedido->posicao);
            encontrados++;
        }
        indicePedido = indicePedido->prox;
    }

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\nTotal encontrados: %d - Comparacoes: %d\n", encontrados, comparacoes);
    printf("Tempo gasto: %.10f segundo(s).\n", tempoGasto);
}

// Funções que encontram o índice via chave hash recebida e exibem os dados dos itens.
void exibirJoiasViaChaveHash(long long id_categoria) {
    clock_t inicio = clock();

    unsigned int h = hashLongLong(id_categoria);

    HASH_INDICE_JOIA *indiceJoia = tabelaJoia.bucket[h];

    FILE *arquivoJoias = fopen(ARQUIVO_JOIAS, "rb");
    if (arquivoJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de joias.\n");
        exit(1);
    }

    int encontrados = 0;
    int comparacoes = 0;

    printf("\nJOIAS COM O ID DE CATEGORIA %lld:\n", id_categoria);
    while (indiceJoia) {
        comparacoes++;
        if (indiceJoia->id_categoria == id_categoria) {
            JOIA joia;
            fseek(arquivoJoias, indiceJoia->posicao * sizeof(JOIA), SEEK_SET);
            fread(&joia, sizeof(JOIA), 1, arquivoJoias);

            printf("\nPosicao: %d\n", indiceJoia->posicao);
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

            encontrados++;
        }
        indiceJoia = indiceJoia->prox;
    }

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\nTotal encontrados: %d - Comparacoes: %d\n", encontrados, comparacoes);
    printf("Tempo gasto: %.10f segundo(s).\n", tempoGasto);

    fclose(arquivoJoias);
}

void exibirPedidosViaChaveHash(const char *date_time) {
    clock_t inicio = clock();

    unsigned int h = hashStr(date_time);

    HASH_INDICE_PEDIDO *indicePedido = tabelaPedido.bucket[h];

    FILE *arquivoPedidos = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arquivoPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de pedidos.\n");
        exit(1);
    }

    int encontrados = 0;
    int comparacoes = 0;

    printf("\nPEDIDOS PARA DATA E HORA %s:\n", date_time);
    while (indicePedido) {
        comparacoes++;
        if (strcasecmp(indicePedido->date_time, date_time) == 0) {
            PEDIDO pedido;
            fseek(arquivoPedidos, indicePedido->posicao * sizeof(PEDIDO), SEEK_SET);
            fread(&pedido, sizeof(PEDIDO), 1, arquivoPedidos);

            printf("\nPosicao: %d\n", indicePedido->posicao);
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

            encontrados++;
        }
        indicePedido = indicePedido->prox;
    }

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\nTotal encontrados: %d - Comparacoes: %d\n", encontrados, comparacoes);
    printf("Tempo gasto: %.10f segundo(s).\n", tempoGasto);

    fclose(arquivoPedidos);
}

// Funções para destruição de tabelas (liberação de memória alocada) para a reorganização geral dos arquivos.
void destruirTabelaHashIndicesJoias() {
    clock_t inicio = clock();

    int i;
    for (i = 0; i < TAM_HASH; i++) {
        HASH_INDICE_JOIA *joia = tabelaJoia.bucket[i];
        while (joia) {
            HASH_INDICE_JOIA *temp = joia;
            joia = joia->prox;
            free(temp);
        }
        tabelaJoia.bucket[i] = NULL;
    }

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo para destruir a tabela hash de indices de joias: %.10f segundo(s).\n", tempoGasto);
}

void destruirTabelaHashIndicesPedidos() {
    clock_t inicio = clock();

    int i;
    for (i = 0; i < TAM_HASH; i++) {
        HASH_INDICE_PEDIDO *pedido = tabelaPedido.bucket[i];
        while (pedido) {
            HASH_INDICE_PEDIDO *temp = pedido;
            pedido = pedido->prox;
            free(temp);
        }
        tabelaPedido.bucket[i] = NULL;
    }

    clock_t fim = clock();
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo para destruir a tabela hash de indices de pedidos: %.10f segundo(s).\n", tempoGasto);
}
