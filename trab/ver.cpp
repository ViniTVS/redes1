#include "ver.h"


int respostaVer(uint8_t* sequencia, int soquete, Mensagem msg_linha){    
    std::string nome_arquivo = "";
    // *-------------------- leio o nome do arquivo ----------------------------
    for (int i = 0; i < msg_linha.corpo.tamanho; i++)
        nome_arquivo += msg_linha.dados[i].c;
    
    std::fstream arquivo; 
    std::string conteudo_arquivo;
    arquivo.open(nome_arquivo, std::ios::in); 
    // falha ao abrir o arquivo:
    if (arquivo.fail()){
        uint8_t array_dados[] = {3};
        Mensagem resposta(1, 0b10, 0b01, 0b1111, *sequencia, array_dados); // erro 3
        if (resposta.enviaMensagem(soquete) == 20)
            *sequencia = ((*sequencia + 1) & 0x0F);
        arquivo.close();
        return 0;
    }
    int linha = 1;
    while (!arquivo.eof()){
        std::string aux;
        std::getline (arquivo, aux);
        conteudo_arquivo += std::to_string(linha) + " " + aux + "\n";
        // arquivo = arquivo.get();
        linha++;
    }
    
    arquivo.close();
    
    // *-------------------- envio o conteúdo do arquivo ----------------------------
    int len = 0;
    uint8_t array_dados[20];
    *sequencia = ((*sequencia + 1) & 0x0F);
    while( len < conteudo_arquivo.length()){
        int tam_envio = 0;
        for(; len < conteudo_arquivo.length() && tam_envio < 15; tam_envio++){
            array_dados[tam_envio] = conteudo_arquivo[len];
            len++;
        }  
        // envia a mensagem com os dados
        Mensagem mensagem(tam_envio, 0b10, 0b01, 0b1100, *sequencia, array_dados);
        if (mensagem.enviaMensagem(soquete) < 20)
            return -1;       

        Mensagem resposta = mensagem.recebeResposta(soquete);
        // verifica timeout
        if(resposta.isEqual(mensagem)){
            return -1;     
        }
        
        // manda a mesma mensagem enquanto a resposta for NACK
        while ((resposta.corpo.tipo == 0b1001 && resposta.corpo.sequencia == mensagem.corpo.sequencia) || !resposta.verificaParidade()){
            Mensagem mensagem(tam_envio, 0b10, 0b01, 0b1100, *sequencia, array_dados);

            if (mensagem.enviaMensagem(soquete) < 20)
                return -1;          // give up
                 
            Mensagem resposta = mensagem.recebeResposta(soquete);
            if(resposta.isEqual(mensagem))
                return -1;          // give up
        }
        if (resposta.verificaParidade() && resposta.corpo.tipo == 0b1000){
            *sequencia = ((*sequencia + 1) & 0x0F);
        }
    }

    // *--------------------------- final de envio ----------------------------
    
    *sequencia = ((*sequencia + 1) & 0x0F);
    Mensagem mensagem(0, 0b10, 0b01, 0b1101, *sequencia, NULL);
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


int pedidoVer(uint8_t* sequencia, int soquete, std::string nome_arquivo){
    std::string saida_linha;
    uint8_t array_dados[20];
    // sair se o nome for maior que 15 chars
    if (nome_arquivo.length() > 15)
        return 0;
    
    // passo o nome do arquivo p/ os dados
    for(int i = 0; i < nome_arquivo.length(); i++){
        array_dados[i] = nome_arquivo[i];
    }  

    Mensagem msg_linha(nome_arquivo.length(), 0b01, 0b10, 0b0010, *sequencia, array_dados); // pedido do comando
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
    if (dados_linha.corpo.tipo == 0b1111){
        messageError(dados_linha);
        *sequencia = ((*sequencia + 1) & 0x0F);
        return 0;
    } 
    //* ------------------------ enquanto tenho mensagem pra ler --------------------------------------
    *sequencia = ((*sequencia + 1) & 0x0F);
    while(dados_linha.corpo.tipo == 0b1100){
        // verifico se a mensagem está na ordem esperada e sua paridade
        if (dados_linha.corpo.sequencia == *sequencia && dados_linha.verificaParidade()){ 
            for(int i = 0; i < unsigned(dados_linha.corpo.tamanho); i++){
                saida_linha += dados_linha.dados[i].c;
            }
            // respondo ACK
            Mensagem resposta(0, 0b01, 0b10, 0b1000, *sequencia, NULL);
            if (resposta.enviaMensagem(soquete) < 20)
                return -1;
            dados_linha = resposta.recebeResposta(soquete);
            if (dados_linha.isEqual(resposta))
                return -1;
            *sequencia = ((*sequencia + 1) & 0x0F);
            
        } else { // mensagem mal formada ou fora de sequência
            Mensagem resposta(0, 0b01, 0b10, 0b1001, dados_linha.corpo.sequencia, NULL); // NACK
            if (resposta.enviaMensagem(soquete) < 20)
                return -1;
            dados_linha = resposta.recebeResposta(soquete);
            if (dados_linha.isEqual(resposta))
                return -1;

        }
    }
    if (dados_linha.corpo.tipo == 0b1101 && dados_linha.verificaParidade()){
        std::cout << saida_linha;
        *sequencia = ((*sequencia + 1) & 0x0F);
    }
    else
        return -1;

    return 1;
}
