# Trabalho 1 e 2 de Algoritmos e Estrutura de Dados II

**Aluna:** Sthefany Machado Lopes.

## Relatório sobre o Trabalho 2

**Dados das Tabelas Hash:**
- Tamanho: 2503 (número primo).

**Dados das Árvores B:**
- Ordem: 100.
- Número máximo de chaves em cada nodo: 200.
- Número máximo de filhos em cada nodo: 201.

### Comparação de Tempos entre Tabelas Hash, Árvores B e Índices em Arquivos:

Os tempos são aproximados e foram medidos em segundos.

| Operações Gerais       | Tabela Hash de Joias  | Árvore B de Joias  | Índice em Arquivo de Joias  | Tabela Hash de Pedidos  | Árvore B de Pedidos  | Índice em Arquivo de Pedidos  |
|------------------------|-----------------------|--------------------|-----------------------------|-------------------------|----------------------|-------------------------------|
| Criação dos índices    | 0.0030000000          | 0.0010000000       | 0.0105000000                | 0.0160000000            | 0.0100000000         | 0.0105000000                  |
| Recriação dos índices  | 0.0020000000          | 0.0010000000       | 0.0100000000                | 0.0240000000            | 0.0100000000         | 0.0100000000                  |

| Descrição              | Operações de Consulta                            | Tabela Hash de Joias  |
|------------------------|--------------------------------------------------|-----------------------|
| Item no início         | Consulta ao ID de Categoria 1806829201915904347  | 0.2020000000          |
| Item no meio           | Consulta ao ID de Categoria 1806829201924292956  | 0.0570000000          |
| Item inexistente       | Consulta ao ID de Categoria 1000000000000000000  | 0.0000000000          |
| Item inserido no meio  | Consulta ao ID de Categoria 1500000000000000000  | 0.0010000000          |

| Descrição              | Operações de Consulta               | Árvore B de Joias  | Índice em Arquivo de Joias  |
|------------------------|-------------------------------------|--------------------|-----------------------------|
| Item no início         | Consulta ao ID 1313550904313512734  | 0.0010000000       | 0.0010000000                |
| Item no meio           | Consulta ao ID 1515966223093480615  | 0.0010000000       | 0.0020000000                |
| Item inexistente       | Consulta ao ID 1000000000000000000  | 0.0000000000       | 0.0030000000                |
| Item inserido no meio  | Consulta ao ID 1463725899001102733  | 0.0020000000       | 0.0010000000                |

| Descrição              | Operações de Consulta                           | Tabela Hash de Pedidos  |
|------------------------|-------------------------------------------------|-------------------------|
| Item no início         | Consulta a Data e Hora 2018-12-01 11:40:29 UTC  | 0.0010000000            |
| Item no meio           | Consulta a Data e Hora 2019-02-03 10:31:38 UTC  | 0.0020000000            |
| Item inexistente       | Consulta a Data e Hora 2000-01-01 00:30:00 UTC  | 0.0010000000            |
| Item inserido no meio  | Consulta a Data e Hora 2019-01-01 00:00:00 UTC  | 0.0020000000            |

| Descrição              | Operações de Consulta               | Árvore B de Pedidos  | Índice em Arquivo de Pedidos  |
|------------------------|-------------------------------------|----------------------|-------------------------------|
| Item no início         | Consulta ao ID 1924719191579951782  | 0.0010000000         | 0.0010000000                  |
| Item no meio           | Consulta ao ID 1971070191756378677  | 0.0020000000         | 0.0010000000                  |
| Item inexistente       | Consulta ao ID 1000000000000000000  | 0.0000000000         | 0.0030000000                  |
| Item inserido no meio  | Consulta ao ID 1971070191756378678  | 0.0020000000         | 0.0020000000                  |