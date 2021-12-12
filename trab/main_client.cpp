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
#include "compilar.h"

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

        if (comando == "cd"){
            std::string nome_dir = entrada.substr(3, entrada.length());
            pedidoCd(&sequencia, soquete, nome_dir);
        }
        else if (comando == "lcd"){
            std::string nome_dir = entrada.substr(4, entrada.length());
            trocaDir(nome_dir);
        }
        else if (comando == "ls"){
            int resposta = pedidoLs(&sequencia, soquete);
            if (resposta != 1){
                if (resposta == 0 )
                    std::cout << "Server timeout\n";
                else
                    std::cout << "Erro no soquete\n";                    
            }
        }
        else if (comando == "lls"){
            std::cout << ls();
        }
        else if (comando == "ver"){
            std::string nome_arq = entrada.substr(4, entrada.length());
            pedidoVer(&sequencia, soquete, nome_arq);
        }
        else if (comando == "linha"){
            std::string resto_entrada = entrada.substr(6, entrada.length());
            uint8_t linha = std::stoi( resto_entrada.substr(0, resto_entrada.find(" ")));

            resto_entrada = resto_entrada.substr(resto_entrada.find(" ") + 1, resto_entrada.length());
            std::string nome_arq = resto_entrada.substr(0, resto_entrada.find(" "));

            pedidoLinha(&sequencia, soquete, linha, nome_arq);
        }
        else if (comando == "linhas"){
            std::string resto_entrada = entrada.substr(7, entrada.length());
            uint8_t linha_inicial = std::stoi( resto_entrada.substr(0, resto_entrada.find(" ")));
            
            resto_entrada = resto_entrada.substr(resto_entrada.find(" ") + 1, resto_entrada.length());
            uint8_t linha_final = std::stoi( resto_entrada.substr(0, resto_entrada.find(" ")));

            resto_entrada = resto_entrada.substr(resto_entrada.find(" ") + 1, resto_entrada.length());
            std::string nome_arq = resto_entrada.substr(0, resto_entrada.find(" "));
            pedidoLinhas(&sequencia, soquete, linha_inicial, linha_final, nome_arq);

        }
        else if (comando == "edit"){
            std::string resto_entrada = entrada.substr(5, entrada.length());
            uint8_t linha = std::stoi( resto_entrada.substr(0, resto_entrada.find(" ")));

            resto_entrada = resto_entrada.substr(resto_entrada.find(" ") + 1, resto_entrada.length());
            std::string nome_arq = resto_entrada.substr(0, resto_entrada.find(" "));

            resto_entrada = resto_entrada.substr(resto_entrada.find("\"") + 1, resto_entrada.length());
            std::string novo_texto = resto_entrada.substr(0, resto_entrada.find("\""));

            pedidoEdit(&sequencia, soquete, linha, nome_arq, novo_texto);
        }
        else if (comando == "compilar" || comando == "Compilar"){
            compila(entrada.substr(9, entrada.length()));
        }
        else if (comando == "sair" || comando == "exit"){
            return 0;
        }
        else { // nenhum comando conhecido
            std::cout << "ERRO: Comando não reconhecido\n";
        }
        std::cout << "\n>";
    }
}
