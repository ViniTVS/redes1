#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <linux/if.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <stdio.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <filesystem>
#include <string>
#include "ls.h"
#include "mensagem.h"

namespace fs = std::filesystem;

std::string getPath(){
    char temp[MAXPATHLEN];
    return ( getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("") );
}

int enviaLs(uint8_t sequencia, int soquete){
    uint8_t mensagemBruta[20];
    uint8_t dado_recebido[20];
    std::string itens = list();
    // std::cout << itens;
    int len = 0;

    // envia as mensagens de 15 chars
    for(int i = 0; i < itens.length()/15; i++){

        mensagemBruta[0] = 0b01111110; // marcador inicio
        mensagemBruta[1] = 0b01101111; // destino 01(servidor), origem 10(cliente), tam 15 
        mensagemBruta[2] = 0b00001011 | (sequencia << 4);        
        
        for(int j = 0; j < 15; j++){
            mensagemBruta[3+j] = itens[len];
            len++;
        }
        
        send(soquete, &mensagemBruta, 20, 0);
        recv(soquete, &dado_recebido, 20, 0);
        Mensagem aux(dado_recebido);
        aux.printMensagemString();
        sequencia++;
    }
    // mensagens com menos de 15 chars
    if (len < itens.length()){

        mensagemBruta[0] = 0b01111110; // marcador inicio
        mensagemBruta[1] = 0b01100000 | (itens.length() - len); // destino 01(servidor), origem 10(cliente), tam  
        mensagemBruta[2] = 0b00001011 | (sequencia << 4);        
        int i = 3;
        for(; itens.length() - len > 0; i++){
            mensagemBruta[i] = itens[len];
            len++;
        }
        // mensagemBruta[i] = 0b10101010;
        send(soquete, &mensagemBruta, 20, 0);
        recv(soquete, &dado_recebido, 20, 0);
        Mensagem aux(dado_recebido);
        aux.printMensagemString();
        sequencia++;
    }

    return sequencia;
}


std::string list() {
    std::string path = getPath();
    std::string out = "";
    for (const auto & entry : fs::directory_iterator(path)){
        out = out + entry.path().filename().string() + "\n";
    }
    return out;
}

uint8_t criaPedidoLs(uint8_t sequencia, int soquete){
    uint8_t mensagemBruta[20];
    mensagemBruta[0] = 0b01111110; // marcador inicio
    mensagemBruta[1] = 0b10010000; // destino 10(servidor), origem 01(cliente), tam 0 
    mensagemBruta[2] = 0b00000001 | (sequencia << 4);
    mensagemBruta[3] = 0b00000000;
    int enviado = send(soquete, &mensagemBruta, 20, 0);
    if (enviado <= 0)
        exit(-1);

    return sequencia + 1;
}
