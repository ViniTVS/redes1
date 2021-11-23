#include <iostream>
#include <bitset>
#include <string.h>
#include <vector>

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
// Como os dados da mensagem possuem valor variado, ficarão sempre armazenados em um array de char 
union DadoMensagem{
    uint8_t num;
    char c;
};

class Mensagem{
    private:
        CorpoMensagem corpo;
        std::vector <DadoMensagem> dados;
    public:
        Mensagem(uint8_t *array_bruto);
        // ~Mensagem();
        void printMensagem();
        void printMensagemString();
        uint8_t getTipo();
        uint8_t getSequencia();
};

// Mensagem::Mensagem(uint8_t *array_bruto, int tam_array){
//     memcpy ( &corpo, array_bruto, 3);
//     int i = 0;
//     for ( ; i < unsigned(corpo.tamanho); i++){        
//         dados.push_back(array_bruto[3+i]);
//     }
//     i++;
//     memcpy ( &corpo.paridade, &array_bruto[3+i], 1);
//     std::cout << "Mensagem recebida: \n";
//     for (int i = 0; i < tam_array; i++){        
//         std::cout << "\t" << unsigned(array_bruto[i]) << "\n";
//     }
// }

// Mensagem::~Mensagem(){
// }




// void printMensagem(){

// }


#endif  