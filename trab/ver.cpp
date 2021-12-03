#include "ver.h"
/*
std::string ls() {
    std::string path = fs::current_path();
    std::string out = "";
    for (const auto & entry : fs::directory_iterator(path)){
        out = out + entry.path().filename().string() + "\n";
    }
    return out;
}
*/


int respostaVer(uint8_t* sequencia, int soquete, Mensagem msg_linha){    
    std::string nome_arquivo = "";

    for (int i = 0; i < msg_linha.corpo.tamanho; i++)
        nome_arquivo += msg_linha.dados[i].c;
    // if (nome_arquivo.find(".c") != std::string::npos) {
    //     std::cout << "found!" << '\n';
    // }
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

    while (!arquivo.eof()){
        std::string aux;
        std::getline (arquivo, aux);
        conteudo_arquivo += aux + "\n";
        // arquivo = arquivo.get();
    }
    arquivo.close();
    // else
    // return 0;
    int len = 0;
    uint8_t array_dados[20];
    // envio o conteúdo do arquivo
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
        mensagem.printMensagemString();
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
    if (dados_linha.isEqual(msg_linha) || !dados_linha.verificaParidade()){
        return -1;
    }
    // tenta re-enviar a mensagem em caso de erro
    if (dados_linha.corpo.tipo == 0b1001){
        if (msg_linha.enviaMensagem(soquete) < 20)
            return -1; //give up
        Mensagem dados_linha = msg_linha.recebeResposta(soquete);
        // verifica se foi timeout
        if (dados_linha.isEqual(msg_linha)){
            return -1; //give up
        }
    }
    *sequencia = ((*sequencia + 1) & 0x0F);
    if (dados_linha.corpo.tipo == 0b1111){
        messageError(dados_linha);
        return 0;
    } 
    // enquanto tenho mensagem pra ler 
    while(dados_linha.corpo.tipo == 0b1100 && dados_linha.corpo.tamanho != 0){
        // verifico se a mensagem está na ordem esperada e sua paridade
        if (dados_linha.corpo.sequencia == *sequencia && dados_linha.verificaParidade()){ 
            for(int i = 0; i < dados_linha.corpo.tamanho; i++){
                saida_linha += dados_linha.dados[i].c;
            }
            // respondo ACK
            Mensagem resposta(0, 0b01, 0b10, 0b1000, dados_linha.corpo.sequencia, NULL);
            *sequencia = ((*sequencia + 1) & 0x0F);
            if (resposta.enviaMensagem(soquete) < 20)
                return -1;
            else
                dados_linha = resposta.recebeResposta(soquete);
        } else { // mensagem mal formada ou fora de sequência
            Mensagem resposta(0, 0b01, 0b10, 0b1001, dados_linha.corpo.sequencia, NULL); // NACK
            if (resposta.enviaMensagem(soquete) < 20)
                return -1;
            else
                dados_linha = resposta.recebeResposta(soquete);
        }
    }
    std::cout << saida_linha;

    return 1;
}

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
    msg_linha.printMensagemString();
    // falha no envio da mensagem
    if (msg_linha.enviaMensagem(soquete) < 20)
        return -1;

    Mensagem dados_linha = msg_linha.recebeResposta(soquete);
    // verifica se foi timeout
    std::cout << "resposta:\n";
    dados_linha.printMensagemString();
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
    else 
        return -1;
    // *---------------------------- envio do número da linha --------------------------------- 
    // enviar o nome do arquivo
    array_dados[0] = linha;
    Mensagem msg_linha_num(1, 0b01, 0b10, 0b1010, *sequencia, array_dados); // pedido do comando
    msg_linha_num.printMensagem();
    if (msg_linha_num.enviaMensagem(soquete) < 20);
        return -1; //give up
    Mensagem dados_linha_num = msg_linha_num.recebeResposta(soquete);
    // verifica se foi timeout
    if (dados_linha_num.isEqual(msg_linha_num)){
        return -1;
    }
    // tenta re-enviar a mensagem em caso de erro
    if (dados_linha_num.corpo.tipo == 0b1001 || !dados_linha_num.verificaParidade()){
        if (msg_linha_num.enviaMensagem(soquete) < 20)
            return -1; //give up
        Mensagem dados_linha_num = msg_linha_num.recebeResposta(soquete);
        // verifica se foi timeout
        if (dados_linha_num.isEqual(msg_linha_num)){
            return -1; //give up
        }
    }
    if (dados_linha_num.corpo.tipo == 0b1111 && dados_linha_num.verificaParidade()){
        messageError(dados_linha_num);
        *sequencia = ((*sequencia + 1) & 0x0F);
        return 0;
    }
    *sequencia = ((*sequencia + 1) & 0x0F);
    std::string saida_linha = "";
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
    std::string nome_arquivo = "";

    for (int i = 0; i < msg_linha.corpo.tamanho; i++)
        nome_arquivo += msg_linha.dados[i].c;
    // if (nome_arquivo.find(".c") != std::string::npos) {
    //     std::cout << "found!" << '\n';
    // }
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
    Mensagem resposta(0, 0b10, 0b01, 0b1000, *sequencia, NULL); // erro 1
    std::cout << "Enviando:\n\n";
    resposta.printMensagemString();
    if (resposta.enviaMensagem(soquete) == 20)
        *sequencia = ((*sequencia + 1) & 0x0F);
    // resposta.printMensagemString();
    // arquivo.close();
    Mensagem mensagem_linha = resposta.recebeResposta(soquete);
    std::cout << "RESPOSTAAAAAAAAAAAAAAAAAAAAAAAAAAAA2AA\n";
    mensagem_linha.printMensagem();
    uint8_t linha = mensagem_linha.dados[0].num;
    for (int i = 0; i < linha ; i++){
        if (arquivo.eof()){
            std::cout << "acabou o aqruivo :( \n";
            break;
        }
        std::cout << "Oi :)\n";
        std::getline (arquivo, conteudo_arquivo);
        // arquivo = arquivo.get();
    }
    conteudo_arquivo += "\n";
    std::cout << conteudo_arquivo;
    arquivo.close();
    return 0;
    /*
    // else
    // return 0;
    int len = 0;
    uint8_t array_dados[20];
    // envio o conteúdo do arquivo
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
        mensagem.printMensagemString();
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
    */
}