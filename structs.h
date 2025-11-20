#ifndef STRUCTS_H
#define STRUCTS_H

#include "config.h"

// Estruturas gerais das joias e pedidos.
typedef struct {
    long long id;
    long long id_categoria;
    char alias_categoria[80];
    long long id_marca;
    char genero[2];
    int elo;
    int fl_exclusao;
} JOIA;

typedef struct {
    long long id;
    long long id_produto;
    char date_time[30];
    int quantidade_sku;
    float preco;
    long long id_usuario;
    int elo;
    int fl_exclusao;
} PEDIDO;

// Estrutura do índice utilizado na organização indexado-sequencial.
typedef struct {
    long long id_final;
    int posicao_inicial;
} INDICE;

// Estrutura dos índices utilizados na organização por tabela hash.
typedef struct hash_indice_joia {
    long long id_categoria;
    int posicao;
    struct hash_indice_joia *prox;
} HASH_INDICE_JOIA;

typedef struct hash_indice_pedido {
    char date_time[30];
    int posicao;
    struct hash_indice_pedido *prox;
} HASH_INDICE_PEDIDO;

// Estrutura das tabelas hash para armazenamento dos índices.
typedef struct {
    HASH_INDICE_PEDIDO *bucket[TAM_HASH];
} TABELA_HASH_INDICE_PEDIDO;

typedef struct {
    HASH_INDICE_JOIA *bucket[TAM_HASH];
} TABELA_HASH_INDICE_JOIA;

#endif
