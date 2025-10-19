#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcoes.h"
#include "funcoes_utilitarias.h"
#include "structs.h"
#include "config.h"

void criarArquivosIndices() {
    FILE *arquivoJoias = fopen(ARQUIVO_JOIAS, "rb");
    if (arquivoJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de joias.\n");
        exit(1);
    }

    FILE *arquivoPedidos = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arquivoPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de pedidos.\n");
        exit(1);
    }

    FILE *arquivoIndicesJoias = fopen(ARQUIVO_INDICES_JOIAS, "wb");
    if (arquivoIndicesJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de indices de joias.\n");
        exit(1);
    }

    FILE *arquivoIndicesPedidos = fopen(ARQUIVO_INDICES_PEDIDOS, "wb");
    if (arquivoIndicesPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de indices de pedidos.\n");
        exit(1);
    }

    JOIA joia;
    PEDIDO pedido;
    INDICE indice;

    int contador = 0;
    while (1) {
        long long id_final = -1;
        int posicao_inicial = contador;

        for (int i = 0; i < TAMANHO_BLOCO_INDICE; i++) {
            if (fread(&joia, sizeof(JOIA), 1, arquivoJoias) != 1) {
                break;
            }
            id_final = joia.id;
            contador++;
        }

        if (id_final == -1) {
            break;
        }

        indice.id_final = id_final;
        indice.posicao_inicial = posicao_inicial;
        fwrite(&indice, sizeof(INDICE), 1, arquivoIndicesJoias);

        if (feof(arquivoJoias)) {
             break;
        }
    }

    contador = 0;
    while (1) {
        long long id_final = -1;
        int posicao_inicial = contador;

        for (int i = 0; i < TAMANHO_BLOCO_INDICE; i++) {
            if (fread(&pedido, sizeof(PEDIDO), 1, arquivoPedidos) != 1) {
                break;
            }
            id_final = pedido.id;
            contador++;
        }

        if (id_final == -1) {
            break;
        }

        indice.id_final = id_final;
        indice.posicao_inicial = posicao_inicial;
        fwrite(&indice, sizeof(INDICE), 1, arquivoIndicesPedidos);

        if (feof(arquivoPedidos)) {
             break;
        }
    }

    fclose(arquivoJoias);
    fclose(arquivoPedidos);
    fclose(arquivoIndicesJoias);
    fclose(arquivoIndicesPedidos);
}

int verificarExistenciaArquivosDados() {
    FILE *arquivoJoias = fopen(ARQUIVO_JOIAS, "rb");
    if (arquivoJoias == NULL) {
        return 0;
    }
    fclose(arquivoJoias);

    FILE *arquivoPedidos = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arquivoPedidos == NULL) {
        return 0;
    }
    fclose(arquivoPedidos);

    return 1;
}

int verificarExistenciaArquivosIndices() {
    FILE *arquivoIndicesJoias = fopen(ARQUIVO_INDICES_JOIAS, "rb");
    if (arquivoIndicesJoias == NULL) {
        return 0;
    }
    fclose(arquivoIndicesJoias);

    FILE *arquivoIndicesPedidos = fopen(ARQUIVO_INDICES_PEDIDOS, "rb");
    if (arquivoIndicesPedidos == NULL) {
        return 0;
    }
    fclose(arquivoIndicesPedidos);

    return 1;
}

void gravarVetoresDadosEmArquivos(PEDIDO *vetorPedidos, long quantidadePedidos, JOIA *vetorJoias, long quantidadeJoias) {
    FILE *arquivoPedidos = fopen(ARQUIVO_PEDIDOS, "wb");

    if (arquivoPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de pedidos.\n");
        exit(1);
    }

    FILE *arquivoJoias = fopen(ARQUIVO_JOIAS, "wb");

    if (arquivoJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de joias.\n");
        exit(1);
    }

    for (long i = 0; i < quantidadePedidos; i++) {
        if (i == 0 || vetorPedidos[i].id != vetorPedidos[i - 1].id) {
            fwrite(&vetorPedidos[i], sizeof(PEDIDO), 1, arquivoPedidos);
        }
    }

    for (long i = 0; i < quantidadeJoias; i++) {
        if (i == 0 || vetorJoias[i].id != vetorJoias[i - 1].id) {
            fwrite(&vetorJoias[i], sizeof(JOIA), 1, arquivoJoias);
        }
    }

    fclose(arquivoPedidos);
    fclose(arquivoJoias);

    free(vetorPedidos);
    free(vetorJoias);
}

