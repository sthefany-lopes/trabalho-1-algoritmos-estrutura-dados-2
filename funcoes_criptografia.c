#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcoes_criptografia.h"
#include "funcoes_utilitarias.h"
#include "config.h"

void aplicarXorArquivo(const char *nomeArquivoEntrada, const char *nomeArquivoSaida) {
    FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "rb");
    if (arquivoEntrada == NULL) {
        printf("\nErro ao abrir o arquivo de entrada.\n");
        return;
    }

    FILE *arquivoSaida = fopen(nomeArquivoSaida, "wb");
    if (arquivoSaida == NULL) {
        printf("\nErro ao abrir o arquivo de saida.\n");
        fclose(arquivoEntrada);
        return;
    }

    int c;
    while ((c = fgetc(arquivoEntrada)) != EOF) {
        fputc(c ^ XOR_KEY, arquivoSaida); // Aplica XOR no byte.
    }

    fclose(arquivoEntrada);
    fclose(arquivoSaida);
}

void gerarArquivosDadosCriptografados() {
    aplicarXorArquivo(ARQUIVO_JOIAS, ARQUIVO_JOIAS_CRIPT);
    aplicarXorArquivo(ARQUIVO_PEDIDOS, ARQUIVO_PEDIDOS_CRIPT);
}

void gerarArquivosDadosDescriptografados() {
    aplicarXorArquivo(ARQUIVO_JOIAS_CRIPT, ARQUIVO_JOIAS_DESCRIPT);
    aplicarXorArquivo(ARQUIVO_PEDIDOS_CRIPT, ARQUIVO_PEDIDOS_DESCRIPT);
}
