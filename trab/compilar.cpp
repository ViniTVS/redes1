#include "compilar.h"

int compila(std::string args){
    std::string comando = "gcc " + args + " 2>&1";
    FILE *fp;
    int status;
    char path[PATH_MAX];


    fp = popen(comando.c_str(), "r");
    if (fp == NULL)
        std::cout << "porra";
        /* Handle error */;


    while (fgets(path, PATH_MAX, fp) != NULL)
        std::cout << path;

    
    status = pclose(fp);

    return status;
}


int respostaCompilar(uint8_t* sequencia, int soquete, Mensagem msg_linha){    
    std::string nome_arquivo = "";
    // *-------------------- leio o nome do arquivo ----------------------------
    for (int i = 0; i < msg_linha.corpo.tamanho; i++)
        nome_arquivo += msg_linha.dados[i].c;
    
    std::fstream arquivo; 
    std::string argumentos;
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
        argumentos += std::to_string(linha) + " " + aux + "\n";
        // arquivo = arquivo.get();
        linha++;
    }
    
    arquivo.close();
    
    // *-------------------- envio o conteúdo do arquivo ----------------------------
    int len = 0;
    uint8_t array_dados[20];
    *sequencia = ((*sequencia + 1) & 0x0F);
    while( len < argumentos.length()){
        int tam_envio = 0;
        for(; len < argumentos.length() && tam_envio < 15; tam_envio++){
            array_dados[tam_envio] = argumentos[len];
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


int pedidoCompilar(uint8_t* sequencia, int soquete, std::string argumentos){
    std::string saida_linha;
    uint8_t array_dados[20];
    // sair se o nome for maior que 15 chars
    // *-------------------------------- envia argumentos -----------------------------------
    int len = 0;
    uint8_t array_dados[20];
    *sequencia = ((*sequencia + 1) & 0x0F);
    while( len < argumentos.length()){
        int tam_envio = 0;
        for(; len < argumentos.length() && tam_envio < 15; tam_envio++){
            array_dados[tam_envio] = argumentos[len];
            len++;
        }  
        // envia a mensagem com os dados
        Mensagem mensagem(tam_envio, 0b01, 0b10, 0b0110, *sequencia, array_dados);
        if (mensagem.enviaMensagem(soquete) < 20)
            return -1;       

        Mensagem resposta = mensagem.recebeResposta(soquete);
        // verifica timeout
        if(resposta.isEqual(mensagem)){
            return -1;     
        }
        
        // manda a mesma mensagem enquanto a resposta for NACK
        while ((resposta.corpo.tipo == 0b1001 && resposta.corpo.sequencia == mensagem.corpo.sequencia) || !resposta.verificaParidade()){
            Mensagem mensagem(tam_envio, 0b01, 0b10, 0b1100, *sequencia, array_dados);

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
    Mensagem mensagem(0, 0b10, 0b01, 0b1101, *sequencia, NULL);
    if (mensagem.enviaMensagem(soquete) < 20)
        return -1;   
    Mensagem resposta = mensagem.recebeResposta(soquete);
    if (resposta.corpo.sequencia == mensagem.corpo.sequencia && resposta.verificaParidade()){
        if (resposta.corpo.tipo == 0b1000){
            return 1;
        }
        while (resposta.corpo.tipo == 0b1001 && resposta.verificaParidade()){
            Mensagem mensagem(0, 0b10, 0b01, 0b1011, *sequencia, NULL);
            if (mensagem.enviaMensagem(soquete) < 20)
                return -1;          // give up
                 
            Mensagem resposta = mensagem.recebeResposta(soquete);
            if(resposta.isEqual(mensagem) || !mensagem.verificaParidade())
                return -1;          // give up
        }
    }
    
}
