#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "funcoes.h"
#include "funcoes_criptografia.h"
#include "funcoes_utilitarias.h"
#include "structs.h"
#include "config.h"

int totalInsercoes = 0;
int totalExclusoes = 0;

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

    // Vetores dinamicos.
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

        // Realocacao (se necessario).
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

    // Ordenacao dos vetores dinamicos.
    qsort(vetorPedidos, quantidadePedidos, sizeof(PEDIDO), compararPedidos);
    qsort(vetorJoias, quantidadeJoias, sizeof(JOIA), compararJoias);

    gravarVetoresDadosEmArquivos(vetorPedidos, quantidadePedidos, vetorJoias, quantidadeJoias);
}

void exibirJoiasOrdenacaoFisica(const char *nomeArquivoJoias) {
    FILE *arquivoJoias = fopen(nomeArquivoJoias, "rb");
    if (arquivoJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de joias.\n");
        exit(1);
    }

    int contador = 0;
    JOIA joia;

    printf("\nLISTA DE JOIAS (ORDEM FISICA)\n");
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

void exibirPedidosOrdenacaoFisica(const char *nomeArquivoPedidos) {
    FILE *arquivoPedidos = fopen(nomeArquivoPedidos, "rb");
    if (arquivoPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de pedidos.\n");
        exit(1);
    }

    int contador = 0;
    PEDIDO pedido;

    printf("\nLISTA DE PEDIDOS (ORDEM FISICA)\n");
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

void exibirJoiasOrdenacaoLogica() {
    FILE *arquivoIndicesJoias= fopen(ARQUIVO_INDICES_JOIAS, "rb");
    if (arquivoIndicesJoias == NULL) {
        printf("\nNenhuma joia foi encontrada.\n");
        return;
    }

    INDICE indice;
    if (fread(&indice, sizeof(INDICE), 1, arquivoIndicesJoias) != 1) {
        printf("\nNenhuma joia foi encontrada.\n");
        fclose(arquivoIndicesJoias);
        return;
    }

    int posicaoAtual = indice.posicao_inicial;

    fseek(arquivoIndicesJoias, -sizeof(INDICE), SEEK_END);
    fread(&indice, sizeof(INDICE), 1, arquivoIndicesJoias);
    long long idFinal = indice.id_final;

    fclose(arquivoIndicesJoias);

    FILE *arquivoJoias = fopen(ARQUIVO_JOIAS, "rb");
    if (arquivoJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de joias.\n");
        exit(1);
    }

    fseek(arquivoJoias, posicaoAtual * sizeof(JOIA), SEEK_SET);

    JOIA joia;
    int contador = 0;

    printf("\nLISTA DE JOIAS (ORDEM LOGICA)\n");
    while (fread(&joia, sizeof(JOIA), 1, arquivoJoias) == 1) {
        printf("\nPosicao: %d\n", posicaoAtual);
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

        if (joia.id == idFinal) {
            break;
        }

        contador++;
        if (contador % TAM_PAGINA == 0) {
            if (!aguardarAcaoPaginacao()) {
                break;
            }
        }

        if (joia.elo != -1 && joia.elo != posicaoAtual + 1) {
            fseek(arquivoJoias, joia.elo * sizeof(JOIA), SEEK_SET);
            posicaoAtual = joia.elo;
        } else {
            posicaoAtual++;
        }
    }

    fclose(arquivoJoias);
}

void exibirPedidosOrdenacaoLogica() {
    FILE *arquivoIndicesPedidos = fopen(ARQUIVO_INDICES_PEDIDOS, "rb");
    if (arquivoIndicesPedidos == NULL) {
        printf("\nNenhum pedido foi encontrado.\n");
        return;
    }

    INDICE indice;
    if (fread(&indice, sizeof(INDICE), 1, arquivoIndicesPedidos) != 1) {
        printf("\nNenhum pedido foi encontrado.\n");
        fclose(arquivoIndicesPedidos);
        return;
    }

    int posicaoAtual = indice.posicao_inicial;

    fseek(arquivoIndicesPedidos, -sizeof(INDICE), SEEK_END);
    fread(&indice, sizeof(INDICE), 1, arquivoIndicesPedidos);

    long long idFinal = indice.id_final;

    fclose(arquivoIndicesPedidos);

    FILE *arquivoPedidos = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arquivoPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de pedidos.\n");
        exit(1);
    }

    fseek(arquivoPedidos, posicaoAtual * sizeof(PEDIDO), SEEK_SET);

    PEDIDO pedido;
    int contador = 0;

    printf("\nLISTA DE PEDIDOS (ORDEM LOGICA)\n");
    while (fread(&pedido, sizeof(PEDIDO), 1, arquivoPedidos) == 1) {
        printf("\nPosicao: %d\n", posicaoAtual);
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

        if (pedido.id == idFinal) {
            break;
        }

        contador++;
        if (contador % TAM_PAGINA == 0) {
            if (!aguardarAcaoPaginacao()) {
                break;
            }
        }

        if (pedido.elo != -1 && pedido.elo != posicaoAtual + 1) {
            fseek(arquivoPedidos, pedido.elo * sizeof(PEDIDO), SEEK_SET);
            posicaoAtual = pedido.elo;
        } else {
            posicaoAtual++;
        }
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

int buscarIndicePorId(FILE *arquivoIndices, long long idBusca) {
    INDICE indice;
    long long inicio = 0;

    fseek(arquivoIndices, 0, SEEK_END);

    long long tamanho = ftell(arquivoIndices);
    long long totalIndices = tamanho / sizeof(INDICE);
    long long fim = totalIndices - 1;

    int blocoEncontrado = -1;

    while (inicio <= fim) {
        long long meio = (inicio + fim) / 2;
        fseek(arquivoIndices, meio * sizeof(INDICE), SEEK_SET);
        fread(&indice, sizeof(INDICE), 1, arquivoIndices);

        if (idBusca <= indice.id_final) {
            blocoEncontrado = meio;
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }

    return blocoEncontrado;
}

void buscarJoia() {
    FILE *arquivoIndicesJoias = fopen(ARQUIVO_INDICES_JOIAS, "rb");
    if (arquivoIndicesJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de indices de joias.\n");
        exit(1);
    }

    FILE *arquivoJoias = fopen(ARQUIVO_JOIAS, "rb");
    if (arquivoJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de joias.\n");
        exit(1);
    }

    printf("\nBUSCA POR JOIA\n");

    long long idBusca;
    printf("Informe o ID da joia: ");
    scanf("%lld", &idBusca);

    int bloco = buscarIndicePorId(arquivoIndicesJoias, idBusca);
    if (bloco == -1) {
        printf("\nJoia nao encontrada.\n");
        fclose(arquivoIndicesJoias);
        fclose(arquivoJoias);
        return;
    }

    INDICE indice;
    fseek(arquivoIndicesJoias, bloco * sizeof(INDICE), SEEK_SET);
    fread(&indice, sizeof(INDICE), 1, arquivoIndicesJoias);

    JOIA joia;
    fseek(arquivoJoias, indice.posicao_inicial * sizeof(JOIA), SEEK_SET);

    while (fread(&joia, sizeof(JOIA), 1, arquivoJoias) == 1) {
        if (joia.id > indice.id_final) {
            break;
        }

        if (joia.id == idBusca) {
            printf("\nID: %lld\n", joia.id);
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

            fclose(arquivoIndicesJoias);
            fclose(arquivoJoias);
            return;
        }

        if (joia.elo != -1) {
            fseek(arquivoJoias, joia.elo * sizeof(JOIA), SEEK_SET);
        }
    }

    printf("\nJoia nao encontrada.\n");
    fclose(arquivoIndicesJoias);
    fclose(arquivoJoias);
}

void buscarPedido() {
    FILE *arquivoIndicesPedidos = fopen(ARQUIVO_INDICES_PEDIDOS, "rb");
    if (arquivoIndicesPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de indices de pedidos.\n");
        exit(1);
    }

    FILE *arquivoPedidos = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arquivoPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de pedidos.\n");
        exit(1);
    }

    printf("\nBUSCA POR PEDIDO\n");

    long long idBusca;
    printf("Informe o ID do pedido: ");
    scanf("%lld", &idBusca);

    int bloco = buscarIndicePorId(arquivoIndicesPedidos, idBusca);
    if (bloco == -1) {
        printf("\nPedido nao encontrado.\n");
        fclose(arquivoIndicesPedidos);
        fclose(arquivoPedidos);
        return;
    }

    INDICE indice;
    fseek(arquivoIndicesPedidos, bloco * sizeof(INDICE), SEEK_SET);
    fread(&indice, sizeof(INDICE), 1, arquivoIndicesPedidos);

    PEDIDO pedido;
    fseek(arquivoPedidos, indice.posicao_inicial * sizeof(PEDIDO), SEEK_SET);

    while (fread(&pedido, sizeof(PEDIDO), 1, arquivoPedidos) == 1) {
        if (pedido.id > indice.id_final) {
            break;
        }

        if (pedido.id == idBusca) {
            printf("\nID do Pedido: %lld\n", pedido.id);
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

            fclose(arquivoIndicesPedidos);
            fclose(arquivoPedidos);
            return;
        }

        if (pedido.elo != -1) {
            fseek(arquivoPedidos, pedido.elo * sizeof(PEDIDO), SEEK_SET);
        }
    }

    printf("\nPedido nao encontrado.\n");
    fclose(arquivoIndicesPedidos);
    fclose(arquivoPedidos);
}

JOIA criarJoia() {
    JOIA joia;

    printf("Insira o ID da Joia: ");
    scanf("%lld", &joia.id);

    printf("Insira o ID da Categoria: ");
    scanf("%lld", &joia.id_categoria);

    getchar();
    printf("Insira o Alias da Categoria: ");
    gets(joia.alias_categoria);

    printf("Insira o ID da Marca: ");
    scanf("%lld", &joia.id_marca);

    getchar();
    printf("Insira o Genero: ");
    gets(joia.genero);

    joia.elo = -1;
    joia.fl_exclusao = 0;

    return joia;
}

PEDIDO criarPedido() {
    PEDIDO pedido;

    printf("Insira o ID do Pedido: ");
    scanf("%lld", &pedido.id);

    printf("Insira o ID do Produto: ");
    scanf("%lld", &pedido.id_produto);

    getchar();
    printf("Insira a Data e Hora: ");
    gets(pedido.date_time);

    printf("Insira a Quantidade SKU: ");
    scanf("%d", &pedido.quantidade_sku);

    printf("Insira o Preco: ");
    scanf("%f", &pedido.preco);

    printf("Insira o ID do Usuario: ");
    scanf("%lld", &pedido.id_usuario);

    pedido.elo = -1;
    pedido.fl_exclusao = 0;

    return pedido;
}

void inserirJoia() {
    printf("\nINSERIR JOIA\n");

    JOIA joiaNova = criarJoia();

    FILE *arquivoIndicesJoias = fopen(ARQUIVO_INDICES_JOIAS, "rb+");
    if (arquivoIndicesJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de indices de joias.\n");
        exit(1);
    }

    FILE *arquivoJoias = fopen(ARQUIVO_JOIAS, "rb+");
    if (arquivoJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de joias.\n");
        exit(1);
    }

    fseek(arquivoJoias, 0, SEEK_END);
    int posicaoNovaJoia = ftell(arquivoJoias) / sizeof(JOIA);

    JOIA joiaAtual;

    int bloco = buscarIndicePorId(arquivoIndicesJoias, joiaNova.id);
    if (bloco == -1) {
        // A nova joia sera a ultima do arquivo.
        INDICE ultimoIndice;
        fseek(arquivoIndicesJoias, -sizeof(INDICE), SEEK_END);
        fread(&ultimoIndice, sizeof(INDICE), 1, arquivoIndicesJoias);

        fseek(arquivoJoias, ultimoIndice.posicao_inicial * sizeof(JOIA), SEEK_SET);

        while (fread(&joiaAtual, sizeof(JOIA), 1, arquivoJoias) == 1) {
            if (joiaAtual.id == ultimoIndice.id_final) {
                fseek(arquivoJoias, -sizeof(JOIA), SEEK_CUR);
                joiaAtual.elo = posicaoNovaJoia;
                fwrite(&joiaAtual, sizeof(JOIA), 1, arquivoJoias);

                fseek(arquivoJoias, 0, SEEK_END);
                fwrite(&joiaNova, sizeof(JOIA), 1, arquivoJoias);

                ultimoIndice.id_final = joiaNova.id;
                fseek(arquivoIndicesJoias, -sizeof(INDICE), SEEK_END);
                fwrite(&ultimoIndice, sizeof(INDICE), 1, arquivoIndicesJoias);

                fclose(arquivoIndicesJoias);
                fclose(arquivoJoias);

                totalInsercoes++;
                return;
            }

            if (joiaAtual.elo != -1) {
                fseek(arquivoJoias, joiaAtual.elo * sizeof(JOIA), SEEK_SET);
            }
        }
    } else {
        // A nova joia pertence a um bloco.
        INDICE indice;
        fseek(arquivoIndicesJoias, bloco * sizeof(INDICE), SEEK_SET);
        fread(&indice, sizeof(INDICE), 1, arquivoIndicesJoias);

        fseek(arquivoJoias, indice.posicao_inicial * sizeof(JOIA), SEEK_SET);

        JOIA joiaAnterior;
        joiaAnterior.id = -1;
        int posicaoJoiaAnterior = -1;
        while (fread(&joiaAtual, sizeof(JOIA), 1, arquivoJoias) == 1) {
            if (joiaAtual.id == joiaNova.id) {
                // Nao sera inserida uma joia com o mesmo ID.
                fclose(arquivoIndicesJoias);
                fclose(arquivoJoias);
                return;
            } else if (joiaAtual.id > joiaNova.id) {
                if (joiaAnterior.id == -1) {
                    // A nova joia sera a primeira do bloco.
                    joiaNova.elo = indice.posicao_inicial;
                    fseek(arquivoJoias, 0, SEEK_END);
                    fwrite(&joiaNova, sizeof(JOIA), 1, arquivoJoias);

                    indice.posicao_inicial = posicaoNovaJoia;
                    fseek(arquivoIndicesJoias, -sizeof(INDICE), SEEK_CUR);
                    fwrite(&indice, sizeof(INDICE), 1, arquivoIndicesJoias);

                    if (bloco != 0) {
                        INDICE indiceBlocoAnterior;
                        fseek(arquivoIndicesJoias, (bloco - 1) * sizeof(INDICE), SEEK_SET);
                        fread(&indiceBlocoAnterior, sizeof(INDICE), 1, arquivoIndicesJoias);

                        fseek(arquivoJoias, indiceBlocoAnterior.posicao_inicial * sizeof(JOIA), SEEK_SET);

                        JOIA ultimaJoiaBlocoAnterior;
                        while (fread(&ultimaJoiaBlocoAnterior, sizeof(JOIA), 1, arquivoJoias) == 1) {
                            if (ultimaJoiaBlocoAnterior.id == indiceBlocoAnterior.id_final) {
                                ultimaJoiaBlocoAnterior.elo = posicaoNovaJoia;
                                fseek(arquivoJoias, -sizeof(JOIA), SEEK_CUR);
                                fwrite(&ultimaJoiaBlocoAnterior, sizeof(JOIA), 1, arquivoJoias);
                                break;
                            }

                            if (ultimaJoiaBlocoAnterior.elo != -1) {
                                fseek(arquivoJoias, ultimaJoiaBlocoAnterior.elo * sizeof(JOIA), SEEK_SET);
                            }
                        }
                    }

                    fclose(arquivoIndicesJoias);
                    fclose(arquivoJoias);

                    totalInsercoes++;
                    return;
                } else {
                    // Nao sera a primeira joia do bloco e nem a ultima do bloco.
                    joiaNova.elo = (joiaAnterior.elo != -1) ? joiaAnterior.elo : (ftell(arquivoJoias) / sizeof(JOIA)) - 1;

                    fseek(arquivoJoias, 0, SEEK_END);
                    fwrite(&joiaNova, sizeof(JOIA), 1, arquivoJoias);

                    joiaAnterior.elo = posicaoNovaJoia;
                    fseek(arquivoJoias, posicaoJoiaAnterior * sizeof(JOIA), SEEK_SET);
                    fwrite(&joiaAnterior, sizeof(JOIA), 1, arquivoJoias);

                    fclose(arquivoIndicesJoias);
                    fclose(arquivoJoias);

                    totalInsercoes++;
                    return;
                }
            }

            posicaoJoiaAnterior = (ftell(arquivoJoias) / sizeof(JOIA)) - 1;

            if (joiaAtual.elo != -1) {
                fseek(arquivoJoias, joiaAtual.elo * sizeof(JOIA), SEEK_SET);
            }

            joiaAnterior = joiaAtual;
        }
    }

    fclose(arquivoIndicesJoias);
    fclose(arquivoJoias);
}

void inserirPedido() {
    printf("\nINSERIR PEDIDO\n");

    PEDIDO pedidoNovo = criarPedido();

    FILE *arquivoIndicesPedidos = fopen(ARQUIVO_INDICES_PEDIDOS, "rb+");
    if (arquivoIndicesPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de indices de pedidos.\n");
        exit(1);
    }

    FILE *arquivoPedidos = fopen(ARQUIVO_PEDIDOS, "rb+");
    if (arquivoPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de pedidos.\n");
        exit(1);
    }

    PEDIDO pedidoAtual;

    fseek(arquivoPedidos, 0, SEEK_END);
    int posicaoNovoPedido = ftell(arquivoPedidos) / sizeof(PEDIDO);

    int bloco = buscarIndicePorId(arquivoIndicesPedidos, pedidoNovo.id);
    if (bloco == -1) {
        // O novo pedido sera o ultimo do arquivo.
        INDICE ultimoIndice;
        fseek(arquivoIndicesPedidos, -sizeof(INDICE), SEEK_END);
        fread(&ultimoIndice, sizeof(INDICE), 1, arquivoIndicesPedidos);

        fseek(arquivoPedidos, ultimoIndice.posicao_inicial * sizeof(PEDIDO), SEEK_SET);

        while (fread(&pedidoAtual, sizeof(PEDIDO), 1, arquivoPedidos) == 1) {
            if (pedidoAtual.id == ultimoIndice.id_final) {
                fseek(arquivoPedidos, -sizeof(PEDIDO), SEEK_CUR);
                pedidoAtual.elo = posicaoNovoPedido;
                fwrite(&pedidoAtual, sizeof(PEDIDO), 1, arquivoPedidos);

                fseek(arquivoPedidos, 0, SEEK_END);
                fwrite(&pedidoNovo, sizeof(PEDIDO), 1, arquivoPedidos);

                ultimoIndice.id_final = pedidoNovo.id;
                fseek(arquivoIndicesPedidos, -sizeof(INDICE), SEEK_END);
                fwrite(&ultimoIndice, sizeof(INDICE), 1, arquivoIndicesPedidos);

                fclose(arquivoIndicesPedidos);
                fclose(arquivoPedidos);
                return;
            }

            if (pedidoAtual.elo != -1) {
                fseek(arquivoPedidos, pedidoAtual.elo * sizeof(PEDIDO), SEEK_SET);
            }
        }
    } else {
        // O novo pedido pertence a um bloco.
        INDICE indice;
        fseek(arquivoIndicesPedidos, bloco * sizeof(INDICE), SEEK_SET);
        fread(&indice, sizeof(INDICE), 1, arquivoIndicesPedidos);

        fseek(arquivoPedidos, indice.posicao_inicial * sizeof(PEDIDO), SEEK_SET);

        PEDIDO pedidoAnterior;
        pedidoAnterior.id = -1;
        int posicaoPedidoAnterior = -1;
        while (fread(&pedidoAtual, sizeof(PEDIDO), 1, arquivoPedidos) == 1) {
            if (pedidoAtual.id == pedidoNovo.id) {
                // Nao sera inserido um novo pedido com o mesmo ID.
                fclose(arquivoIndicesPedidos);
                fclose(arquivoPedidos);
                return;
            } else if (pedidoAtual.id > pedidoNovo.id) {
                if (pedidoAnterior.id == -1) {
                    // O novo pedido sera o primeiro do bloco.
                    pedidoNovo.elo = indice.posicao_inicial;
                    fseek(arquivoPedidos, 0, SEEK_END);
                    fwrite(&pedidoNovo, sizeof(PEDIDO), 1, arquivoPedidos);

                    indice.posicao_inicial = posicaoNovoPedido;
                    fseek(arquivoIndicesPedidos, -sizeof(INDICE), SEEK_CUR);
                    fwrite(&indice, sizeof(INDICE), 1, arquivoIndicesPedidos);

                    if (bloco != 0) {
                        INDICE indiceBlocoAnterior;
                        fseek(arquivoIndicesPedidos, (bloco - 1) * sizeof(INDICE), SEEK_SET);
                        fread(&indiceBlocoAnterior, sizeof(INDICE), 1, arquivoIndicesPedidos);

                        fseek(arquivoPedidos, indiceBlocoAnterior.posicao_inicial * sizeof(PEDIDO), SEEK_SET);

                        PEDIDO ultimoPedidoBlocoAnterior;
                        while (fread(&ultimoPedidoBlocoAnterior, sizeof(PEDIDO), 1, arquivoPedidos) == 1) {
                            if (ultimoPedidoBlocoAnterior.id == indiceBlocoAnterior.id_final) {
                                ultimoPedidoBlocoAnterior.elo = posicaoNovoPedido;
                                fseek(arquivoPedidos, -sizeof(PEDIDO), SEEK_CUR);
                                fwrite(&ultimoPedidoBlocoAnterior, sizeof(PEDIDO), 1, arquivoPedidos);
                                break;
                            }

                            if (ultimoPedidoBlocoAnterior.elo != -1) {
                                fseek(arquivoPedidos, ultimoPedidoBlocoAnterior.elo * sizeof(PEDIDO), SEEK_SET);
                            }
                        }
                    }

                    fclose(arquivoIndicesPedidos);
                    fclose(arquivoPedidos);

                    totalInsercoes++;
                    return;
                } else {
                    pedidoNovo.elo = (pedidoAnterior.elo != -1) ? pedidoAnterior.elo : (ftell(arquivoPedidos) / sizeof(PEDIDO)) - 1;

                    fseek(arquivoPedidos, 0, SEEK_END);
                    fwrite(&pedidoNovo, sizeof(PEDIDO), 1, arquivoPedidos);

                    pedidoAnterior.elo = posicaoNovoPedido;
                    fseek(arquivoPedidos, posicaoPedidoAnterior * sizeof(PEDIDO), SEEK_SET);
                    fwrite(&pedidoAnterior, sizeof(PEDIDO), 1, arquivoPedidos);

                    fclose(arquivoIndicesPedidos);
                    fclose(arquivoPedidos);

                    totalInsercoes++;
                    return;
                }
            }

            posicaoPedidoAnterior = (ftell(arquivoPedidos) / sizeof(PEDIDO)) - 1;

            if (pedidoAtual.elo != -1) {
                fseek(arquivoPedidos, pedidoAtual.elo * sizeof(PEDIDO), SEEK_SET);
            }

            pedidoAnterior = pedidoAtual;
        }
    }

    fclose(arquivoIndicesPedidos);
    fclose(arquivoPedidos);
}

void excluirJoia() {
    FILE *arquivoIndicesJoias = fopen(ARQUIVO_INDICES_JOIAS, "rb");
    if (arquivoIndicesJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de indices de joias.\n");
        exit(1);
    }

    FILE *arquivoJoias = fopen(ARQUIVO_JOIAS, "rb+");
    if (arquivoJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de joias.\n");
        exit(1);
    }

    printf("\nEXCLUIR JOIA\n");

    long long idBusca;
    printf("Informe o ID da joia: ");
    scanf("%lld", &idBusca);

    int bloco = buscarIndicePorId(arquivoIndicesJoias, idBusca);
    if (bloco == -1) {
        printf("\nJoia nao encontrada.\n");
        fclose(arquivoIndicesJoias);
        fclose(arquivoJoias);
        return;
    }

    INDICE indice;
    fseek(arquivoIndicesJoias, bloco * sizeof(INDICE), SEEK_SET);
    fread(&indice, sizeof(INDICE), 1, arquivoIndicesJoias);

    JOIA joia;
    fseek(arquivoJoias, indice.posicao_inicial * sizeof(JOIA), SEEK_SET);

    while (fread(&joia, sizeof(JOIA), 1, arquivoJoias) == 1) {
        if (joia.id > indice.id_final) {
            break;
        }

        if (joia.id == idBusca) {
            joia.fl_exclusao = 1;
            fseek(arquivoJoias, -1 * sizeof(JOIA), SEEK_CUR);
            fwrite(&joia, sizeof(JOIA), 1, arquivoJoias);

            totalExclusoes++;

            fclose(arquivoIndicesJoias);
            fclose(arquivoJoias);
            return;
        }

        if (joia.elo != -1) {
            fseek(arquivoJoias, joia.elo * sizeof(JOIA), SEEK_SET);
        }
    }

    printf("\nJoia nao encontrada.\n");
    fclose(arquivoIndicesJoias);
    fclose(arquivoJoias);
}

void excluirPedido() {
    FILE *arquivoIndicesPedidos = fopen(ARQUIVO_INDICES_PEDIDOS, "rb");
    if (arquivoIndicesPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de indices de pedidos.\n");
        exit(1);
    }

    FILE *arquivoPedidos = fopen(ARQUIVO_PEDIDOS, "rb+");
    if (arquivoPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de pedidos.\n");
        exit(1);
    }

    printf("\nEXCLUIR PEDIDO\n");

    long long idBusca;
    printf("Informe o ID do pedido: ");
    scanf("%lld", &idBusca);

    int bloco = buscarIndicePorId(arquivoIndicesPedidos, idBusca);
    if (bloco == -1) {
        printf("\nPedido nao encontrado.\n");
        fclose(arquivoIndicesPedidos);
        fclose(arquivoPedidos);
        return;
    }

    INDICE indice;
    fseek(arquivoIndicesPedidos, bloco * sizeof(INDICE), SEEK_SET);
    fread(&indice, sizeof(INDICE), 1, arquivoIndicesPedidos);

    PEDIDO pedido;
    fseek(arquivoPedidos, indice.posicao_inicial * sizeof(PEDIDO), SEEK_SET);

    while (fread(&pedido, sizeof(PEDIDO), 1, arquivoPedidos) == 1) {
        if (pedido.id > indice.id_final) {
            break;
        }

        if (pedido.id == idBusca) {
            pedido.fl_exclusao = 1;
            fseek(arquivoPedidos, -1 * sizeof(PEDIDO), SEEK_CUR);
            fwrite(&pedido, sizeof(PEDIDO), 1, arquivoPedidos);

            totalExclusoes++;

            fclose(arquivoIndicesPedidos);
            fclose(arquivoPedidos);
            return;
        }

        if (pedido.elo != -1) {
            fseek(arquivoPedidos, pedido.elo * sizeof(PEDIDO), SEEK_SET);
        }
    }

    printf("\nPedido nao encontrado.\n");
    fclose(arquivoIndicesPedidos);
    fclose(arquivoPedidos);
}

void reorganizarArquivoDadosJoias() {
    FILE *arquivoIndicesJoias= fopen(ARQUIVO_INDICES_JOIAS, "rb");
    if (arquivoIndicesJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de indices de joias.\n");
        exit(1);
    }

    INDICE indice;
    if (fread(&indice, sizeof(INDICE), 1, arquivoIndicesJoias) != 1) {
        fclose(arquivoIndicesJoias);
        return;
    }

    int posicaoAtual = indice.posicao_inicial;

    fseek(arquivoIndicesJoias, -sizeof(INDICE), SEEK_END);
    fread(&indice, sizeof(INDICE), 1, arquivoIndicesJoias);
    long long idFinal = indice.id_final;

    fclose(arquivoIndicesJoias);

    FILE *arquivoJoias = fopen(ARQUIVO_JOIAS, "rb");
    if (arquivoJoias == NULL) {
        printf("\nErro ao abrir o arquivo binario de joias.\n");
        exit(1);
    }
    fseek(arquivoJoias, posicaoAtual * sizeof(JOIA), SEEK_SET);

    FILE *arquivoJoiasAuxiliar = fopen(ARQUIVO_AUXILIAR_JOIAS, "wb");
    if (arquivoJoiasAuxiliar == NULL) {
        printf("\nErro ao abrir o arquivo auxiliar binario de joias.\n");
        exit(1);
    }

    JOIA joia;
    while (fread(&joia, sizeof(JOIA), 1, arquivoJoias) == 1) {
        if (joia.elo != -1 && joia.elo != posicaoAtual + 1) {
            fseek(arquivoJoias, joia.elo * sizeof(JOIA), SEEK_SET);
            posicaoAtual = joia.elo;
        } else {
            posicaoAtual++;
        }

        if (joia.fl_exclusao != 1) {
            joia.elo = -1;
            fwrite(&joia, sizeof(JOIA), 1, arquivoJoiasAuxiliar);
        }

        if (joia.id == idFinal) {
            break;
        }
    }

    fclose(arquivoJoias);
    fclose(arquivoJoiasAuxiliar);

    if (remove(ARQUIVO_JOIAS) != 0) {
        printf("\nErro ao excluir o arquivo original de joias.\n");
        exit(1);
    }

    if (rename(ARQUIVO_AUXILIAR_JOIAS, ARQUIVO_JOIAS) != 0) {
        printf("\nErro ao renomear o arquivo auxiliar de joias.\n");
        exit(1);
    }
}

void reorganizarArquivoDadosPedidos() {
    FILE *arquivoIndicesPedidos= fopen(ARQUIVO_INDICES_PEDIDOS, "rb");
    if (arquivoIndicesPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de indices de pedidos.\n");
        exit(1);
    }

    INDICE indice;
    if (fread(&indice, sizeof(INDICE), 1, arquivoIndicesPedidos) != 1) {
        fclose(arquivoIndicesPedidos);
        return;
    }

    int posicaoAtual = indice.posicao_inicial;

    fseek(arquivoIndicesPedidos, -sizeof(INDICE), SEEK_END);
    fread(&indice, sizeof(INDICE), 1, arquivoIndicesPedidos);
    long long idFinal = indice.id_final;

    fclose(arquivoIndicesPedidos);

    FILE *arquivoPedidos = fopen(ARQUIVO_PEDIDOS, "rb");
    if (arquivoPedidos == NULL) {
        printf("\nErro ao abrir o arquivo binario de pedidos.\n");
        exit(1);
    }
    fseek(arquivoPedidos, posicaoAtual * sizeof(PEDIDO), SEEK_SET);

    FILE *arquivoPedidosAuxiliar = fopen(ARQUIVO_AUXILIAR_PEDIDOS, "wb");
    if (arquivoPedidosAuxiliar == NULL) {
        printf("\nErro ao abrir o arquivo auxiliar binario de pedidos.\n");
        exit(1);
    }

    PEDIDO pedido;
    while (fread(&pedido, sizeof(PEDIDO), 1, arquivoPedidos) == 1) {
        if (pedido.elo != -1 && pedido.elo != posicaoAtual + 1) {
            fseek(arquivoPedidos, pedido.elo * sizeof(PEDIDO), SEEK_SET);
            posicaoAtual = pedido.elo;
        } else {
            posicaoAtual++;
        }

        if (pedido.fl_exclusao != 1) {
            pedido.elo = -1;
            fwrite(&pedido, sizeof(PEDIDO), 1, arquivoPedidosAuxiliar);
        }

        if (pedido.id == idFinal) {
            break;
        }
    }

    fclose(arquivoPedidos);
    fclose(arquivoPedidosAuxiliar);

    if (remove(ARQUIVO_PEDIDOS) != 0) {
        printf("\nErro ao excluir o arquivo original de pedidos.\n");
        exit(1);
    }

    if (rename(ARQUIVO_AUXILIAR_PEDIDOS, ARQUIVO_PEDIDOS) != 0) {
        printf("\nErro ao renomear o arquivo auxiliar de pedidos.\n");
        exit(1);
    }
}

void reorganizarArquivos() {
    printf("\nINICIANDO A REORGANIZACAO DOS ARQUIVOS DE DADOS E INDICES...\n");

    reorganizarArquivoDadosJoias();
    reorganizarArquivoDadosPedidos();
    criarArquivosIndices();

    printf("Todos os arquivos foram reorganizados com sucesso.\n\n");
}

void gerarArquivosCriptografadosDescriptografados() {
    gerarArquivosDadosCriptografados();
    gerarArquivosDadosDescriptografados();

    printf("\nTodos os arquivos foram criados com sucesso.\n");
}

void exibirJoiasArquivoDescriptografado() {
    FILE *arquivoJoiasDescriptografado = fopen(ARQUIVO_JOIAS_DESCRIPT, "rb");
    if (arquivoJoiasDescriptografado == NULL) {
        printf("\nO arquivo descriptografado de joias nao existe.\n");
        return;
    }

    fclose(arquivoJoiasDescriptografado);
    exibirJoiasOrdenacaoFisica(ARQUIVO_JOIAS_DESCRIPT);
}

void exibirPedidosArquivoDescriptografado() {
    FILE *arquivoPedidosDescriptografado = fopen(ARQUIVO_PEDIDOS_DESCRIPT, "rb");
    if (arquivoPedidosDescriptografado == NULL) {
        printf("\nO arquivo descriptografado de pedidos nao existe.\n");
        return;
    }

    fclose(arquivoPedidosDescriptografado);
    exibirPedidosOrdenacaoFisica(ARQUIVO_PEDIDOS_DESCRIPT);
}

void exibirMenu() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("MENU\n");
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    printf("1 - Exibir joias por ordenacao fisica (desordenado)\n");
    printf("2 - Exibir pedidos por ordenacao fisica (desordenado)\n");
    printf("3 - Exibir joias por ordenacao logica (ordenado)\n");
    printf("4 - Exibir pedidos por ordenacao logica (ordenado)\n");
    printf("5 - Exibir indices das joias\n");
    printf("6 - Exibir indices dos pedidos\n");
    printf("7 - Buscar joia\n");
    printf("8 - Buscar pedido\n");
    printf("9 - Inserir joia\n");
    printf("10 - Inserir pedido\n");
    printf("11 - Excluir joia\n");
    printf("12 - Excluir pedido\n");
    printf("13 - Gerar arquivos criptografados e descriptografados de joias e pedidos\n");
    printf("14 - Visualizar arquivo de joias descriptografado\n");
    printf("15 - Visualizar arquivo de pedidos descriptografado\n");
    printf("0 - Sair\n");
    printf("\nSelecione: ");
}

void processarOpcaoMenu(int opcao) {
    switch (opcao) {
        case 0:
            break;
        case 1:
            exibirJoiasOrdenacaoFisica(ARQUIVO_JOIAS);
            break;
        case 2:
            exibirPedidosOrdenacaoFisica(ARQUIVO_PEDIDOS);
            break;
        case 3:
            exibirJoiasOrdenacaoLogica();
            break;
        case 4:
            exibirPedidosOrdenacaoLogica();
            break;
        case 5:
            exibirIndicesJoias();
            break;
        case 6:
            exibirIndicesPedidos();
            break;
        case 7:
            buscarJoia();
            break;
        case 8:
            buscarPedido();
            break;
        case 9:
            inserirJoia();
            break;
        case 10:
            inserirPedido();
            break;
        case 11:
            excluirJoia();
            break;
        case 12:
            excluirPedido();
            break;
        case 13:
            gerarArquivosCriptografadosDescriptografados();
            break;
        case 14:
            exibirJoiasArquivoDescriptografado();
            break;
        case 15:
            exibirPedidosArquivoDescriptografado();
            break;
        default:
            printf("\nOpcao invalida.\n");
            break;
    }
}

void gerenciarMenu() {
    int opcao;
    do {
        exibirMenu();
        scanf("%d", &opcao);
        processarOpcaoMenu(opcao);
        printf("\n");

        if (totalInsercoes + totalExclusoes >= MAX_MODIFICACOES) {
            totalInsercoes = 0;
            totalExclusoes = 0;

            reorganizarArquivos();
        }
    } while (opcao != 0);
}
