#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <vector>
#include <map>

struct Symbol {
    int address;                                            // Endereço final do símbolo (LC) se definido
    bool isDefined;                                         // Indica se o símbolo já foi definido
    int firstPendency;                                      // Cabeça da lista de pendências (-1 se não houver)
};

struct Instruction {
    int opcode;                                             // Código da operação
    int size;                                               // Tamanho da instrução
};

class Assembler {
public:
    Assembler();
    void generate(const std::string& preFilename);

private:
    int locationCounter;
    std::map<std::string, Symbol> symbolTable;
    std::map<std::string, Instruction> instructionTable;
    std::vector<int> machineCode;                           // Memória com o código resolvido (.obj)
    std::vector<int> penCode;                               // Memória com as pendências originais (.pen)

    void initializeInstructionTable();
    std::vector<std::string> splitLine(const std::string& line);
    void handleOperand(const std::string& operand);
};

#endif