void criarArquivosDados() {
    FILE *arquivoEntrada = fopen(ARQUIVO_ENTRADA, "r");
    if (arquivoEntrada == NULL) {
        printf("\nErro ao abrir o arquivo csv de entrada.\n");
        exit(1);
    }

    // Vetores dinâmicos.
    PEDIDO *vetorPedidos = malloc(CAPACIDADE_INICIAL_VETOR * sizeof(PEDIDO));
    JOIA *vetorJoias = malloc(CAPACIDADE_INICIAL_VETOR * sizeof(JOIA));

    if (!vetorPedidos || !vetorJoias) {
        printf("\nErro de alocacao de memoria.\n");
        exit(1);
    }

    long capacidade = CAPACIDADE_INICIAL_VETOR;
    long quantidadePedidos = 0;
    long quantidadeJoias = 0;

    char linha[TAM_LINHA_ARQUIVO_ENTRADA];

    while (fgets(linha, sizeof(linha), arquivoEntrada)) {
        linha[strcspn(linha, "\n")] = '\0';

        char *campos[10] = {NULL};
        int campoAtual = 0;
        char *inicio = linha;

        while (campoAtual < 10) {
            char *virgula = strchr(inicio, ',');
            if (virgula) {
                *virgula = '\0';
                campos[campoAtual++] = inicio;
                inicio = virgula + 1;
            } else {
                campos[campoAtual++] = inicio;
                break;
            }
        }

        while (campoAtual < 10) {
            campos[campoAtual++] = "";
        }

        // Montagem do pedido.
        PEDIDO pedido;
        memset(&pedido, 0, sizeof(pedido));

        strncpy(pedido.date_time, campos[0], sizeof(pedido.date_time) - 1);
        pedido.date_time[sizeof(pedido.date_time) - 1] = '\0';
        pedido.id = strParaLongLong(campos[1]);
        pedido.id_produto = strParaLongLong(campos[2]);
        pedido.quantidade_sku = strParaInt(campos[3]);
        pedido.preco = strParaFloat(campos[7]);
        pedido.id_usuario = strParaLongLong(campos[8]);
        pedido.elo = -1;
        pedido.fl_exclusao = 0;

        // Montagem da joia.
        JOIA joia;
        memset(&joia, 0, sizeof(joia));

        joia.id = pedido.id_produto;
        joia.id_categoria = strParaLongLong(campos[4]);

        if (strlen(campos[5]) > 0) {
            strncpy(joia.alias_categoria, campos[5], sizeof(joia.alias_categoria) - 1);
        } else {
            strcpy(joia.alias_categoria, "");
        }
        joia.alias_categoria[sizeof(joia.alias_categoria) - 1] = '\0';

        joia.id_marca = strParaLongLong(campos[6]);

        if (strlen(campos[9]) > 0) {
            strncpy(joia.genero, campos[9], sizeof(joia.genero) - 1);
        } else {
            strcpy(joia.genero, "");
        }
        joia.genero[sizeof(joia.genero) - 1] = '\0';

        joia.elo = -1;
        joia.fl_exclusao = 0;

        // Realocação (se necessário).
        if (quantidadePedidos >= capacidade) {
            capacidade *= 2;
            vetorPedidos = realloc(vetorPedidos, capacidade * sizeof(PEDIDO));
            vetorJoias = realloc(vetorJoias, capacidade * sizeof(JOIA));

            if (!vetorPedidos || !vetorJoias) {
                printf("\nErro de realocacao de memoria.\n");
                exit(1);
            }
        }

        vetorPedidos[quantidadePedidos++] = pedido;
        vetorJoias[quantidadeJoias++] = joia;
    }

    fclose(arquivoEntrada);

    // Ordenação dos vetores dinâmicos.
    qsort(vetorPedidos, quantidadePedidos, sizeof(PEDIDO), compararPedidos);
    qsort(vetorJoias, quantidadeJoias, sizeof(JOIA), compararJoias);

    gravarVetoresDadosEmArquivos(vetorPedidos, quantidadePedidos, vetorJoias, quantidadeJoias);
}

