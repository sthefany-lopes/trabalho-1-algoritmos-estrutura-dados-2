#include <stdio.h>
#include <stdlib.h>

#include "funcoes.h"
#include "funcoes_hash.h"

int main() {
    // Montagem dos arquivos de dados e de organização indexado-sequencial.
    int arquivosDadosExistem = verificarExistenciaArquivosDados();
    if (!arquivosDadosExistem) {
        criarArquivosDados();
        criarArquivosIndices();
    } else {
        int arquivosIndicesExistem = verificarExistenciaArquivosIndices();
        if (!arquivosIndicesExistem) {
            criarArquivosIndices();
        }
    }

    // Montagem das tabelas hash em memória.
    montarTabelaHashIndicesJoias();
    montarTabelaHashIndicesPedidos();
    printf("\n");

    gerenciarMenu();

    return 0;
}
