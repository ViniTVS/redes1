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
    uint8_t sequencia = 0;
    while(true){
        std::cout << "\033[2J\033[1;1H";
        std::cout << ">";
        std::string entrada;

        // realizar a leitura da entrada de toda a linha em vez de string por string
        std::getline(std::cin, entrada);
        std::cout << "\033[2J\033[1;1H";
        std::string comando = entrada.substr(0, entrada.find(" ")); // separa o comando

        if (comando == "cd"){

        }
        else if (comando == "lcd"){
            std::string nome_dir = entrada.substr(4, entrada.length());
            trocaDir(nome_dir);
        }
        else if (comando == "ls"){
            criaPedidoLs(sequencia, soquete);
        }
        else if (comando == "lls"){
            std::cout << list();
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
    
    while(true){

        nread = recv(soquete, &dado_recebido, 20, 0);
        // std::cout << nread << "\n";
        if (nread == -1)
            return(-1); 
        else{
            Mensagem mensagemRecebida(dado_recebido);
            uint8_t sequencia = mensagemRecebida.getSequencia();
            if(mensagemRecebida.corpo.destino != 0b10)
                continue;
            else 
                mensagemRecebida.printMensagem();
            switch(mensagemRecebida.getTipo()) {
                case 0x00: // cd
                //     // code block
                    break;
                case 0x01: // ls
                    enviaRespostaLs(sequencia, soquete);

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