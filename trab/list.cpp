#include "list.h"
#include "mensagem.h"
#include "directory.h"

std::string list() {
    std::string path = fs::current_path();
    std::string out = "";
    for (const auto & entry : fs::directory_iterator(path)){
        out = out + entry.path().filename().string() + "\n";
    }
    return out;
}

int respostaLs(uint8_t* sequencia, int soquete){
    
    uint8_t array_dados[15];
    std::string itens = list();
    // std::cout << itens;
    int len = 0;

    // envia todas as mensagens de tamanho 15
    while( len < itens.length()){
        int j = 0;
        for(; j < itens.length() - len && j < 15; j++){
            array_dados[j] = itens[len];
            len++;
        }  
        // envia a mensagem com os dados
        *sequencia = ((*sequencia + 1) & 0x0F);
        Mensagem mensagem(j, 0b10, 0b01, 0b1011, *sequencia, array_dados);

        if (mensagem.enviaMensagem(soquete) <= 0)
            return -1;       

        // verifica timeout
        Mensagem resposta = mensagem.recebeResposta(soquete);

        if(resposta.isEqual(mensagem)){
            // ! criar erro de timeout
            return -1;     
        }
        // igonoro a resposta se não for uma das esperadas
        while ((resposta.corpo.tipo != 0b1001 && resposta.corpo.tipo != 0b1000 && resposta.corpo.tipo != 0b1111) || resposta.corpo.sequencia != mensagem.corpo.sequencia){
            resposta = mensagem.recebeResposta(soquete);
            
            if(resposta.isEqual(mensagem)){
                // ! criar erro de timeout
                return -1;     
            }
        }
        // código de erro
        if (resposta.corpo.tipo == 0b1111){
            return -1;
        }
        // manda a mesma mensagem enquanto a resposta for NACK
        while (resposta.corpo.tipo == 0b1001 && resposta.corpo.sequencia == mensagem.corpo.sequencia ){
            Mensagem mensagem(j, 0b10, 0b01, 0b1011, *sequencia, array_dados);

            if (mensagem.enviaMensagem(soquete) <= 0)
                return -1;          // give up
                 
            Mensagem resposta = mensagem.recebeResposta(soquete);
            if(resposta.isEqual(mensagem))
                return -1;          // give up
        }

        if (mensagem.corpo.tamanho == 0 && resposta.corpo.tipo == 0b1000){
            return 1;
        }
    }

    *sequencia = ((*sequencia + 1) & 0x0F);
    // final de envio
    Mensagem mensagem(0, 0b10, 0b01, 0b1011, *sequencia, NULL);
    if (mensagem.enviaMensagem(soquete) < 20)
        return -1;   
    Mensagem resposta = mensagem.recebeResposta(soquete);
    if (resposta.corpo.sequencia == mensagem.corpo.sequencia){
        if (resposta.corpo.tipo == 0b1000){
            return 1;
        }
        while (resposta.corpo.tipo == 0b1001){
            Mensagem mensagem(0, 0b10, 0b01, 0b1011, *sequencia, NULL);
            if (mensagem.enviaMensagem(soquete) <= 0)
                return -1;          // give up
                 
            Mensagem resposta = mensagem.recebeResposta(soquete);
            if(resposta.isEqual(mensagem))
                return -1;          // give up
        }
    }
    
    return 0;
}

int pedidoLs(uint8_t* sequencia, int soquete){
    std::string saida_ls;
    
    Mensagem msg_ls(0, 0b01, 0b10, 0b0001, *sequencia, NULL); // pedido do comando
    // falha no envio da mensagem
    if (msg_ls.enviaMensagem(soquete) < 20)
        return (-1);
    Mensagem dados_ls = msg_ls.recebeResposta(soquete);
    // verifica se foi timeout
    if (dados_ls.isEqual(msg_ls)){
        std::cout << "Server timeout\n";
        return (0);
    }

    *sequencia = ((*sequencia + 1) & 0x0F);
    // enquanto tenho mensagem pra ler 
    while(dados_ls.corpo.tipo == 0b1011 && dados_ls.corpo.tamanho != 0){
        // verifico se a mensagem está na ordem esperada 
        if (dados_ls.corpo.sequencia == *sequencia){ // (lembrando que precisa manter seq em 4bits)
            for(int i = 0; i < dados_ls.corpo.tamanho; i++){
                saida_ls += dados_ls.dados[i].c;
            }
            // respondo ACK
            Mensagem resposta(0, 0b01, 0b10, 0b1000, dados_ls.corpo.sequencia, NULL);
            *sequencia = ((*sequencia + 1) & 0x0F);
            resposta.enviaMensagem(soquete);
            dados_ls = resposta.recebeResposta(soquete);
        } else {
            // respondo NACK
            Mensagem resposta(0, 0b01, 0b10, 0b1001, *sequencia, NULL);
            resposta.enviaMensagem(soquete);
            dados_ls = resposta.recebeResposta(soquete);
        }
    }
    std::cout << saida_ls;

    return 1;
}
