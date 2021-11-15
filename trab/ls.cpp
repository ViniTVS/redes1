#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <sys/param.h>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

std::string get_working_path(){
    char temp[MAXPATHLEN];
    return (    getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("") );
}

std::string list() {
    std::string path = get_working_path();
    std::string out = "";
    for (const auto & entry : fs::directory_iterator(path)){
        out = out + entry.path().filename().string() + "\n";
    }
    return out;
}

void criaMensagemLs(uint8_t sequencia, uint8_t *mensagem){
    mensagem[0] = 0b01111110; // marcador inicio
    mensagem[1] = 0b10010000; // destino 10(servidor), origem 01(cliente), tam 0 
    mensagem[2] = 0b00000000 | (sequencia << 4);
    for(int i = 3; i < 20; i++){
        mensagem[i] = 0b00000000;
    }

    // return mensagem;
}
