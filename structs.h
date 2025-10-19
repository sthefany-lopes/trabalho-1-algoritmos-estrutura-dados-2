#ifndef STRUCTS_H
#define STRUCTS_H

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

typedef struct {
    long long id_final;
    int posicao_inicial;
} INDICE;

#endif
