#ifndef CONFIG_H
#define CONFIG_H

// Configurações de arquivos.
#define ARQUIVO_ENTRADA "dados/jewelry.csv"
#define ARQUIVO_JOIAS "dados/joias.bin"
#define ARQUIVO_PEDIDOS "dados/pedidos.bin"

#define TAM_LINHA_ARQUIVO_ENTRADA 512

#define ARQUIVO_INDICES_JOIAS "dados/indices_joias.bin"
#define ARQUIVO_INDICES_PEDIDOS "dados/indices_pedidos.bin"

#define ARQUIVO_AUXILIAR_JOIAS "dados/joias_auxiliar.bin"
#define ARQUIVO_AUXILIAR_PEDIDOS "dados/pedidos_auxiliar.bin"

#define TAMANHO_BLOCO_INDICE 10000

// Configurações de arquivos de criptografia.
#define XOR_KEY 0xAA

#define ARQUIVO_JOIAS_CRIPT "dados/criptografia/joias_criptografado.bin"
#define ARQUIVO_PEDIDOS_CRIPT "dados/criptografia/pedidos_criptografado.bin"

#define ARQUIVO_JOIAS_DESCRIPT "dados/criptografia/joias_descriptografado.bin"
#define ARQUIVO_PEDIDOS_DESCRIPT "dados/criptografia/pedidos_descriptografado.bin"

// Configurações de hash.
#define TAM_HASH 5003

// Configurações para alocações de memória.
#define CAPACIDADE_INICIAL_VETOR 10000

// Configurações de paginação.
#define TAM_PAGINA 50
#define TAM_PAGINA_BUCKETS 3

// Configurações de reorganização.
#define MAX_MODIFICACOES 50

#endif
