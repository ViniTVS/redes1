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
    std::string itens = list();
    // std::cout << itens;
    int len = 0;

    // envia todas as mensagens de tamanho 15
    for(int i = 0; i < itens.length()/15; i++){
        
        for(int j = 0; j < 15; j++){
            array_dados[j] = itens[len];
            len++;
        }  
        // envia a mensagem com os dados
        Mensagem mensagem(15, 0b10, 0b01, 0b1011, ++sequencia, array_dados);

        std::cout << "Estou enviando:\n";
        mensagem.printMensagemString();
        if (mensagem.enviaMensagem(soquete) <= 0)
            return -1;       

        // verifica timeout
        Mensagem resposta = mensagem.recebeResposta(soquete);
        std::cout << "Resposta:\n";
        resposta.printMensagemString();
        if(resposta.isEqual(mensagem)){
            // ! criar erro de timeout
            return -1;     
        }
        // igonoro a resposta se não for uma das esperadas
        while ((resposta.corpo.tipo != 0b1001 && resposta.corpo.tipo != 0b1000 && resposta.corpo.tipo != 0b1111) || resposta.corpo.sequencia != mensagem.corpo.sequencia){
            resposta = mensagem.recebeResposta(soquete);
            std::cout << "Nova resposta:\n";
            resposta.printMensagemString();
            if(resposta.isEqual(mensagem)){
                // ! criar erro de timeout
                return -1;     
            }
        }
        // manda a mesma mensagem enquanto a resposta for NACK
        while (resposta.corpo.tipo == 0b1001 && resposta.corpo.sequencia == mensagem.corpo.sequencia ){
            Mensagem mensagem(15, 0b10, 0b01, 0b1011, sequencia, array_dados);
            std::cout << "Estou enviando:\n";
            mensagem.printMensagemString();
            if (mensagem.enviaMensagem(soquete) <= 0)
                return -1;          // give up
                 
            Mensagem resposta = mensagem.recebeResposta(soquete);
            
            std::cout << "recebi uma resposta:\n";
            resposta.printMensagemString();
            std::cout << "\n\n\n\n\n\n";
            if(resposta.isEqual(mensagem))
                return -1;          // give up
        }
        
        // ACK!
        // if(resposta.corpo.tipo == 0b1000){
        //     sequencia++;
        // }
        // return 0;
        // Mensagem aux(dado_recebido);
        // aux.printMensagemString();
    }
    // mensagens com menos de 15 chars
    if (len < itens.length()){
    
        uint8_t restante = itens.length() - len;
        for(int i = 0 ; itens.length() - len > 0; i++){
            array_dados[i] = itens[len];
            len++;
        }  

        // envia a mensagem com os dados
        Mensagem mensagem(restante, 0b10, 0b01, 0b1011, sequencia++, array_dados);
        if (mensagem.enviaMensagem(soquete) <= 0)
            return -1;       
        std::cout << "recebi uma resposta:\n";

        // verifica timeout
        Mensagem resposta = mensagem.recebeResposta(soquete);
        if(resposta.isEqual(mensagem)){
            // ! criar erro de timeout
            return -1;     
        }
        // manda a mesma mensagem enquanto a resposta for NACK
        while (resposta.corpo.tipo == 0b1001 && resposta.corpo.sequencia == mensagem.corpo.sequencia){
            Mensagem mensagem(15, 0b10, 0b01, 0b1011, sequencia, array_dados);
            if (mensagem.enviaMensagem(soquete) <= 0)
                return -1;          // give up
                 
            Mensagem resposta = mensagem.recebeResposta(soquete);
            if(resposta.isEqual(mensagem))
                return -1;          // give up
        }
        // mensagemBruta[i] = 0b10101010;
        // send(soquete, &mensagemBruta, 20, 0);
        // recv(soquete, &dado_recebido, 20, 0);
    }



    Mensagem mensagem(0, 0b10, 0b01, 0b1011, sequencia++, NULL);
    if (mensagem.enviaMensagem(soquete) <= 0)
        return -1;       

    return sequencia;
}

std::string list() {
    std::string path = fs::current_path();
    std::string out = "";
    for (const auto & entry : fs::directory_iterator(path)){
        out = out + entry.path().filename().string() + "\n";
    }
    return out;
}

