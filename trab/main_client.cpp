#include <iostream>
#include <string>
#include <sys/socket.h>
#include <bitset>
#include <string.h>
#include <chrono>
#include <thread>
#include "mensagem.h"
#include "list.h"
#include "directory.h"
#include "ver.h"
#include "edit.h"
#include "linha.h"
#include "linhas.h"
// #include "compilar.h"

extern "C" {
  #include "rawsockets.h" 
}

int main(){
    char tipo_conexao[] = "lo";
    int soquete = ConexaoRawSocket(tipo_conexao);
    uint8_t sequencia = 0;
    std::cout << "\033[2J\033[1;1H";
    std::cout << ">";
    while(true){
        std::string entrada;
        // realizar a leitura da entrada de toda a linha em vez de string por string
        std::getline(std::cin, entrada);
        std::cout << "\033[2J\033[1;1H";
        std::string comando = entrada.substr(0, entrada.find(" ")); // separa o comando
        int resposta;
        if (comando == "cd"){
            std::string nome_dir = entrada.substr(3, entrada.length());
            resposta = pedidoCd(&sequencia, soquete, nome_dir);
            if (resposta == -1)
                std::cout << "Ocorreu um erro de comunicação\n";                    
            
        }
        else if (comando == "lcd"){
            std::string nome_dir = entrada.substr(4, entrada.length());
            resposta = trocaDir(nome_dir);
            if (resposta == -1)
                std::cout << "Ocorreu um erro no acesso do diretório\n";     
            if (resposta == 0)
                std::cout << "O diretório informado não existe\n";                    
            
        }
        else if (comando == "ls"){
            resposta = pedidoLs(&sequencia, soquete);
            if (resposta == -1)
                std::cout << "Ocorreu um erro de comunicação\n";                    
            
        }
        else if (comando == "lls"){
            std::cout << ls();
            if (resposta == -1)
                std::cout << "Ocorreu um erro de comunicação\n";                    
            
        }
        else if (comando == "ver"){
            std::string nome_arq = entrada.substr(4, entrada.length());
            resposta = pedidoVer(&sequencia, soquete, nome_arq);
            if (resposta == -1)
                std::cout << "Ocorreu um erro de comunicação\n";                    
            
        }
        else if (comando == "linha"){
            try{
                std::string resto_entrada = entrada.substr(6, entrada.length());
                uint8_t linha = std::stoi( resto_entrada.substr(0, resto_entrada.find(" ")));

                resto_entrada = resto_entrada.substr(resto_entrada.find(" ") + 1, resto_entrada.length());
                std::string nome_arq = resto_entrada.substr(0, resto_entrada.find(" "));

                resposta = pedidoLinha(&sequencia, soquete, linha, nome_arq);
                if (resposta == -1)
                    std::cout << "Ocorreu um erro de comunicação\n";                    
            
            }
            catch(...){
                std::cout << "Erro na formatação do comando";
            }
        }
        else if (comando == "linhas"){
            try{
                std::string resto_entrada = entrada.substr(7, entrada.length());
                uint8_t linha_inicial = std::stoi( resto_entrada.substr(0, resto_entrada.find(" ")));
                
                resto_entrada = resto_entrada.substr(resto_entrada.find(" ") + 1, resto_entrada.length());
                uint8_t linha_final = std::stoi( resto_entrada.substr(0, resto_entrada.find(" ")));

                resto_entrada = resto_entrada.substr(resto_entrada.find(" ") + 1, resto_entrada.length());
                std::string nome_arq = resto_entrada.substr(0, resto_entrada.find(" "));
                resposta = pedidoLinhas(&sequencia, soquete, linha_inicial, linha_final, nome_arq);
                if (resposta == -1)
                    std::cout << "Ocorreu um erro de comunicação\n";                    
            
            }
            catch(...){
                std::cout << "Erro na formatação do comando";
            }
        }
        else if (comando == "edit"){
            try {
                std::string resto_entrada = entrada.substr(5, entrada.length());
                uint8_t linha = std::stoi( resto_entrada.substr(0, resto_entrada.find(" ")));

                resto_entrada = resto_entrada.substr(resto_entrada.find(" ") + 1, resto_entrada.length());
                std::string nome_arq = resto_entrada.substr(0, resto_entrada.find(" "));

                resto_entrada = resto_entrada.substr(resto_entrada.find("\"") + 1, resto_entrada.length());
                std::string novo_texto = resto_entrada.substr(0, resto_entrada.find("\""));

                resposta = pedidoEdit(&sequencia, soquete, linha, nome_arq, novo_texto);
                if (resposta == -1)
                    std::cout << "Ocorreu um erro de comunicação\n";                    
            
            }
            catch(...){
                std::cout << "Erro na formatação do comando";
            }
        }
        else if (comando == "compilar" || comando == "Compilar"){
            // compila(entrada.substr(9, entrada.length()));
        }
        else if (comando == "sair" || comando == "exit"){
            return 0;
        }
        else { // nenhum comando conhecido
            std::cout << "Comando não reconhecido\n";
        }
        std::cout << "\n>";
    }
}
