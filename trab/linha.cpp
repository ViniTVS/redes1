#include "linha.h"

int pedidoLinha(uint8_t* sequencia, int soquete, uint8_t linha, std::string nome_arquivo){
    // cria mensagem contendo o número da linha
    uint8_t array_dados[20];
    if (nome_arquivo.length() > 15)
        return -1;
    for(int i = 0; i < nome_arquivo.length(); i++){
        array_dados[i] = nome_arquivo[i];
    }
    // *------------------------- envio do nome do arquivo --------------------------------- 
    Mensagem msg_linha(nome_arquivo.length(), 0b01, 0b10, 0b0011, *sequencia, array_dados); // pedido do comando
    // falha no envio da mensagem
    if (msg_linha.enviaMensagem(soquete) < 20)
        return -1;

    Mensagem dados_linha = msg_linha.recebeResposta(soquete);
    // verifica se foi timeout
    if (dados_linha.isEqual(msg_linha)){
        return -1;
    }
    // tenta re-enviar a mensagem em caso de erro
    if (dados_linha.corpo.tipo == 0b1001 || !dados_linha.verificaParidade()){
        if (msg_linha.enviaMensagem(soquete) < 20)
            return -1; //give up
        Mensagem dados_linha = msg_linha.recebeResposta(soquete);
        // verifica se foi timeout
        if (dados_linha.isEqual(msg_linha)){
            return -1; //give up
        }
    }
    else if (dados_linha.corpo.tipo == 0b1111 && dados_linha.verificaParidade()){
        messageError(dados_linha);
        *sequencia = ((*sequencia + 1) & 0x0F);
        return 0;
    } else if (dados_linha.corpo.tipo == 0b1000 && dados_linha.verificaParidade()){
        *sequencia = ((*sequencia + 1) & 0x0F);
    }
    else // qualquer outra coisa...
        return -1;

    // *---------------------------- envio do número da linha --------------------------------- 

    array_dados[0] = linha;
    Mensagem msg_linha_num(1, 0b01, 0b10, 0b1010, *sequencia, array_dados); // envio do número da linha

    if (msg_linha_num.enviaMensagem(soquete) < 20)
        return -1; //give up
    
    Mensagem dados_linha_num = msg_linha_num.recebeResposta(soquete);
    // verifica se foi timeout
    if (dados_linha_num.isEqual(msg_linha_num))
        return -1;
    
    // tenta re-enviar a mensagem em caso de erro
    if (dados_linha_num.corpo.tipo == 0b1001 || !dados_linha_num.verificaParidade()){
        if (msg_linha.enviaMensagem(soquete) < 20)
            return -1; //give up
        Mensagem dados_linha_num = msg_linha.recebeResposta(soquete);
        // verifica se foi timeout
        if (dados_linha_num.isEqual(msg_linha)){
            return -1; //give up
        }
    }
    else if (dados_linha_num.corpo.tipo == 0b1111 && dados_linha_num.verificaParidade()){
        messageError(dados_linha_num);
        *sequencia = ((*sequencia + 1) & 0x0F);
        return 0;
    } else if (dados_linha_num.corpo.tipo != 0b1100 || !dados_linha_num.verificaParidade()){
        return -1;
    }
    
    std::string saida_linha = "";
    *sequencia = ((*sequencia + 1) & 0x0F);
    // *------------------------- leitura da linha do arquivo --------------------------------- 
    while(dados_linha_num.corpo.tipo == 0b1100 && dados_linha_num.corpo.tamanho != 0){
        // verifico se a mensagem está na ordem esperada e sua paridade
        if (dados_linha_num.corpo.sequencia == *sequencia && dados_linha_num.verificaParidade()){ 
            for(int i = 0; i < dados_linha_num.corpo.tamanho; i++){
                saida_linha += dados_linha_num.dados[i].c;
            }
            // respondo ACK
            Mensagem resposta(0, 0b01, 0b10, 0b1000, dados_linha_num.corpo.sequencia, NULL);
            *sequencia = ((*sequencia + 1) & 0x0F);
            if (resposta.enviaMensagem(soquete) < 20)
                return -1;
            else
                dados_linha_num = resposta.recebeResposta(soquete);
        } else { // mensagem mal formada ou fora de sequência
            Mensagem resposta(0, 0b01, 0b10, 0b1001, dados_linha_num.corpo.sequencia, NULL); // NACK
            if (resposta.enviaMensagem(soquete) < 20)
                return -1;
            else
                dados_linha_num = resposta.recebeResposta(soquete);
        }
    }
    std::cout << saida_linha;

    return 1;
}


