#include <iostream>
#include <bitset>
#include <string.h>

using namespace std;

// Estrutura do CorpoMensagem:
// ┌────────────────────┬──────────┬──────────┬──────────────┬──────────────┬──────────────┬────────────────────┐
// │      marcador      │  destino │  origem  │    tamanho   │  sequencia   │     tipo     │      paridade      │
// │      (8 bits)      │ (2 bits) │ (2 bits) │   (4 bits)   │   (4 bits)   │   (4 bits)   │      (8 bits)      │  
// └────────────────────┴──────────┴──────────┴──────────────┴──────────────┴──────────────┴────────────────────┘ 

union CorpoMensagem {
    struct { 
        uint8_t paridade;
        uint8_t tipo :4;
        uint8_t sequencia :4;
        uint8_t tamanho :4;
        uint8_t origem :2;
        uint8_t destino :2;
        uint8_t marcador;
    };
    uint32_t binario;
};
// 
// Como os dados da mensagem possuem valor variado, ficarão sempre armazenados em um array de char 
union DadoMensagem{
    uint8_t num;
    char c;
};

int main(){
    CorpoMensagem msg;
    // msg.binario = 0b011111100110001100011110;
    uint64_t teste = 0b0111111001100100000111100011000001001000011101010000000010011100;
    std::cout << "teste: 0b" << std::bitset<64>(teste) << std::endl;
    // corpo da mensagem passa a ter os primeiros 24 bits da mensagem
    msg.binario = (teste >> (sizeof(teste)*8 - 24));
    msg.binario = (msg.binario << 8);
    // separo os 8 últimos bits da mensagem
    uint8_t aux = teste;
    // e os adiciono no corpo da mensagem
    msg.binario = (msg.binario | aux);
    std::cout << "binario: 0b" << std::bitset<32>(msg.binario) << std::endl << std::endl;

    std::cout << "Marcador: " << std::bitset<8>(msg.marcador) << std::endl;
    std::cout << "Destino: " << std::bitset<2>(msg.destino) << std::endl;
    std::cout << "Origem: " << std::bitset<2>(msg.origem) << std::endl;
    std::cout << "Tamanho: " << int( msg.tamanho) << std::endl;
    std::cout << "Sequência: " << std::bitset<4>(msg.sequencia) << std::endl;
    std::cout << "Tipo: " << std::bitset<4>(msg.tipo) << std::endl;
    std::cout << "Paridade: " << std::bitset<8>(msg.paridade) << std::endl;

    int tam = int( msg.tamanho);
    DadoMensagem array[tam];
    // copiar os dados da mensagem no campo 
    for (int i = tam - 1; i >= 0; i--){
        teste = teste >> 8;
        memcpy( &array[i], &teste, sizeof(char) );
    }

    // mensagem: 00110000 01001000 01110101 0000000
    //                  0        H        u    NULL
    for (int i = 0; i < tam; i++){
        std::cout << "int: " << std::bitset<8>(array[i].num) << "\tASCII: " << array[i].car << std::endl;
    }

    return 0;
}

// class Mensagem
// {
// private:
//     /* data */
//     CorpoMensagem msg;
// public:
//     Mensagem(/* args */);
//     ~Mensagem();
// };

// Mensagem::Mensagem(/* args */)
// {
// }

// Mensagem::~Mensagem()
// {
// }
