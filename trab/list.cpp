#include "list.h"
#include "mensagem.h"
#include "directory.h"

std::string ls() {
    std::string path = fs::current_path();
    std::string out = "";
    for (const auto & entry : fs::directory_iterator(path)){
        out = out + entry.path().filename().string() + "\n";
    }
    return out;
}

int respostaLs(uint8_t* sequencia, int soquete){
    
    uint8_t array_dados[15];
    std::string itens = ls();
    // std::cout << itens;
    int len = 0;

    // envia todas as mensagens de tamanho 15
    while( len < itens.length()){
        int tam_envio = 0;
        for(; tam_envio < itens.length() - len && tam_envio < 15; tam_envio++){
            array_dados[tam_envio] = itens[len];
            len++;
        }  
        // envia a mensagem com os dados
        *sequencia = ((*sequencia + 1) & 0x0F);
        Mensagem mensagem(tam_envio, 0b10, 0b01, 0b1011, *sequencia, array_dados);
        if (mensagem.enviaMensagem(soquete) < 20)
            return -1;       

        Mensagem resposta = mensagem.recebeResposta(soquete);
        // verifica timeout
        if(resposta.isEqual(mensagem)){
            return -1;     
        }
        // busca uma resposta se não for uma das esperadas
        
        // código de erro
        if (resposta.corpo.tipo == 0b1111){
            return 0;
        }
        // manda a mesma mensagem enquanto a resposta for NACK
        while (resposta.corpo.tipo == 0b1001 && resposta.corpo.sequencia == mensagem.corpo.sequencia && mensagem.verificaParidade() ){
            Mensagem mensagem(tam_envio, 0b10, 0b01, 0b1011, *sequencia, array_dados);

            if (mensagem.enviaMensagem(soquete) < 20)
                return -1;          // give up
                 
            Mensagem resposta = mensagem.recebeResposta(soquete);
            if(resposta.isEqual(mensagem))
                return -1;          // give up
        }

        if (mensagem.corpo.tamanho == 0 && resposta.corpo.tipo == 0b1000 && mensagem.verificaParidade()){
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
            if (mensagem.enviaMensagem(soquete) < 20)
                return -1;          // give up
                 
            Mensagem resposta = mensagem.recebeResposta(soquete);
            if(resposta.isEqual(mensagem) || !mensagem.verificaParidade())
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
        if (dados_ls.corpo.sequencia == *sequencia && dados_ls.verificaParidade()){ // (lembrando que precisa manter seq em 4bits)
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
