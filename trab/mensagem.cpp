#include <iostream>
#include <bitset>
#include <string.h>
#include <vector>

// using namespace std;

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
        uint8_t marcador;
        CorpoMensagem corpo;
        std::vector <DadoMensagem> dados;
        uint8_t paridade;

    public:
        Mensagem(uint8_t *array_bruto);
        // ~Mensagem();
        void printMensagem();
};

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
