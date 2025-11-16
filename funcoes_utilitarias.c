#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "funcoes_utilitarias.h"

long long strParaLongLong(const char *str) {
    if (str == NULL || strlen(str) == 0) {
        return -1;
    }

    return atoll(str);
}

float strParaFloat(const char *str) {
    if (str == NULL || strlen(str) == 0) {
        return -1.0f;
    }

    return atof(str);
}

int strParaInt(const char *str) {
    if (str == NULL || strlen(str) == 0) {
        return -1;
    }

    return atoi(str);
}

int compararPedidos(const void *a, const void *b) {
    long long idA = ((PEDIDO*)a)->id;
    long long idB = ((PEDIDO*)b)->id;

    if (idA < idB) {
        return -1;
    } else if (idA > idB) {
        return 1;
    }

    return 0;
}

int compararJoias(const void *a, const void *b) {
    long long idA = ((JOIA*)a)->id;
    long long idB = ((JOIA*)b)->id;

    if (idA < idB) {
        return -1;
    } else if (idA > idB) {
        return 1;
    }

    return 0;
}

int aguardarAcaoPaginacao() {
    int opcao;

    printf("\nPressione '1' para continuar ou '0' para sair: ");
    scanf("%d", &opcao);

    return opcao;
}
