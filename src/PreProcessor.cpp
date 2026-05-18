#include "../include/PreProcessor.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

std::string substituirEQU(const std::string linha, const std::vector<std::pair<std::string, std::string>> tabela) {
    std::string resultado;
    std::string tokenAtual;
    std::string valor;
    
    for (int i = 0; i < (int)linha.size(); ++i) {
        if (linha[i] == ' ' || linha[i] == ',') {
            if (!tokenAtual.empty()) {
                valor.clear();
                
                for (int j = 0; j < (int)tabela.size(); ++j) {
                    if (tokenAtual == tabela[j].first) {
                        valor = tabela[j].second;
                        break;
                    }
                }
                if (!valor.empty()){
                    resultado = resultado + valor;
                }
                else {
                    resultado = resultado + tokenAtual;
                }
                
                tokenAtual.clear();
            }

            resultado = resultado + linha[i];
        }
        else {
            tokenAtual = tokenAtual + linha[i];
        }
    }
    
    if (!tokenAtual.empty()) {
        valor.clear();
        
        for (int j = 0; j < (int)tabela.size(); ++j) {
            if (tokenAtual == tabela[j].first) {
                valor = tabela[j].second;
                break;
            }
        }
        if (!valor.empty()){
            resultado = resultado + valor;
        }
        else{
            resultado = resultado + tokenAtual; 
        }
    }
    
    return resultado;
}

std::string removerComentario(const std::string linha) {
    if (linha.find(';') == std::string::npos) {
        return linha; 
    }
    
    return linha.substr(0, linha.find(';'));
}

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
    std::vector<std::pair<std::string, std::string>> tabelaDiretiva;
    std::vector<std::string> vetorFinal;
    bool texto = false;
    bool dados = false;
    bool ordenado = false; 
    bool zero = true;
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

    // Se COPY, junta os dois operandos separados por vírgula sem espaço
    for (int i = 0; i < (int)novoVetor.size(); i++) {
        if (novoVetor[i].find("COPY") == 0) {
            std::string operandos = novoVetor[i].substr(5);
            std::string operandosNormalizado;
            
            for (int j = 0; j < (int)operandos.size(); ++j) {
                char caracter = operandos[j];
                
                if (caracter != ' ') {
                    operandosNormalizado = operandosNormalizado + caracter;
                }
            }
            
            novoVetor[i] = "COPY " + operandosNormalizado;
        }
    }

    // Se EQU, adiciona na tabela de diretiva
    int i = 0;
    int total = (int)novoVetor.size();
    for (i = 0; i < total; ++i) {
        std::string linha = removerComentario(vetorLinha[i]);
        
        if (linha.empty()) {
            continue;
        }

        if (linha.find(" EQU ") == std::string::npos) {
            break;
        }
        
        std::string rotulo = linha.substr(0, novoVetor[i].find(" EQU "));
        std::string valor = linha.substr(novoVetor[i].find(" EQU ") + 5);

        
        if (!rotulo.empty() && rotulo.back() == ':') {
            rotulo.pop_back();
        }

        tabelaDiretiva.insert(tabelaDiretiva.end(), {rotulo, valor});
    }

    // Copia as linhas restantes que não tem EQU
    vetorLinha.clear();
    for (int j = i; j < total; ++j) {
        std::string linhaNormalizada = removerComentario(novoVetor[j]);
        
        if (!linhaNormalizada.empty()){
            vetorLinha.insert(vetorLinha.end(), linhaNormalizada);
        }
    }

    // Substitui todas as ocorrencias do EQU
    for (int i = 0; i < (int)vetorLinha.size(); ++i) {
        vetorLinha[i] = substituirEQU(vetorLinha[i], tabelaDiretiva);
    }

    // Processa o IF
    for (int i = 0; i < (int)vetorLinha.size(); ++i) {
        if (vetorLinha[i].size() >= 3 && vetorLinha[i].substr(0, 3) == "IF ") {
            std::string condicao = vetorLinha[i].substr(3);
            
            for (char caractere : condicao) {
                if (caractere != '0' && caractere != 'X') {
                    zero = false;
                    break;
                }
            }
            
            if (!zero && i + 1 < (int)vetorLinha.size()) {
                vetorFinal.insert(vetorFinal.end(), vetorLinha[i+1]);
            }

            i = i + 1;
            continue;
        }
        
        vetorFinal.insert(vetorFinal.end(), vetorLinha[i]);
    }
   
    // Associa um objeto ao arquivo.pre
    std::string arquivo = caminhoArquivo;
    size_t nomeArquivo = caminhoArquivo.rfind('.');
    arquivo = arquivo.substr(0, nomeArquivo) + ".pre";
    
    std::ofstream arquivo_pre(arquivo);
    
    // Escreve o arquivo.pre
    for (int i = 0; i < (int)vetorFinal.size(); ++i) {
        arquivo_pre << vetorFinal[i] << '\n';
    }
}