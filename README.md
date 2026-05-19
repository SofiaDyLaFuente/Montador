# Montador Assembler
Repositório para a implementação do Trabalho 1 da disciplina de Software Básico. O projeto consiste na implementação, em C/C++, de um montador para a linguagem assembly fictícia apresentada em aula.

## Autores
- João Vitor Pereira Vieira - 211036114
- Sofia Dy La Fuente Monteiro - 211055530

## Funcionalidades Implementadas
- **Pré-processador:** Tratamento de maiúsculas/minúsculas, remoção de comentários, remoção de quebras de linha e espaços redundantes, e tratamento das diretivas `EQU` e `IF`.
- **Montador (Single-Pass):** Algoritmo de passagem única com resolução de referências futuras via **Backpatching**. Suporte a rotulos na mesma linha ou linha isolada, e leitura de valores inteiros, negativos e hexadecimais (ex: `0x14`).
- **Simulador:** Máquina virtual completa para execução do código objeto gerado, com suporte a entrada (`INPUT`) e saída (`OUTPUT`) interativas pelo terminal.

## Ambiente e Pré-requisitos
O projeto foi desenvolvido e homologado em ambiente **Linux (Ubuntu / WSL)**. Recomenda-se a execução neste sistema operacional para garantir o correto funcionamento da manipulação de caminhos de arquivos e quebras de linha.

- Compilador `g++` com suporte ao padrão C++11 (ou superior).

**Instalação das dependências (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install g++
```

## Compilação

Para compilar o projeto, utilize o seguinte comando no terminal (dentro da pasta raiz do projeto):

```bash
g++ src/*.cpp -o montador
```

## Execução do programa

### Pré-processamento

```bash
./montador arquivo.asm
```

### Montagem 

```bash
./montador arquivo.pre
```

### Simulação

```bash
./montador arquivo.obj
```

## Observações
- Arquivos de entrada: todos os arquivos (`.asm`, `.pre`, `.obj`) devem estar na pasta `files/`.
- Arquivos de saída: Todos os aquivos de saída são gerados na mesma pasta `files/`.
