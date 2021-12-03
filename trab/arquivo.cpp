#include "arquivo.h"
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


int respostaVer(uint8_t* sequencia, int soquete, Mensagem msg_ver){    
    std::string nome_arquivo = "";

    for (int i = 0; i < msg_ver.corpo.tamanho; i++)
        nome_arquivo += msg_ver.dados[i].c;
    
    std::ifstream arquivo; 
    arquivo.open(nome_arquivo); 
    // falha ao abrir o arquivo:
    if (arquivo.fail()){
        uint8_t array_dados[] = {3};
        Mensagem resposta(1, 0b10, 0b01, 0b1111, *sequencia, array_dados); // erro 1
        if (resposta.enviaMensagem(soquete) == 20)
            *sequencia = ((*sequencia + 1) & 0x0F);
        return 0;
    }
    else
        return 0;



    // int saida = trocaDir(path);
    // if (saida == -1) {
    //     uint8_t array_dados[] = {1};
    //     Mensagem resposta(1, 0b10, 0b01, 0b1111, *sequencia, array_dados); // erro 1
    //     resposta.enviaMensagem(soquete);
    //     // return 0;
    // } else if (saida == 0){
    //     uint8_t array_dados[] = {2};
    //     Mensagem resposta(1, 0b10, 0b01, 0b1111, *sequencia, array_dados); // erro 2
    //     resposta.enviaMensagem(soquete);
    //     // return 0;
    // } else {
    //     Mensagem resposta(0, 0b10, 0b01, 0b1000, *sequencia, NULL); // ACK
    //     resposta.enviaMensagem(soquete);
    // }

    // *sequencia = ((*sequencia + 1) & 0x0F);

    // return saida;
}


int pedidoVer(uint8_t* sequencia, int soquete, std::string nome_arquivo){
    std::string saida_ver;
    uint8_t array_dados[20];
    // sair se o nome for maior que 15 chars
    if (nome_arquivo.length() > 15)
        return 0;
    
    // passo o nome do arquivo p/ os dados
    for(int i = 0; i < nome_arquivo.length(); i++){
        array_dados[i] = nome_arquivo[i];
    }  

    Mensagem msg_ver(nome_arquivo.length(), 0b01, 0b10, 0b0010, *sequencia, array_dados); // pedido do comando
    msg_ver.printMensagemString();
    // falha no envio da mensagem
    if (msg_ver.enviaMensagem(soquete) < 20)
        return -1;
    Mensagem dados_ver = msg_ver.recebeResposta(soquete);
    // verifica se foi timeout
    if (dados_ver.isEqual(msg_ver) || !dados_ver.verificaParidade()){
        return -1;
    }
    // tenta re-enviar a mensagem em caso de erro
    if (dados_ver.corpo.tipo == 0b1001){
        if (msg_ver.enviaMensagem(soquete) < 20)
            return -1; //give up
        Mensagem dados_ver = msg_ver.recebeResposta(soquete);
        // verifica se foi timeout
        if (dados_ver.isEqual(msg_ver)){
            return -1; //give up
        }
    }
    *sequencia = ((*sequencia + 1) & 0x0F);
    if (dados_ver.corpo.tipo == 0b1111){
        messageError(dados_ver);
        return 0;
    }
    // enquanto tenho mensagem pra ler 
    while(dados_ver.corpo.tipo == 0b1100 && dados_ver.corpo.tamanho != 0){
        // verifico se a mensagem está na ordem esperada e sua paridade
        if (dados_ver.corpo.sequencia == *sequencia && dados_ver.verificaParidade()){ 
            for(int i = 0; i < dados_ver.corpo.tamanho; i++){
                saida_ver += dados_ver.dados[i].c;
            }
            // respondo ACK
            Mensagem resposta(0, 0b01, 0b10, 0b1000, dados_ver.corpo.sequencia, NULL);
            *sequencia = ((*sequencia + 1) & 0x0F);
            if (resposta.enviaMensagem(soquete) < 20)
                return -1;
            else
                dados_ver = resposta.recebeResposta(soquete);
        } else { // mensagem mal formada ou fora de sequência
            Mensagem resposta(0, 0b01, 0b10, 0b1001, dados_ver.corpo.sequencia, NULL); // NACK
            if (resposta.enviaMensagem(soquete) < 20)
                return -1;
            else
                dados_ver = resposta.recebeResposta(soquete);
        }
    }
    std::cout << saida_ver;

    return 1;
}
