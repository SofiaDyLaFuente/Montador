/*
Trabalho prático 1 - Montador Assembler

Alunos: João Vitor Pereira Vieira,
        Sofia Dy La Fuente Monteiro.
*/

#include <iostream>
#include <fstream>
#include <string>

void pre_processamento() {
    //continuar implementação
}

void montador() {
    //continuar implementação
}

void simulador() {
    //continuar implementação
}

int main(int argc, char* argv[]) {
    std::string nomeArquivo = argv[1];

    size_t finalArquivo = nomeArquivo.rfind('.');
    std::string extensao = nomeArquivo.substr(finalArquivo);

    if (extensao == ".asm") {
        pre_processamento();
    }
    else if (extensao == ".pre") {
        montador();
    }
    else if (extensao == ".obj") {
        simulador();
    }
    else {
        return 1;
    }

    return 0;
}