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



void traduz(uint8_t *array_bruto, int tam_array){
    CorpoMensagem corpo;
    memcpy ( &corpo, array_bruto, 3);
    // memcpy ( &corpo.paridade, array_bruto, 3);
    // memcpy ( &corpo.marcador, &array_bruto[0], 1);
    // memcpy ( &corpo.tipo, &array_bruto[1], 1);
    // memcpy ( &corpo.paridade, &array_bruto[tam_array], 1);
    std::cout << "corpo: " << std::bitset<32>(corpo.binario) << "\n";
    std::cout << "marcador: " << std::bitset<8>(corpo.marcador) << "\n";
    std::cout << "destino: " << std::bitset<2>(corpo.destino) << "\n";
    std::cout << "origem: " << std::bitset<2>(corpo.origem) << "\n";
    std::cout << "tamanho: " << unsigned(corpo.tamanho) << "\n";
    std::cout << "sequencia: " << std::bitset<4>(corpo.sequencia) << "\n";
    std::cout << "tipo: " << std::bitset<4>(corpo.tipo) << "\n";
    std::cout << "paridade: " << std::bitset<8>(corpo.paridade) << "\n";
    
    std::cout << "Dados: \n";
    for (int i = 0; i < unsigned(corpo.tamanho); i++){        
        std::cout << "\t" << unsigned(array_bruto[3+i]) << "\n";
    }
    
    std::cout << "Mensagem recebida: \n";
    for (int i = 0; i < tam_array; i++){        
        std::cout << "\t" << unsigned(array_bruto[i]) << "\n";
    }
    
}

int main(int argc, char *argv[]){
    /*
    // std::string str;
    // str = "lo";int
    CorpoMensagem msg;
 
    char tipo_conexao[3];
    int tam_envio = 19;
    uint8_t dadobruto[tam_envio];
    int tam_recibo = 19;
    uint8_t dado_recebido[tam_recibo];

    dadobruto[0] = 0b00111110;
    dadobruto[1] = 0b01101110;
    dadobruto[2] = 0b00100100;
    for (int i = 3; i < tam_envio; i++){
        dadobruto[i] = i;
    }


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
    list();
    // std::cout << << std::endl;
    return 0;
}