#include "../include/Simulator.h"
#include <iostream>
#include <fstream>

Simulator::Simulator() {
    accumulator = 0;
    programCounter = 0;
}

void Simulator::loadMemory(const std::string& objFilename) {
    std::ifstream file(objFilename);
    int value;

    while (file >> value) {
        memory.push_back(value);
    }
    file.close();
}

void Simulator::run(const std::string& objFilename) {
    loadMemory(objFilename);

    if (memory.empty()) {
        std::cerr << "Erro: Arquivo objeto vazio ou inexistente. \n";
        return;
    }

    std::cout << "\n======================================================\n";
    std::cout << "            INICIANDO EXECUÇÃO DO PROGRAMA            \n";
    std::cout << "======================================================\n";

    // Loop de Execução (Ciclo de Busca)
    while (programCounter < memory.size()) {
        int opcode = memory[programCounter];

        if (opcode == 1) {  // ADD
            accumulator += memory[memory[programCounter + 1]];
            programCounter += 2;
        }
        else if (opcode == 2) { // SUB
            accumulator -= memory[memory[programCounter + 1]];
            programCounter += 2;
        }
        else if (opcode == 3) { // MUL
            accumulator *= memory[memory[programCounter + 1]];
            programCounter += 2;
        }
        else if (opcode == 4) { // DIV
            int divisor = memory[memory[programCounter + 1]];
            if (divisor == 0) {
                std::cerr << "\n[ERRO FATAL] Divisao por zero abortada!\n";
                break;
            }
            accumulator /= divisor;
            programCounter += 2;
        }
        else if (opcode == 5) { // JMP
            programCounter = memory[programCounter + 1];
        }
        else if (opcode == 6) { // JMPN
            if (accumulator < 0) programCounter = memory[programCounter + 1];
            else programCounter += 2;
        }
        else if (opcode == 7) { // JMPP
            if (accumulator > 0) programCounter = memory[programCounter + 1];
            else programCounter += 2;
        }
        else if (opcode == 8) { // JMPZ
            if (accumulator == 0) programCounter = memory[programCounter + 1];
            else programCounter += 2;
        }
        else if (opcode == 9) { // COPY 
            int op1 = memory[programCounter + 1];
            int op2 = memory[programCounter + 2];
            memory[op2] = memory[op1];
            programCounter += 3;
        }
        else if (opcode == 10) { // LOAD
            accumulator = memory[memory[programCounter + 1]];
            programCounter += 2;
        }
        else if (opcode == 11) { // STORE
            memory[memory[programCounter + 1]] = accumulator;
            programCounter += 2;
        }
        else if (opcode == 12) { // INPUT
            int inputVal;
            std::cout << ">> (INPUT) Digite um valor: ";
            std::cin >> inputVal;
            memory[memory[programCounter + 1]] = inputVal;
            programCounter += 2;
        }
        else if (opcode == 13) { // OUTPUT
            std::cout << "<< (OUTPUT) Valor: " << memory[memory[programCounter + 1]] << "\n";
            programCounter += 2;
        }
        else if (opcode == 14) { // STOP
            std::cout << "\n[STOP] Execução finalizada com sucesso.\n";
            break;
        }
        else {
            std::cerr << "\n[ERRO] Opcode inválido (" << opcode << ") no PC " << programCounter << "\n";
            break;
        }
    } 
    std::cout << "======================================================\n\n";
}