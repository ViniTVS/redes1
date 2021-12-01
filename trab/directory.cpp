#include "directory.h"
#include "error.h"

int trocaDir(std::string nome_dir){
    std::string novo_dir = "";
    // verifica se a entrada é um possível caminho diretório
    if(nome_dir[0] == '/')
        novo_dir = nome_dir;
    else if(nome_dir == "..") // se quer voltar 1 dir
        novo_dir = fs::current_path().parent_path().string(); 
    else // ou cd em um dir do local atual
        novo_dir = fs::current_path().string() + "/" + nome_dir ; 
    
    if(fs::is_directory(novo_dir)){
        fs::current_path(novo_dir);
        if (fs::current_path().string() != novo_dir) // não foi possível acessar o novo diretório
            return -1;
    }
    else 
        return 0;
        
    return 1;
}

int pedidoCd(uint8_t *sequencia, int soquete, std::string nome_dir){
    int len = nome_dir.length();
    if (len > 15)
        return -1;

    uint8_t msg_path[len];
    for (int i = 0; i < len ; i++){
        msg_path[i] = nome_dir[i];
    }
    
    Mensagem msg_cd(len, 0b01, 0b10, 0b0000, *sequencia, msg_path); // pedido do comando
    if (msg_cd.enviaMensagem(soquete) < 20)
        return (-1);
    Mensagem resposta = msg_cd.recebeResposta(soquete);
    // verifica se foi timeout
    if (resposta.isEqual(msg_cd)){
        std::cout << "Server timeout\n";
        return -1;
    }
    // tento enviar a mensagem novamente se for NACK
    while (resposta.corpo.tipo == 0b1001 && resposta.corpo.sequencia == *sequencia && resposta.verificaParidade()){
        if (msg_cd.enviaMensagem(soquete) < 20)
            return (-1);
        resposta = msg_cd.recebeResposta(soquete);
    }
    // trata ACK
    if(resposta.corpo.tipo == 0b1000 && resposta.verificaParidade() && resposta.corpo.sequencia == *sequencia){
        *sequencia = ((*sequencia + 1) & 0x0F);
        return 1;
    }
    // trata Erro
    else if (resposta.corpo.tipo == 0b1111 && resposta.corpo.sequencia == *sequencia){
        messageError(resposta);
        *sequencia = ((*sequencia + 1) & 0x0F);
        return 0;
    }
    *sequencia = ((*sequencia + 1) & 0x0F);
    return -1;
}

int respostaCd(uint8_t* sequencia, int soquete, Mensagem msg_cd){    
    std::string path = "";

    for (int i = 0; i < msg_cd.corpo.tamanho; i++){
        path += msg_cd.dados[i].c;
    }

    int saida = trocaDir(path);
    if (saida == -1) {
        uint8_t array_dados[] = {1};
        Mensagem resposta(1, 0b10, 0b01, 0b1111, *sequencia, array_dados); // erro 1
        resposta.enviaMensagem(soquete);
        // return 0;
    } else if (saida == 0){
        uint8_t array_dados[] = {2};
        Mensagem resposta(1, 0b10, 0b01, 0b1111, *sequencia, array_dados); // erro 2
        resposta.enviaMensagem(soquete);
        // return 0;
    } else {
        Mensagem resposta(0, 0b10, 0b01, 0b1000, *sequencia, NULL); // ACK
        resposta.enviaMensagem(soquete);
    }

    *sequencia = ((*sequencia + 1) & 0x0F);

    return saida;
}