uint8_t recebeLs(int soquete, uint8_t sequencia){
    uint8_t dado_recebido[20];
    std::string saida_ls;
    int recv = read(soquete, &dado_recebido, 20);
    if (recv < 20){
        return 0;
    }
    Mensagem dados_ls(dado_recebido);
    std::cout << "Respdfasfsaddxsadasosta:\tseq:" << unsigned(sequencia) << "\n";
    dados_ls.printMensagemString();   
    if (dados_ls.corpo.sequencia == sequencia){
        for(int i = 0; i < dados_ls.corpo.tamanho; i++){
            saida_ls += dados_ls.dados[i].c;
        }

        std::cout << "Respdfasfsaddxsadasosta:\n";
        dados_ls.printMensagemString();            
        // respondo ACK
        Mensagem msg(0, 0b01, 0b10, 0b1000,  sequencia, NULL); // cria mensagem de ACK
        msg.enviaMensagem(soquete);
        dados_ls = msg.recebeResposta(soquete);
        sequencia++;
    }
    // enquanto recebe dados
    while(dados_ls.corpo.tamanho != 0 && dados_ls.corpo.tipo == 0b1011){
        
        
        // !! verifica paridade
        // if (paridade)
        if (dados_ls.corpo.sequencia == sequencia){
            for(int i = 0; i < dados_ls.corpo.tamanho; i++){
                saida_ls += dados_ls.dados[i].c;
            }

            std::cout << "Respdfasfsaddxsadasosta:\n";
            dados_ls.printMensagemString();            
            // respondo ACK
            Mensagem msg(0, 0b01, 0b10, 0b1000,  sequencia, NULL); // cria mensagem de ACK
            msg.enviaMensagem(soquete);
            dados_ls = msg.recebeResposta(soquete);
            sequencia++;
            
        }else { //ignoro a mensagem
            continue;
        }
        // else
        // Mensagem msg(0, 0b01, 0b10, 0b1111, resposta.corpo.sequencia, NULL); // cria mensagem de ACK


        std::cout << "\n\n" << saida_ls << "\n\n";

    }
    return 0;
    // std::string saida_ls;
    // // exit (0);
    // if(resposta.corpo.tipo != 0b1011){

    //     return (-1);
    // }
    // for(int i = 0; i < unsigned(resposta.corpo.tamanho); i++){
    //     saida_ls += resposta.dados[i].c;
    // }


    

    // else {
    //     return (-1);
    // }

    std::cout << saida_ls << "\n";

    // int tentativas = 0;
    // while(tentativas < 5){
    //     // dadosLs();
    // }

    return sequencia;
}

int pedidoLs(uint8_t sequencia, int soquete){
    uint8_t seq = sequencia;
    std::string saida_ls;
    Mensagem msg_ls(0, 0b01, 0b10, 0b0001, seq, NULL);
    
    if (msg_ls.enviaMensagem(soquete) < 20)
        return (-1);
    Mensagem dados_ls = msg_ls.recebeResposta(soquete);
    
    // verifica se foi timeout
    if (dados_ls.isEqual(msg_ls)){
        std::cout << "Server timeout\n";
        return (0);
    }

    seq++;
    while(dados_ls.corpo.tamanho != 0 && dados_ls.corpo.tipo == 0b1011){
        
        std::cout << "dados:\n";
        dados_ls.printMensagemString();
        if (dados_ls.corpo.sequencia == seq){
            for(int i = 0; i < dados_ls.corpo.tamanho; i++){
                saida_ls += dados_ls.dados[i].c;
            }
            // respondo ACK
            Mensagem resposta(0, 0b01, 0b10, 0b1000,  seq, NULL);
            seq++;
            resposta.enviaMensagem(soquete);
            dados_ls = resposta.recebeResposta(soquete);
            std::cout << "resposta:\n";
            resposta.printMensagemString();
        } else {
            
            // respondo NACK
            Mensagem resposta(0, 0b01, 0b10, 0b1001,  seq, NULL);
            resposta.enviaMensagem(soquete);
            dados_ls = resposta.recebeResposta(soquete);
            std::cout << "resposta:\n";
            resposta.printMensagemString();
        }
        std::cout << saida_ls << "\n\n";
    }
    // Mensagem dados_ls = 




    return 1;
}
