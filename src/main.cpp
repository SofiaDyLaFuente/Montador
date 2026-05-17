/*
Trabalho prático 1 - Montador Assembler

Alunos: João Vitor Pereira Vieira,
        Sofia Dy La Fuente Monteiro.
*/

#include <iostream>
#include <string>
#include "../include/PreProcessor.h"
#include "../include/Assembler.h" 
#include "../include/Simulator.h"

int main(int argc, char* argv[]) {
    // Evita crash (Segmentation Fault) se o usuário digitar só "./montador"
    if (argc < 2) {
        std::cerr << "Erro: Arquivo não especificado. Use: ./montador nome_do_arquivo.ext\n";
        return 1;
    }

    std::string nomeArquivo = argv[1];

    // Evita crash se o usuário digitar um arquivo sem extensão (ex: "./montador teste")
    size_t finalArquivo = nomeArquivo.rfind('.');
    if (finalArquivo == std::string::npos) {
        std::cerr << "Erro: O arquivo precisa ter uma extensao (.asm, .pre ou .obj).\n";
        return 1;
    }

    std::string extensao = nomeArquivo.substr(finalArquivo);
    std::string caminho = "files/" + nomeArquivo; 

    // Roteamento
    if (extensao == ".asm") {

        std::cout << "Redirecionando para o Pre-Processador...\n";
    }
    else if (extensao == ".pre") {
        Assembler gerador;  
        gerador.generate(caminho);
        std::cout << "Montagem finalizada com sucesso!\n";
    }
    else if (extensao == ".obj") {
        Simulator sim;
        sim.run(caminho);
        std::cout << "Redirecionando para o Simulador...\n";
    }
    else {
        std::cerr << "Erro: Extensao invalida (" << extensao << ").\n";
        return 1;
    }

    return 0;
}