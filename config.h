#ifndef CONFIG_H
#define CONFIG_H

// Configurações de arquivos:
#define ARQUIVO_ENTRADA "dados/jewelry.csv"
#define ARQUIVO_JOIAS "dados/joias.bin"
#define ARQUIVO_PEDIDOS "dados/pedidos.bin"

#define TAM_LINHA_ARQUIVO_ENTRADA 512

#define ARQUIVO_INDICES_JOIAS "dados/indices_joias.bin"
#define ARQUIVO_INDICES_PEDIDOS "dados/indices_pedidos.bin"

#define TAMANHO_BLOCO_INDICE 1000

// Configurações para alocações de memória:
#define CAPACIDADE_INICIAL_VETOR 10000

// Configurações de paginação:
#define TAM_PAGINA 50

#endif
