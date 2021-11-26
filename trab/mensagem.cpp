#include <iostream>
#include <bitset>
#include <string.h>
#include <vector>
#include <sys/socket.h>
#include <poll.h>

// Estrutura do CorpoMensagem:
// ┌────────────────────┬──────────┬──────────┬──────────────┬──────────────┬──────────────┬────────────────────┐
// │      marcador      │  destino │  origem  │    tamanho   │  sequencia   │     tipo     │      paridade      │
// │      (8 bits)      │ (2 bits) │ (2 bits) │   (4 bits)   │   (4 bits)   │   (4 bits)   │      (8 bits)      │  
// └────────────────────┴──────────┴──────────┴──────────────┴──────────────┴──────────────┴────────────────────┘ 

union CorpoMensagem {
    struct { 
        uint8_t marcador;
        uint8_t tamanho :4;
        uint8_t origem :2;
        uint8_t destino :2;
        uint8_t tipo :4;
        uint8_t sequencia :4;
        uint8_t paridade;
    };
    uint32_t binario;
};
// 
// Como os dados da mensagem possuem valor variado, ficarão sempre armazenados em um array de char 
union DadoMensagem{
    uint8_t num;
    char c;
};

class Mensagem{
    public:
        CorpoMensagem corpo;
        std::vector <DadoMensagem> dados;

    public:
        Mensagem(uint8_t *array_bruto);
        Mensagem(uint8_t tamanho_in, uint8_t origem_in, uint8_t destino_in, uint8_t tipo_in, uint8_t sequencia_in, uint8_t *array_dados);
        // ~Mensagem();
        void printMensagem();
        void printMensagemString();
        uint8_t getTipo();
        uint8_t getSequencia();
        int enviaMensagem(int soquete);
        Mensagem recebeResposta(int soquete);
        bool isEqual(Mensagem m1);
};

Mensagem::Mensagem(uint8_t tamanho_in, uint8_t origem_in, uint8_t destino_in, uint8_t tipo_in, uint8_t sequencia_in, uint8_t *array_dados){
    // Copia os primeiros 3 bytes da mensagem (que sempre serão usados)
    corpo.marcador = 0b01111110;
    corpo.tamanho = tamanho_in;
    corpo.origem = origem_in;
    corpo.destino = destino_in;
    corpo.tipo = tipo_in;
    corpo.sequencia = sequencia_in;
    corpo.paridade = 0b10101010;

    dados.clear();
    DadoMensagem aux;
    for (int i = 0; i < unsigned(corpo.tamanho); i++){
        aux.num = array_dados[i];
        dados.push_back(aux);
    }
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
        std::cout << std::bitset<8>(dados[i].num) << " ";
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

uint8_t Mensagem::getTipo(){
    return corpo.tipo;
}

uint8_t Mensagem::getSequencia(){
    return corpo.sequencia;
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
    mensagem_bruta[i + 4] = corpo.paridade;
    
    int saida =send(soquete, &mensagem_bruta, 20, 0);
    return saida;
}

Mensagem Mensagem::recebeResposta(int soquete){
    int tentativas = 0;
    uint8_t buffer[20];
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 30000;
    // fazer leitura das mensagens até encontrar a atual
    int timeout = setsockopt(soquete, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    if(timeout == 0){
        recv(soquete, &buffer, 20, 0);
        Mensagem resposta(buffer);
        
        while (!this->isEqual(resposta)){
            timeout = setsockopt(soquete, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
            if (timeout != 0){
                break;
            }

            recv(soquete, &buffer, 20, 0);
            Mensagem resposta(buffer);
        // fazer 5 tentativas de timeout 
        }

    }  
    // ler a mensagem seguinte da atual em 5 tentativas
    while(tentativas < 5){
        timeout = setsockopt(soquete, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
        if(timeout == 0){
            recv(soquete, &buffer, 20, 0);
            Mensagem resposta(buffer);

            if (resposta.corpo.destino == corpo.origem && resposta.corpo.marcador == 0b01111110 /*&& resposta.verificaParidade()*/)
                return resposta;
        }
        tentativas++;
    }
    
    return *this;
}
// int Mensagem::enviaMensagem(){

// }

bool Mensagem::isEqual(Mensagem m1){
    if (m1.corpo.binario != corpo.binario)
        return false;
        
    for (int i = 0; i < dados.size(); i++){
        if (dados[i].c != m1.dados[i].c)
            return false;
    }

    return true;
}