# Montador Assembler
Repositório para a implementação do Trabalho 1 da disciplina de Software Básico. O projeto consiste na implementação, em C/C++, de um montador para a linguagem assembly fictícia apresentada em aula.


## Pré-processamento

Para compilar o pré-processador, utilize o seguinte comando no terminal (dentro da pasta raiz do projeto):

```bash
g++ -std=c++11 -Iinclude src/PreProcessor.cpp src/main.cpp -o montador
```

Para executar o pré-processador sobre um arquivo fonte (.asm), use:

```bash
./montador nomedoarquivo.asm
```

Observações:
- O arquivo de entrada deve estar na pasta `files/` (ex.: `files/nomedoarquivo.asm`).
- O pré-processador gera um arquivo de saída com o mesmo nome e extensão `.pre` na mesma pasta `files/`.