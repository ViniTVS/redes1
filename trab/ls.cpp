#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <sys/param.h>
#include <filesystem>
#include <string>
#include "ls.h"

namespace fs = std::filesystem;

std::string getPath(){
    char temp[MAXPATHLEN];
    return ( getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("") );
}

int enviaLs(uint8_t sequencia){
    uint8_t mensagem[20];
    std::string itens = list();
    // std::cout << itens;

    mensagem[0] = 0b01111110; // marcador inicio
    mensagem[1] = 0b10010000; // destino 10(servidor), origem 01(cliente), tam 0 
    mensagem[2] = 0b00000001 | (sequencia << 4);

    for(int i = 0; i < itens.length()/15; i++){
        for(int j = 0; j < 15; j++){
            mensagem[3+j] = itens[j + i*15];
            // std::cout << itens[j + i*15] << "\t" << j + i*15;
        }
    for(int i = 3; i < 18; i++){
        std::cout << mensagem[i];
    }
        std::cout << std::endl;
    }

    return 0;
}


std::string list() {
    std::string path = getPath();
    std::string out = "";
    for (const auto & entry : fs::directory_iterator(path)){
        out = out + entry.path().filename().string() + "\n";
    }
    return out;
}

void criaPedidoLs(uint8_t sequencia, uint8_t *mensagem){
    mensagem[0] = 0b01111110; // marcador inicio
    mensagem[1] = 0b10010000; // destino 10(servidor), origem 01(cliente), tam 0 
    mensagem[2] = 0b00000001 | (sequencia << 4);
    for(int i = 3; i < 20; i++){
        mensagem[i] = 0b00000000;
    }

    // return mensagem;
}
