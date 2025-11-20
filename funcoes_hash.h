#ifndef FUNCOES_HASH_H
#define FUNCOES_HASH_H

unsigned int hashLongLong(long long x);

unsigned int hashStr(const char *str);

void inicializarTabelaHashJoia();

void inicializarTabelaHashPedido();

void inserirIndiceJoiaTabelaHash(long long id_categoria, int posicao);

void inserirIndicePedidoTabelaHash(const char *date_time, int posicao);

void montarTabelaHashIndicesJoias();

void montarTabelaHashIndicesPedidos();

void exibirTodosIndicesJoiasTabelaHash();

void exibirTodosIndicesPedidosTabelaHash();

void exibirIndicesJoiasViaChaveHash(long long id_categoria);

void exibirIndicesPedidosViaChaveHash(const char *date_time);

void exibirJoiasViaChaveHash(long long id_categoria);

void exibirPedidosViaChaveHash(const char *date_time);

void destruirTabelaHashIndicesJoias();

void destruirTabelaHashIndicesPedidos();

#endif
