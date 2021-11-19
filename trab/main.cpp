#include <iostream>
#include <string>
#include <sys/socket.h>
#include <bitset>
#include <string.h>
#include <chrono>
#include <thread>
#include "mensagem.h"
#include "ls.h"

extern "C" {
  #include "rawsockets.h" //a C header, so wrap it in extern "C" 
}


int main(int argc, char *argv[]){
    /*
    // std::string str;
    // str = "lo";int
    CorpoMensagem msg;
 
    char tipo_conexao[3];


    std::cout << "Mensagem: \n";
    for (int i = 0; i < tam_envio; i++){        
        std::cout << "\t" << unsigned(dadobruto[i]) << "\n";
    }

    std::cout << "Iniciando socket...\n";
    strcpy(tipo_conexao, "lo");
    int soquete = ConexaoRawSocket(tipo_conexao);
    std::cout << "soquete: " << soquete << "\n";
    
    // if(argc == 1){
        std::cout << "sending...\n";
        int nread = send(soquete, &dadobruto, tam_envio, 0);
        std::cout << "bytes enviados: " << nread << std::endl;

    // } else {

        uint64_t recebe;
         nread = recv(soquete, &dado_recebido, tam_recibo, 0);
        if (nread == -1)
            exit(-1); 
        std::cout << "bytes recebidos: " << nread << "\n";

        traduz(dado_recebido, tam_recibo);
    // }

        // std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    // for (int i = 0; i < tam_recibo; i++){
    //     std::cout << "\t" << unsigned(dado_recebido[i]) << "\n";
    // }
    */
    // ;
    uint8_t mensagemBruta[20];
    uint8_t sequencia = 255;
    sequencia++;
    enviaLs(sequencia);

    // std::cout << "Mensagem: \n";
    // for (int i = 0; i < 20; i++){
    //     std::cout << std::bitset<8>(mensagemBruta[i]);
    // }
    // std::cout << std::endl;
    

    // Mensagem teste(mensagemBruta);

    // teste.printMensagem();
    // Mensagem

    // std::cout << list();
    return 0;
}