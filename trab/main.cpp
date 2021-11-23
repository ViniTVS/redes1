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

int clientMain(int soquete){
    uint8_t sequencia = 255;
    Mensagem teste(0, 0b11, 0b10, 0b0111, sequencia, 0b01111010, NULL);
    teste.printMensagem();
    
    if (teste.enviaMensagem(soquete) != 0)
        std::cout << "enviado \n";
    return 0; 
    sequencia = criaPedidoLs(sequencia, soquete);
    uint8_t mensagemBruta[20];
    CorpoMensagem msg;
    sequencia++;

    return 0;
}

int serverMain(int soquete){
    uint8_t dado_recebido[20];
    int nread;
    
    while(true){

        nread = recv(soquete, &dado_recebido, 20, 0);
        // std::cout << nread << "\n";
        if (nread == -1)
            return(-1); 
        else{
            Mensagem mensagemRecebida(dado_recebido);
            uint8_t sequencia = mensagemRecebida.getSequencia();

            switch(mensagemRecebida.getTipo()) {
                case 0x00: // cd
                //     // code block
                    break;
                case 0x01: // ls
                    enviaLs(sequencia, soquete);

                //     // code block
                    break;
                case 0x02:
                //     // code block
                    break;
                case 0x03:
                //     // code block
                    break;
                case 0x04:
                //     // code block
                    break;
                case 0x05:
                //     // code block
                    break;
                // case 12:
                //     // code block
                //     break;
                default:
                    // std::cout << std::bitset<8>(mensagemRecebida.getTipo());
                    std::cout << " Mensagem recebida: \n";
                    mensagemRecebida.printMensagem();
                    // code block
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[]){

    char tipo_conexao[] = "lo";
    int soquete = ConexaoRawSocket(tipo_conexao);

    if (argc > 1)
        if( strcmp(argv[1], "-s") == 0 )
            return serverMain(soquete);

    return clientMain(soquete);
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

    // std::cout << "Mensagem: \n";
    // for (int i = 0; i < 20; i++){
    //     std::cout << std::bitset<8>(mensagemBruta[i]);
    // }
    // std::cout << std::endl;
    

    // Mensagem teste(mensagemBruta);

    // teste.printMensagem();
    // Mensagem

    // std::cout << list();
}