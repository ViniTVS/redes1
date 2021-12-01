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

extern "C" {
  #include "rawsockets.h" 
}

int clientMain(int soquete){
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
            // std::cout << entrada << "\n";
            std::cout << nome_arq << "\n";
        }
        else if (comando == "linha"){

        }
        else if (comando == "linhas"){

        }
        else if (comando == "edit"){

        }
        else if (comando == "compilar" || comando == "Compilar"){

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

int serverMain(int soquete){
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
            || mensagemRecebida.corpo.sequencia != (sequencia & 0x0F) 
            || !mensagemRecebida.verificaParidade()
        ){
            continue;
        }
        int resposta;
        switch(mensagemRecebida.corpo.tipo) {
            case 0x00: // cd
                respostaCd(&sequencia, soquete, mensagemRecebida);
                break;
            case 0x01: // ls
                resposta = respostaLs(&sequencia, soquete);
                if (resposta == -1){
                    std::cout << "ERRO! \n";
                }
                break;
            // case 0x02:
            // //     // code block
            //     break;
            // case 0x03:
            // //     // code block
            //     break;
            // case 0x04:
            // //     // code block
            //     break;
            // case 0x05:
            // //     // code block
            //     break;
            // case 12:
            //     // code block
            //     break;
            default:
                break;
            //     continue;

                // std::cout << std::bitset<8>(mensagemRecebida.getTipo());
                // std::cout << " Mensagem recebida: \n";
                // mensagemRecebida.printMensagem();
                // code block
            
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
    
}