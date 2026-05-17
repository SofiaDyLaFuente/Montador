#include "../include/Assembler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

Assembler::Assembler() {
    locationCounter = 0;
    initializeInstructionTable();
}

// OPCODE E TAMANHO (PALAVRAS) de cada instrução
void Assembler::initializeInstructionTable() {
    instructionTable["ADD"]    = {1, 2};
    instructionTable["SUB"]    = {2, 2};
    instructionTable["MULT"]   = {3, 2};
    instructionTable["DIV"]    = {4, 2};
    instructionTable["JMP"]    = {5, 2};
    instructionTable["JMPN"]   = {6, 2};
    instructionTable["JMPP"]   = {7, 2};
    instructionTable["JMPZ"]   = {8, 2};
    instructionTable["COPY"]   = {9, 3};
    instructionTable["LOAD"]   = {10, 2};
    instructionTable["STORE"]  = {11, 2};
    instructionTable["INPUT"]  = {12, 2};
    instructionTable["OUTPUT"] = {13, 2};
    instructionTable["STOP"]   = {14, 1};
}

// Divide a linha por espaços para extrair os tokens
std::vector<std::string> Assembler::splitLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(line);
    while (tokenStream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// ALGORITMO DE PASSAGEM ÚNICA: Trata os operandos (variáveis/rótulos) gerando a lista de pendências
void Assembler::handleOperand(const std::string& operand) {

    // Se o operando for um literal (número direto), empacota no código objeto 
    // Ex: o argumento de um CONST ou cópia imediata
    if (isdigit(operand[0]) || (operand[0] == '-' && isdigit(operand[1]))) {
        int num = std::stoi(operand);
        machineCode.push_back(num);
        penCode.push_back(num);
        locationCounter++;
        return;
    }

    // Se for um símbolo (Rótulo de texto)
    if (symbolTable.find(operand) == symbolTable.end()) {
        
        // Caso 1: Símbolo nunca foi visto no código.
        // Registramos na tabela que ele não está definido e que sua primeira pendência é o LC atual.
        symbolTable[operand] = {0, false, locationCounter};

        // Inserimos -1 no código de máquina para demarcar o fim da lista encadeada de pendências.

        machineCode.push_back(-1);          
        penCode.push_back(-1);              
        
        std::cout << "  -> [PENDÊNCIA] '" << operand << "' inicializada com -1 no LC " << locationCounter << "\n";
    }
    else if (!symbolTable[operand].isDefined) {

        // Caso 2: Símbolo já foi usado antes, mas continua sem definição (Referência para frente)
        // Pegamos o índice da pendência anterior para criar o nó da lista encadeada.
        int previousPendency = symbolTable[operand].firstPendency;

        // Atualizamos a cabeça da lista na Tabela de Símbolos para apontar para este LC.
        symbolTable[operand].firstPendency = locationCounter;

        // Gravamos no próprio código objeto o ponteiro que aponta para a pendência anterior.
        machineCode.push_back(previousPendency);
        penCode.push_back(previousPendency);

        std::cout << "  -> [PENDÊNCIA] '" << operand << "' encadeada. Aponta para o LC anterior " << previousPendency << "\n";
    }
    else {

        // Caso 3: Símbolo já foi definido previamente no código (Referência para trás)
        // Não há pendência. Inserimos o endereço de memória real dele diretamente.
        int realAddress = symbolTable[operand].address;
        machineCode.push_back(realAddress);
        penCode.push_back(realAddress);

        std::cout << "  -> [OK] '" << operand << "' já era conhecido. Inserindo endereço real " << realAddress << "\n";
    }

    // Toda palavra inserida no código de máquina avança o Location Counter
    locationCounter++;
}

// Função feita com o intuito de GERAR PRINTS no terminal de forma organizada
void printTablesToTerminal(const std::map<std::string, Symbol>& symbols, const std::vector<int>& pen, const std::vector<int>& obj) {
    std::cout << "\n======================================================\n";
    std::cout << "                TABELA DE SÍMBOLOS FINAL              \n";
    std::cout << "======================================================\n";
    std::cout << std::left << std::setw(18) << "Símbolo" 
              << "| " << std::setw(10) << "Endereço" 
              << "| " << std::setw(10) << "Definido?" << "\n";
    std::cout << "------------------------------------------------------\n";
    for (const auto& pair : symbols) {
        std::cout << std::left << std::setw(18) << pair.first
                  << "| " << std::setw(10) << pair.second.address
                  << "| " << std::setw(10) << (pair.second.isDefined ? "SIM" : "NÃO") << "\n";
    }
    
    std::cout << "\n======================================================\n";
    std::cout << "       ARQUIVO INTERMEDIÁRIO .PEN (COM PENDÊNCIAS)    \n";
    std::cout << "======================================================\n";
    for (size_t i = 0; i < pen.size(); ++i) {
        std::cout << pen[i] << (i + 1 == pen.size() ? "" : " ");
    }
    
    std::cout << "\n\n======================================================\n";
    std::cout << "          ARQUIVO OBJETO FINAL .OBJ (RESOLVIDO)       \n";
    std::cout << "======================================================\n";
    for (size_t i = 0; i < obj.size(); ++i) {
        std::cout << obj[i] << (i + 1 == obj.size() ? "" : " ");
    }
    std::cout << "\n======================================================\n\n";
}

// Função principal do Montador
void Assembler::generate(const std::string& preFilename) {

    std::string baseName = preFilename.substr(0, preFilename.find_last_not_of('.'));
    std::string objFilename = baseName + ".obj";
    std::string penFilename = baseName + ".pen";

    std::ifstream inputFile(preFilename);
    std::string line;

    // Limpeza dos vetores para garantir execuções limpas
    machineCode.clear();
    penCode.clear();
    symbolTable.clear();
    locationCounter = 0;

    std::cout << "\nINICIANDO MONTAGEM (ALGORITMO DE PASSAGEM ÚNICA)\n";

    while (std::getline(inputFile, line)) {                                                 // Leitura do Arquivo .pre linha por linha

        // Ignora os cabeçalhos de seção e linhas vazias
        if (line.empty() || line == "SECTION TEXT" || line == "SECTION DATA") continue;

        std::vector<std::string> tokens = splitLine(line);
        if (tokens.empty()) continue;
        std::cout << "\n[LINHA]: " << line << "\n";

        int idx = 0;
        std::string current = tokens[idx];

        // ETAPA A: Processamento de Rótulo de Definição (Ex: "L1:" ou "DOIS:")
        if (current.back() == ':') {
            std::string label = current.substr(0, current.size() - 1);

            if (symbolTable.find(label) == symbolTable.end()) {
                // Se o rótulo é novo, define ele apontando para o LC atual
                symbolTable[label] = {locationCounter, true, -1};
                std::cout << "  -> [RÓTULO] '" << label << "' mapeado direto para o LC " << locationCounter << ".\n";
            }
            else if (!symbolTable[label].isDefined) {
                // BACKPATCHING - Se o rótulo já tinha sido definido, resolvemos a lista de pendências
                std::cout << "  -> [BACKPATCHING] Resgatando a lista de pendências de '" << label << "' no LC " << locationCounter << "\n";
                int pendencyIndex = symbolTable[label].firstPendency;

                // Percorre a lista encadeada gravada dentro do próprio vetor machineCode
                while (pendencyIndex != -1) {                                               
                    int nextPendency = machineCode[pendencyIndex];                          // Salva o ponteiro para o prox. nó
                    machineCode[pendencyIndex] = locationCounter;                           // Substitui o valor pelo endereço real atual
                    pendencyIndex = nextPendency;                                           // Caminha para a próxima pendência
                }

                // Atualiza o estado do símbolo para definido
                symbolTable[label].isDefined = true;
                symbolTable[label].address = locationCounter;
            }

            // Avança para o próximo token da linha após processar o rótulo
            idx++;
            if (idx >= tokens.size()) continue;                                             // Se a linha era só o rótulo sozinho, pula pra próxima
            current = tokens[idx];
        }

        // ETAPA B: Processamento de Instruções Reais do Sistema
        if (instructionTable.find(current) != instructionTable.end()) {
            Instruction inst = instructionTable[current];
            machineCode.push_back(inst.opcode);                                             // Insere o OPCODE númerico no binário
            penCode.push_back(inst.opcode);
            locationCounter++;

            idx++;
            if (idx < tokens.size()) {
                std::string operandsStr = tokens[idx];

                // Tratamento específico pra COPY, que possui 2 operandos separados por vírgula
                size_t commaPos = operandsStr.find(',');
                if (commaPos != std::string::npos) {
                    std::string op1 = operandsStr.substr(0, commaPos);
                    std::string op2 = operandsStr.substr(commaPos + 1);
                    handleOperand(op1);
                    handleOperand(op2);
                } else {
                    // Instrução de operando único (Ex: LOAD OLD_DATA)
                    handleOperand(operandsStr);
                }
            }
        }

        // ETAPA C: Processamento de Diretivas de Memória de Dados
        else if (current == "CONST") {
            idx++;                                                                          // Avança para pegar o valor númerico a frente do CONST
            int constValue = std::stoi(tokens[idx]);
            machineCode.push_back(constValue);
            penCode.push_back(constValue);
            locationCounter++;
        }
        else if (current == "SPACE") {
            int spacesToAllocate = 1;                                                       // Por padrão, aloca 1 espaço
            idx++;

            // Se houver um argumento númerico na diretiva Space (Ex: SPACE 3)
            if (idx < tokens.size()) {
                spacesToAllocate = std::stoi(tokens[idx]);
            }
            // Aloca N espaços na memória preenchidos com o valor zero
            for (int i = 0; i < spacesToAllocate; i++) {
                machineCode.push_back(0);
                penCode.push_back(0);
                locationCounter++;
            }
        }
    }
    inputFile.close();

    // Gravação do Arquivo Objeto (.obj) 
    std::ofstream objFile(objFilename);
    for (size_t i = 0; i < machineCode.size(); ++i) {
        objFile << machineCode[i] << (i + 1 == machineCode.size() ? "" : " ");
    }
    objFile.close();

    // Gravação do Arquivo de Mapeamento/Pendências (.pen)
    std::ofstream penFile(penFilename);
    for (size_t i = 0; i < penCode.size(); ++i) {
        penFile << penCode[i] << (i + 1 == penCode.size() ? "" : " ");
    }
    penFile.close();

    // Printa os resultados no terminal
    printTablesToTerminal(symbolTable, penCode, machineCode);
}


