#include "edit.h"

int pedidoEdit(uint8_t* sequencia, int soquete, uint8_t linha, std::string nome_arquivo, std::string novo_texto){
    // cria mensagem contendo o número da linha
    uint8_t array_dados[20];
    if (nome_arquivo.length() > 15)
        return -1;
    for(int i = 0; i < nome_arquivo.length(); i++){
        array_dados[i] = nome_arquivo[i];
    }
    // *------------------------- envio do nome do arquivo --------------------------------- 
    Mensagem msg_edit(nome_arquivo.length(), 0b01, 0b10, 0b0101, *sequencia, array_dados); // pedido do comando
    // falha no envio da mensagem
    if (msg_edit.enviaMensagem(soquete) < 20)
        return -1;

    Mensagem dados_linha = msg_edit.recebeResposta(soquete);
    // verifica se foi timeout
    if (dados_linha.isEqual(msg_edit)){
        return -1;
    }
    // tenta re-enviar a mensagem em caso de erro
    if (dados_linha.corpo.tipo == 0b1001 || !dados_linha.verificaParidade()){
        if (msg_edit.enviaMensagem(soquete) < 20)
            return -1; //give up
        Mensagem dados_linha = msg_edit.recebeResposta(soquete);
        // verifica se foi timeout
        if (dados_linha.isEqual(msg_edit)){
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
    Mensagem msg_edit_num(1, 0b01, 0b10, 0b1010, *sequencia, array_dados); // envio do número da linha

    if (msg_edit_num.enviaMensagem(soquete) < 20)
        return -1; //give up
    
    Mensagem resp_linha = msg_edit_num.recebeResposta(soquete);
    // verifica se foi timeout
    if (resp_linha.isEqual(msg_edit_num))
        return -1;
    
    // tenta re-enviar a mensagem em caso de erro
    if (resp_linha.corpo.tipo == 0b1001 || !resp_linha.verificaParidade()){
        if (msg_edit.enviaMensagem(soquete) < 20)
            return -1; //give up
        Mensagem resp_linha = msg_edit.recebeResposta(soquete);
        // verifica se foi timeout
        if (resp_linha.isEqual(msg_edit)){
            return -1; //give up
        }
    }
    else if (resp_linha.corpo.tipo == 0b1111 && resp_linha.verificaParidade()){
        messageError(resp_linha);
        *sequencia = ((*sequencia + 1) & 0x0F);
        return 0;
    } else if (resp_linha.corpo.tipo != 0b1000 || !resp_linha.verificaParidade()){
        return -1;
    }
    // recebeu ACK
    *sequencia = ((*sequencia + 1) & 0x0F);

    // *---------------------------- envio o conteúdo da linha --------------------------------- 
    int len = 0;
    while( len < novo_texto.length()){
        int tam_envio = 0;
        for(; len < novo_texto.length() && tam_envio < 15; tam_envio++){
            array_dados[tam_envio] = novo_texto[len];
            len++;
        }  
        // envia a mensagem com os dados
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

        
        *sequencia = ((*sequencia + 1) & 0x0F);
    }

    // final de envio
    Mensagem mensagem(0, 0b10, 0b01, 0b1101, *sequencia, NULL);
    if (mensagem.enviaMensagem(soquete) < 20)
        return -1;   
    Mensagem resposta = mensagem.recebeResposta(soquete);
    if (resposta.corpo.sequencia == mensagem.corpo.sequencia && resposta.verificaParidade()){
        if (resposta.corpo.tipo == 0b1000){
            *sequencia = ((*sequencia + 1) & 0x0F);
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


int respostaEdit(uint8_t* sequencia, int soquete, Mensagem msg_edit){
    // *------------------------- leio o nome do arquivo --------------------------------- 
    std::string nome_arquivo = "";

    for (int i = 0; i < msg_edit.corpo.tamanho; i++)
        nome_arquivo += msg_edit.dados[i].c;
        
    std::fstream arquivo;
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
    while (!mensagem_linha.verificaParidade()){
        Mensagem nack(0, 0b01, 0b10, 0b1001, *sequencia, NULL); // NACK
        mensagem_linha = nack.recebeResposta(soquete);
    }
    if (mensagem_linha.isEqual(resposta_ACK))
        return -1;
    uint8_t linha = mensagem_linha.dados[0].num;
    std::string conteudo_arquivo;
    // mantenho uma variavel p/ armazenar o conteúdo da linha a see substiuída

    std::string resto_arquivo = "\n";
    std::string aux;
    bool nova_linha = false;
    for (int i = 0; i <  (linha - 1) ; i++){
        if (arquivo.eof()){            
            if (i == (linha - 2)){
                nova_linha = true;    
                break;
            }
            arquivo.close();
            uint8_t array_dados[] = {4};
            Mensagem erro_linha(1, 0b10, 0b01, 0b1111, *sequencia, array_dados); // erro 4
            if (erro_linha.enviaMensagem(soquete) == 20){
                *sequencia = ((*sequencia + 1) & 0x0F);
                return 0;
            }
            return -1; // erro no envio
        }
        std::getline (arquivo, aux);
        conteudo_arquivo =  conteudo_arquivo + aux + "\n";
    }
    std::getline (arquivo, aux);
    // copia o resto do arquivo
    while (!arquivo.eof()){
        std::getline (arquivo, aux);
        resto_arquivo = resto_arquivo + aux + "\n";
    }
    arquivo.close(); // fecho o arquivo 
    Mensagem ack(0, 0b01, 0b10, 0b1000, *sequencia, NULL); // ACK
    if (ack.enviaMensagem(soquete) != 20)
        return -1;

    // novo_texto += "\n";
    Mensagem dado_nova_linha = ack.recebeResposta(soquete);
    *sequencia = ((*sequencia + 1) & 0x0F);
    if (mensagem_linha.isEqual(resposta_ACK))
        return -1;
    // *------------------------- leitura da nova linha do arquivo --------------------------------- 
    std::string novo_texto;
    while(dado_nova_linha.corpo.tipo == 0b1100 && dado_nova_linha.corpo.tamanho != 0){
        // verifico se a mensagem está na ordem esperada e sua paridade
        if (dado_nova_linha.corpo.sequencia == *sequencia && dado_nova_linha.verificaParidade()){ 
            for(int i = 0; i < dado_nova_linha.corpo.tamanho; i++){
                novo_texto = novo_texto + dado_nova_linha.dados[i].c;
            }
            // respondo ACK
            Mensagem resposta(0, 0b01, 0b10, 0b1000, dado_nova_linha.corpo.sequencia, NULL);
            if (resposta.enviaMensagem(soquete) < 20)
                return -1;
            else{
                *sequencia = ((*sequencia + 1) & 0x0F);
                dado_nova_linha = resposta.recebeResposta(soquete);
            }
        } else { // mensagem mal formada ou fora de sequência
            Mensagem resposta(0, 0b01, 0b10, 0b1001, dado_nova_linha.corpo.sequencia, NULL); // NACK
            if (resposta.enviaMensagem(soquete) < 20)
                return -1;
            else
                dado_nova_linha = resposta.recebeResposta(soquete);
        }
    }
    if (dado_nova_linha.corpo.sequencia == *sequencia && dado_nova_linha.verificaParidade() && dado_nova_linha.corpo.tipo == 0b1101){
            // respondo ACK
        Mensagem resposta(0, 0b01, 0b10, 0b1000, dado_nova_linha.corpo.sequencia, NULL);
        if (resposta.enviaMensagem(soquete) < 20)
            return -1;
        else
            *sequencia = ((*sequencia + 1) & 0x0F);
    }
    if (nova_linha)
        conteudo_arquivo += novo_texto + "\n";
    else {
        // conteudo_arquivo.replace(conteudo_arquivo.find(linha_troca), linha_troca.length(), novo_texto);
        conteudo_arquivo = conteudo_arquivo + novo_texto + resto_arquivo.substr(0, resto_arquivo.length() - 1); // retiro o \n desnecessário
    }

    std::ofstream escrita_arquivo(nome_arquivo); // o abro pra escrita
    escrita_arquivo << conteudo_arquivo;
    escrita_arquivo.close(); // fecho o arquivo 
    
    return 1;
    
}