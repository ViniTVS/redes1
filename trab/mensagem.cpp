#include "mensagem.h"

Mensagem::Mensagem(uint8_t tamanho_in, uint8_t origem_in, uint8_t destino_in, uint8_t tipo_in, uint8_t sequencia_in, uint8_t *array_dados){
    // Copia os primeiros 3 bytes da mensagem (que sempre serão usados)
    corpo.marcador = 0b01111110;
    corpo.tamanho = tamanho_in;
    corpo.origem = origem_in;
    corpo.destino = destino_in;
    corpo.tipo = tipo_in;
    corpo.sequencia = sequencia_in;
    uint8_t parity =  tamanho_in ^ sequencia_in ^ tipo_in ; 

    dados.clear();
    DadoMensagem aux;
    for (int i = 0; i < unsigned(corpo.tamanho); i++){
        aux.num = array_dados[i];
        dados.push_back(aux);
        parity = parity ^ array_dados[i];
    }
    corpo.paridade = parity;
}

Mensagem::Mensagem(uint8_t *array_bruto){
    // Copia os primeiros 3 bytes da mensagem (que sempre serão usados)
    memcpy ( &corpo.binario, array_bruto, 3);    
    // copia os dados da mensagem
    DadoMensagem aux;
    int i = 0;
    for ( ; i < unsigned(corpo.tamanho); i++){   
        memcpy ( &aux, &array_bruto[i+3], 1); 
        dados.push_back(aux);
    }
    // copia a paridade
    memcpy ( &corpo.paridade, &array_bruto[3+unsigned(corpo.tamanho)], 1);
    
}

void Mensagem::printMensagem(){
    std::cout << "Marcador: " << std::bitset<8>(corpo.marcador) << std::endl;
    std::cout << "Destino: " << std::bitset<2>(corpo.destino) << std::endl;
    std::cout << "Origem: " << std::bitset<2>(corpo.origem) << std::endl;
    std::cout << "Tamanho: " << unsigned(corpo.tamanho) << std::endl;
    std::cout << "Sequência: " << unsigned(corpo.sequencia) << std::endl;
    std::cout << "Tipo: " << std::bitset<4>(corpo.tipo) << std::endl;
    std::cout << "Paridade: " << std::bitset<8>(corpo.paridade) << std::endl;
    std::cout << "Dados: " << std::endl;
    for (int i = 0; i < dados.size(); i++)
        std::cout << unsigned(dados[i].num) << " ";
    std::cout << std::endl;
}

void Mensagem::printMensagemString(){
    std::cout << "Marcador: " << std::bitset<8>(corpo.marcador) << std::endl;
    std::cout << "Destino: " << std::bitset<2>(corpo.destino) << std::endl;
    std::cout << "Origem: " << std::bitset<2>(corpo.origem) << std::endl;
    std::cout << "Tamanho: " << unsigned(corpo.tamanho) << std::endl;
    std::cout << "Sequência: " << unsigned(corpo.sequencia) << std::endl;
    std::cout << "Tipo: " << std::bitset<4>(corpo.tipo) << std::endl;
    std::cout << "Paridade: " << std::bitset<8>(corpo.paridade) << std::endl;
    std::cout << "Dados: " << std::endl;
    for (int i = 0; i < dados.size(); i++)
        std::cout << dados[i].c;
    std::cout << std::endl;
}

int Mensagem::enviaMensagem(int soquete){
    uint8_t mensagem_bruta[20];
    mensagem_bruta[0] = corpo.marcador;
    mensagem_bruta[1] = (corpo.destino << 6) | (corpo.origem << 4) | (corpo.tamanho);
    mensagem_bruta[2] = (corpo.sequencia << 4) | (corpo.tipo);
    int i = 0;
    for( ; i < corpo.tamanho; i++){
        mensagem_bruta[i + 3] = dados[i].num;
    }
    mensagem_bruta[i + 3] = corpo.paridade;
    
    int saida = send(soquete, &mensagem_bruta, 20, 0);
    return saida;
}

Mensagem Mensagem::recebeResposta(int soquete){
    int tentativas = 0;
    uint8_t buffer[20];
    int tam_msg = 0;
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    int timeout = 0;
    Mensagem resposta(buffer);
    for (; timeout == 0 && !resposta.isEqual(*this); ){
        timeout = setsockopt(soquete, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
        if (timeout == 0)
            tam_msg = recv(soquete, &buffer, 20, 0);
        if(tam_msg < 20)
            return *this;
        resposta = Mensagem(buffer);
    }  
    // ler a mensagem seguinte da atual 
    timeout = setsockopt(soquete, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    if (timeout == 0){
        tam_msg = recv(soquete, &buffer, 20, 0);
        if(tam_msg < 20)
            return *this;
        Mensagem aux(buffer);
        resposta = aux;
        
    }
    return resposta;
    
}

bool Mensagem::isEqual(Mensagem m){
    if (m.corpo.binario != corpo.binario)
        return false;
        
    for (int i = 0; i < dados.size(); i++){
        if (dados[i].c != m.dados[i].c)
            return false;
    }

    return true;
}

bool Mensagem::verificaParidade(){
    
    uint8_t parity =  corpo.tamanho ^ corpo.sequencia ^ corpo.tipo ;
    
    for (int i = 0 ; i < unsigned(corpo.tamanho); i++){
        parity = parity ^ dados[i].num;
    }
    
    return (parity == corpo.paridade);
}