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

    bool running = true;

    // Loop de Execução (Ciclo de Busca)
    while (running && programCounter < memory.size()) {
        int opcode = memory[programCounter];

        switch (opcode) {
            case 1: // ADD
                accumulator += memory[memory[programCounter + 1]];
                programCounter += 2;
                break;
                
            case 2: // SUB
                accumulator -= memory[memory[programCounter + 1]];
                programCounter += 2;
                break;
                
            case 3: // MUL
                accumulator *= memory[memory[programCounter + 1]];
                programCounter += 2;
                break;
                
            case 4: // DIV
            {   
                int divisor = memory[memory[programCounter + 1]];
                if (divisor == 0) {
                    std::cerr << "\n[ERRO FATAL] Divisão por zero abortada!\n";
                    running = false; 
                } else {
                    accumulator /= divisor;
                    programCounter += 2;
                }
                break;
            }
                
            case 5: // JMP
                programCounter = memory[programCounter + 1];
                break;
                
            case 6: // JMPN
                if (accumulator < 0) programCounter = memory[programCounter + 1];
                else programCounter += 2;
                break;
                
            case 7: // JMPP
                if (accumulator > 0) programCounter = memory[programCounter + 1];
                else programCounter += 2;
                break;
                
            case 8: // JMPZ
                if (accumulator == 0) programCounter = memory[programCounter + 1];
                else programCounter += 2;
                break;
                
            case 9: // COPY 
                // A instrução COPY é a única de tamanho 3, logo avança PC em 3
                memory[memory[programCounter + 2]] = memory[memory[programCounter + 1]];
                programCounter += 3;
                break;
                
            case 10: // LOAD
                accumulator = memory[memory[programCounter + 1]];
                programCounter += 2;
                break;
                
            case 11: // STORE
                memory[memory[programCounter + 1]] = accumulator;
                programCounter += 2;
                break;
                
            case 12: // INPUT
            {
                int inputVal;
                std::cout << ">> (INPUT) Digite um valor: ";
                std::cin >> inputVal;
                memory[memory[programCounter + 1]] = inputVal;
                programCounter += 2;
                break;
            }
                
            case 13: // OUTPUT
                std::cout << "<< (OUTPUT) Valor: " << memory[memory[programCounter + 1]] << "\n";
                programCounter += 2;
                break;
                
            case 14: // STOP
                std::cout << "\n[STOP] Execução finalizada com sucesso.\n";
                running = false; 
                break;
                
            default: // Opcode Inválido
                std::cerr << "\n[ERRO] Opcode inválido (" << opcode << ") no PC " << programCounter << "\n";
                running = false; 
                break;
        }
    } 
    std::cout << "======================================================\n\n";
}