int respostaLinha(uint8_t* sequencia, int soquete, Mensagem msg_linha){
    // *------------------------- leio o nome do arquivo --------------------------------- 
    std::string nome_arquivo = "";

    for (int i = 0; i < msg_linha.corpo.tamanho; i++)
        nome_arquivo += msg_linha.dados[i].c;
        
    std::fstream arquivo; 
    std::string conteudo_arquivo;
    arquivo.open(nome_arquivo, std::ios::in); 
    // falha ao abrir o arquivo:
    if (arquivo.fail()){
        uint8_t array_dados[] = {3};
        Mensagem resposta(1, 0b10, 0b01, 0b1111, *sequencia, array_dados); // erro 1
        if (resposta.enviaMensagem(soquete) == 20)
            *sequencia = ((*sequencia + 1) & 0x0F);
        arquivo.close();
        return 0;
    }
    Mensagem resposta_ACK(0, 0b10, 0b01, 0b1000, *sequencia, NULL); // ACK
    if (resposta_ACK.enviaMensagem(soquete) == 20)
        *sequencia = ((*sequencia + 1) & 0x0F);

    // *---------------------------- leio o número da linha --------------------------------- 
    Mensagem mensagem_linha = resposta_ACK.recebeResposta(soquete);
    if (mensagem_linha.isEqual(resposta_ACK))
        return -1;
    uint8_t linha = mensagem_linha.dados[0].num;
    for (int i = 0; i < linha ; i++){
        if (arquivo.eof()){
            arquivo.close();
            uint8_t array_dados[] = {4};
            Mensagem erro_linha(1, 0b10, 0b01, 0b1111, *sequencia, array_dados); // erro 4
            if (erro_linha.enviaMensagem(soquete) == 20){
                *sequencia = ((*sequencia + 1) & 0x0F);
                return 0;
            }
            return -1; // erro no envio
        }
        std::getline (arquivo, conteudo_arquivo);
    }
    arquivo.close();
    conteudo_arquivo += "\n";
    
    int len = 0;
    uint8_t array_dados[20];
    // *---------------------------- envio o conteúdo da linha --------------------------------- 
    while( len < conteudo_arquivo.length()){
        int tam_envio = 0;
        for(; tam_envio < conteudo_arquivo.length() - len && tam_envio < 15; tam_envio++){
            array_dados[tam_envio] = conteudo_arquivo[len];
            len++;
        }  
        // envia a mensagem com os dados
        *sequencia = ((*sequencia + 1) & 0x0F);
        Mensagem mensagem(tam_envio, 0b10, 0b01, 0b1100, *sequencia, array_dados);
        
        if (mensagem.enviaMensagem(soquete) < 20)
            return -1;       

        Mensagem resposta = mensagem.recebeResposta(soquete);
        // verifica timeout
        if(resposta.isEqual(mensagem) || !resposta.verificaParidade()){
            return -1;     
        }
        // manda a mesma mensagem enquanto a resposta for NACK
        while (resposta.corpo.tipo == 0b1001 && resposta.corpo.sequencia == mensagem.corpo.sequencia && resposta.verificaParidade()){
            Mensagem mensagem(tam_envio, 0b10, 0b01, 0b1100, *sequencia, array_dados);

            if (mensagem.enviaMensagem(soquete) < 20)
                return -1;          // give up
                 
            Mensagem resposta = mensagem.recebeResposta(soquete);
            if(resposta.isEqual(mensagem))
                return -1;          // give up
        }

        if (mensagem.corpo.tamanho == 0 && resposta.corpo.tipo == 0b1000){
            return 1;
        }
    }

    // final de envio
    *sequencia = ((*sequencia + 1) & 0x0F);
    Mensagem mensagem(0, 0b10, 0b01, 0b1100, *sequencia, NULL);
    if (mensagem.enviaMensagem(soquete) < 20)
        return -1;   
    Mensagem resposta = mensagem.recebeResposta(soquete);
    if (resposta.corpo.sequencia == mensagem.corpo.sequencia && resposta.verificaParidade()){
        if (resposta.corpo.tipo == 0b1000){
            return 1;
        }
        while (resposta.corpo.tipo == 0b1001 && resposta.verificaParidade()){
            Mensagem mensagem(0, 0b10, 0b01, 0b1100, *sequencia, NULL);
            if (mensagem.enviaMensagem(soquete) < 20)
                return -1;          // give up
                 
            Mensagem resposta = mensagem.recebeResposta(soquete);
            if(resposta.isEqual(mensagem) || !mensagem.verificaParidade())
                return -1;          // give up
        }
    }

    return 1;
    
}