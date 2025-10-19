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

    int opcao;
    do {
        exibirMenu();
        scanf("%d", &opcao);
        processarOpcaoMenu(opcao);
        printf("\n");
    } while (opcao != 0);

    return 0;
}