int aguardarAcaoPaginacao() {
    int opcao;

    printf("\nPressione '1' para continuar ou '0' para sair: ");
    scanf("%d", &opcao);

    return opcao;
}

void exibirJoias() {
    FILE *arquivoJoias = fopen(ARQUIVO_JOIAS, "rb");
    if (arquivoJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de joias.\n");
        exit(1);
    }

    int contador = 0;
    JOIA joia;

    printf("\nLISTA DE JOIAS\n");
    while (fread(&joia, sizeof(JOIA), 1, arquivoJoias) == 1) {
        printf("\nPosicao: %d\n", contador);

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

        contador++;

        if (contador % TAM_PAGINA == 0) {
            if (!aguardarAcaoPaginacao()) {
                break;
            }
        }
    }

    if (contador == 0) {
        printf("\nNenhuma joia foi encontrada.\n");
    }

    fclose(arquivoJoias);
}

void exibirPedidos() {
    FILE *arquivoPedidos = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arquivoPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de pedidos.\n");
        exit(1);
    }

    int contador = 0;
    PEDIDO pedido;

    printf("\nLISTA DE PEDIDOS\n");
    while (fread(&pedido, sizeof(PEDIDO), 1, arquivoPedidos) == 1) {
        printf("\nPosicao: %d\n", contador);
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

        contador++;
        if (contador % TAM_PAGINA == 0) {
            int continuar = aguardarAcaoPaginacao();

            if (!continuar) {
                break;
            }
        }
    }

    if (contador == 0) {
        printf("\nNenhum pedido foi encontrado.\n");
    }

    fclose(arquivoPedidos);
}

void exibirIndicesJoias() {
    FILE *arquivoIndicesJoias = fopen(ARQUIVO_INDICES_JOIAS, "rb");
    if (arquivoIndicesJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de indices de joias.\n");
        exit(1);
    }

    int contador = 0;
    INDICE indice;

    printf("\nLISTA DE INDICES DAS JOIAS\n");
    while (fread(&indice, sizeof(INDICE), 1, arquivoIndicesJoias) == 1) {
        printf("\nPosicao: %d\n", contador);
        printf("ID da Ultima Joia: %lld\n", indice.id_final);
        printf("Posicao da Primeira Joia: %d\n", indice.posicao_inicial);

        contador++;
        if (contador % TAM_PAGINA == 0) {
            int continuar = aguardarAcaoPaginacao();

            if (!continuar) {
                break;
            }
        }
    }

    if (contador == 0) {
        printf("\nNenhum indice de joia foi encontrado.\n");
    }

    fclose(arquivoIndicesJoias);
}

void exibirIndicesPedidos() {
    FILE *arquivoIndicesPedidos = fopen(ARQUIVO_INDICES_PEDIDOS, "rb");
    if (arquivoIndicesPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de indices de pedidos.\n");
        exit(1);
    }

    int contador = 0;
    INDICE indice;

    printf("\nLISTA DE INDICES DE PEDIDOS\n");
    while (fread(&indice, sizeof(INDICE), 1, arquivoIndicesPedidos) == 1) {
        printf("\nPosicao: %d\n", contador);
        printf("ID do Ultimo Pedido: %lld\n", indice.id_final);
        printf("Posicao do Primeiro Pedido: %d\n", indice.posicao_inicial);

        contador++;
        if (contador % TAM_PAGINA == 0) {
            int continuar = aguardarAcaoPaginacao();

            if (!continuar) {
                break;
            }
        }
    }

    if (contador == 0) {
        printf("\nNenhum indice de pedido foi encontrado.\n");
    }

    fclose(arquivoIndicesPedidos);
}

void exibirMenu() {
    printf("MENU\n");
    printf("1 - Exibir joias\n");
    printf("2 - Exibir pedidos\n");
    printf("3 - Exibir indices das joias\n");
    printf("4 - Exibir indices dos pedidos\n");
    printf("0 - Sair\n");
    printf("\nSelecione: ");
}

void processarOpcaoMenu(int opcao) {
    switch (opcao) {
        case 0:
            break;
        case 1:
            exibirJoias();
            break;
        case 2:
            exibirPedidos();
            break;
        case 3:
            exibirIndicesJoias();
            break;
        case 4:
            exibirIndicesPedidos();
            break;
        default:
            printf("\nOpcao invalida.\n");
            break;
    }
}
