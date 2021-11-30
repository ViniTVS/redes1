#include <iostream>
#include <bitset>
#include <string.h>
#include <vector>
#include <sys/socket.h>
#include <poll.h>

#ifndef MENSAGEM
#define MENSAGEM


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
// Como os dados da mensagem possuem valor variado, ficarão sempre armazenados em um vector de char/int de 8bits sem sinal 
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
        int enviaMensagem(int soquete);
        Mensagem recebeResposta(int soquete);
        bool isEqual(Mensagem m1);
        bool verificaParidade();
};


#endif  