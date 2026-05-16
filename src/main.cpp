/*
Trabalho prático 1 - Montador Assembler

Alunos: João Vitor Pereira Vieira,
        Sofia Dy La Fuente Monteiro.
*/

#include <string>
#include "../include/PreProcessor.h"
#include "../include/Assembler.h"
#include "../include/Simulator.h"

int main(int argc, char* argv[]) {
    std::string nomeArquivo = argv[1];

    size_t finalArquivo = nomeArquivo.rfind('.');
    std::string extensao = nomeArquivo.substr(finalArquivo);
    std::string caminho = "files/" + nomeArquivo;

    if (extensao == ".asm") {
        preProcessor(caminho);
    }
    else if (extensao == ".pre") {
        //montador();
        return 0;
    }
    else if (extensao == ".obj") {
        //simulador();
        return 0;
    }
    else {
        return 0;
    }

    return 0;
}