#include <iostream>
#include <string>
#include <sys/socket.h>
#include <string.h>
#include <chrono>
#include "mensagem.h"
#include "list.h"
#include "directory.h"
#include "ver.h"
#include "edit.h"
#include "linha.h"
#include "linhas.h"

extern "C" {
  #include "rawsockets.h" 
}

int main(){
    char tipo_conexao[] = "lo";
    int soquete = ConexaoRawSocket(tipo_conexao);
    uint8_t dado_recebido[20];
    int nread;
    uint8_t sequencia = 0;
    
    struct timeval tv;
    tv.tv_sec = 60*5;
    tv.tv_usec = 0;
    int timeout;
    
    while(true){
        timeout = setsockopt(soquete, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
        if (timeout == 0)
            nread = recv(soquete, &dado_recebido, 20, 0);
        else
            continue;
        if (nread != 20){
            return(-1); 
        }
        
        Mensagem mensagemRecebida(dado_recebido);
        if(
            mensagemRecebida.corpo.destino != 0b10 
            || mensagemRecebida.corpo.sequencia != sequencia 
        ){
            continue;
        }
        // if (msg_cd)
        int resposta;
        switch(mensagemRecebida.corpo.tipo) {
            case 0x00: // cd
                resposta = respostaCd(&sequencia, soquete, mensagemRecebida);
                break;
            case 0x01: // ls
                resposta = respostaLs(&sequencia, soquete);
                break;
            case 0x02: // ver
                resposta = respostaVer(&sequencia, soquete, mensagemRecebida);
                break;
            case 0x03: // linha
                resposta = respostaLinha(&sequencia, soquete, mensagemRecebida);
                break;
            case 0x04: // linhas
                resposta = respostaLinhas(&sequencia, soquete, mensagemRecebida);
                break;
            case 0x05: // edit
                resposta = respostaEdit(&sequencia, soquete, mensagemRecebida);
                break;
            default:
                break;
        }
    }
    return 0;
}
