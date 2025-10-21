#include <stdio.h>
#include <stdlib.h>

#include "funcoes.h"

int main() {
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

    gerenciarMenu();

    return 0;
}
