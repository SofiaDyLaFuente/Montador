#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string>
#include <vector>

class Simulator {
public:
    Simulator();
    void run(const std::string& objFilename);

private:
    int accumulator;
    int programCounter;
    std::vector<int> memory;

    // Função auxiliar para jogar o .obj para dentro do vetor de memória
    void loadMemory(const std::string& objFileName);
};


#endif