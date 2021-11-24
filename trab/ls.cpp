#include <iostream>
#include <string>
#include <filesystem>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <linux/if.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <net/ethernet.h>
#include "ls.h"
#include "mensagem.h"

namespace fs = std::filesystem;

int trocaDir(std::string nome_dir){
    std::string novo_dir = "";
    // verifica se a entrada é um possível caminho diretório
    if(nome_dir[0] == '/')
        novo_dir = nome_dir;
    else if(nome_dir == "..") // se quer voltar 1 dir
        novo_dir = fs::current_path().parent_path().string(); 
    else // ou cd em um dir do local atual
        novo_dir = fs::current_path().string() + "/" + nome_dir ; 
    
    if(fs::is_directory(novo_dir))
        fs::current_path(novo_dir);
        
    return 0;
}

int enviaRespostaLs(uint8_t sequencia, int soquete){
    uint8_t array_dados[15];
    uint8_t dado_recebido[20];
    std::string itens = list();
    // std::cout << itens;
    int len = 0;

    // envia as mensagens de 15 chars
    for(int i = 0; i < itens.length()/15; i++){
        
        for(int j = 0; j < 15; j++){
            array_dados[j] = itens[len];
            len++;
        }
        Mensagem mensagem(15, 0b10, 0b01, 0b1011, sequencia++, array_dados);
        if (mensagem.enviaMensagem(soquete) <= 0)
            return -1;       
        std::cout << "recebi uma resposta:\n"; 
        Mensagem resposta = mensagem.recebeResposta(soquete);
        resposta.printMensagemString();
        return 0;
        // Mensagem aux(dado_recebido);
        // aux.printMensagemString();
        sequencia++;
    }
    // mensagens com menos de 15 chars
    if (len < itens.length()){

        // mensagemBruta[0] = 0b01111110; // marcador inicio
        // mensagemBruta[1] = 0b01100000 | (itens.length() - len); // destino 01(servidor), origem 10(cliente), tam  
        // mensagemBruta[2] = 0b00001011 | (sequencia << 4);        
        int i = 3;
        for(; itens.length() - len > 0; i++){
            // mensagemBruta[i] = itens[len];
            len++;
        }
        // mensagemBruta[i] = 0b10101010;
        // send(soquete, &mensagemBruta, 20, 0);
        // recv(soquete, &dado_recebido, 20, 0);
        Mensagem aux(dado_recebido);
        aux.printMensagemString();
        sequencia++;
    }

    return sequencia;
}

void recebeRespostaLs(uint8_t sequencia, int soquete){

}

std::string list() {
    std::string path = fs::current_path();
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
    
    Mensagem pedidoLs(mensagemBruta);
    pedidoLs.printMensagemString();
    if (pedidoLs.enviaMensagem(soquete) <= 0)
        exit(-1);
    Mensagem resposta = pedidoLs.recebeResposta(soquete);
    if (resposta.corpo.tipo == 0b1111)
        std::cout << "oops! \n";
        // exit(-1);
    
    resposta.printMensagemString();

    // int tentativas = 0;
    // while(tentativas < 5){
    //     // dadosLs();
    // }

    return sequencia + 1;
}
