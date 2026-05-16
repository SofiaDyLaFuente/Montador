#include "../include/PreProcessor.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

std::string normalizarLinha(const std::string linha) {
    std::string resultado;
    bool ultimoEspaco = true; 

    for (int i = 0; i < (int)linha.size(); ++i) {
        char caracter = linha[i];
        char letra = toupper(caracter); 

        if (letra == '\r') {
            continue;
        }
        if (letra == '\t') {
            letra = ' ';
        }
        if (letra == ';') {
            break;
        }
        if (letra == ' ') {
            if (!ultimoEspaco) {
                resultado = resultado + ' ';
                ultimoEspaco = true;
            }
        } 
        else {
            resultado = resultado + letra;
            ultimoEspaco = false;
        }
    }
    
    if (!resultado.empty() && resultado.back() == ' ')
        resultado.pop_back();

    return resultado;
}

void preProcessor(const std::string caminhoArquivo) {
    std::vector<std::string> vetorLinha;
    std::string linha;
    std::vector<std::string> sectionText;
    std::vector<std::string> sectionData;
    std::vector<std::string> novoVetor;
    bool texto = false;
    bool dados = false;
    bool ordenado = false; 
    int contador = 0;
    
    // Lê o arquivo.asm e para cada linha faz a normalização
    std::ifstream arquivo_asm(caminhoArquivo);

    while (std::getline(arquivo_asm, linha)) {
        std::string linhaNormalizada = normalizarLinha(linha);
        
        if (linhaNormalizada.empty()) {
            continue;
        }
        
        vetorLinha.insert(vetorLinha.end(), linhaNormalizada);
    }

    // Se Section Data vier antes de Section Text: inverte
    for (int i = 0; i < (int)vetorLinha.size(); ++ i) {
        if (contador == 0 && vetorLinha[i] == "SECTION TEXT"){
            ordenado = true;
            break;
        }
        
        else if (vetorLinha[i] == "SECTION TEXT") {
            texto = true;
            dados = false;
            sectionText.insert(sectionText.end(), vetorLinha[i]);
            continue;
        }
        else if (vetorLinha[i] == "SECTION DATA") {
            texto = false;
            dados = true;
            sectionData.insert(sectionData.end(), vetorLinha[i]);
            continue;
        }

        if (texto == true) {
            sectionText.insert(sectionText.end(), vetorLinha[i]);
        } 
        else if (dados == true) {
            sectionData.insert(sectionData.end(), vetorLinha[i]);
        }
        
        contador = contador + 1;
    }

    if (ordenado == false){
        vetorLinha.clear();
        vetorLinha.insert(vetorLinha.end(), sectionText.begin(), sectionText.end());
        vetorLinha.insert(vetorLinha.end(), sectionData.begin(), sectionData.end());
    }

    // Se a label for seguida de ENTER: junta na mesma linha
    for (int i = 0; i < (int)vetorLinha.size(); ++i) {
        if (vetorLinha[i].back() == ':') {
            novoVetor.insert(novoVetor.end(), vetorLinha[i] + " " + vetorLinha[i + 1]);
            i = i + 1;
        } 

        else {
            novoVetor.insert(novoVetor.end(), vetorLinha[i]);
        }
    }

    // Associa um objeto ao arquivo.pre
    std::string arquivo = caminhoArquivo;
    size_t nomeArquivo = caminhoArquivo.rfind('.');
    arquivo = arquivo.substr(0, nomeArquivo) + ".pre";
    
    std::ofstream arquivo_pre(arquivo);
    
    // Escreve o arquivo.pre
    for (int i = 0; i < (int)novoVetor.size(); ++i) {
        arquivo_pre << novoVetor[i] << '\n';
    }
}