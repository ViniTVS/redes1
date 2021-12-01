#include <iostream>
#include <bitset>
#include <string.h>
#include <vector>
#include <sys/socket.h>
#include <poll.h>

#ifndef MENSAGEM
#define MENSAGEM


// ┌────────────────────┬──────────┬──────────┬──────────────┬──────────────┬──────────────┬────────────────────┐
// │      marcador      │  destino │  origem  │    tamanho   │  sequencia   │     tipo     │      paridade      │
// │      (8 bits)      │ (2 bits) │ (2 bits) │   (4 bits)   │   (4 bits)   │   (4 bits)   │      (8 bits)      │  
// └────────────────────┴──────────┴──────────┴──────────────┴──────────────┴──────────────┴────────────────────┘ 

/**
 * @brief Union para armazenar os dados do marcador, destino, tamanho, sequencia, tipo e paridade em seus tamamhos
 * especificados pelo protocolo do trabalho utilizados na classe Mensagem.
 * 
 */
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
/**
 * @brief Union para representar os dados presentes na Mensagem tanto como inteiros de 8bits sem sinal 
 * quanto como caracteres.
 */
union DadoMensagem{
    uint8_t num;
    char c;
};

/**
 * @brief Classe para representar as mensagens de dados tranmitidas via socket utilizando o protocolo especificado pelo
 * trabalho. 
 * 
 * Por
 */
class Mensagem{
// declaração do conteúdo da classe
  public:
    
    CorpoMensagem corpo;
    // Como os dados possuem tamanho variado, eles serão representados como um vetor
    std::vector <DadoMensagem> dados;

// declaração de métodos da classe
  public:
    /**
     * @brief Construtor de um novo objeto Mensagem utilizando dados recebidos via leitura do socket.
     * 
     * @param array_bruto Dados recebidos via socket contendo o conteúdo da mensagem.
     */
    Mensagem(uint8_t *array_bruto);

    /**
     * @brief Construtor de um novo objeto Mensagem com os valores de seus campos já definidos.
     * Os valores de seus campos são os parâmetros declarados em sua chamada e a paridade é calculada
     * efetuando um xor byte a byte dos campos tamanho, sequência, tipo e cada um dos dados.   
     * 
     * @param tamanho_in Valor do tamanho do campo de dados.
     * @param origem_in Origem da mensagem (deve ser 0b01 ou 0b10).
     * @param destino_in Destino da mensagem (deve ser 0b01 ou 0b10).
     * @param tipo_in Tipo da mensagem (deve respeitar os tipos definidos no protocolo).
     * @param sequencia_in A sequência a qual a mensagem deve ter baseada no protocolo.
     * @param array_dados Um array contendo os dados que devem ser passados para o campo de dados.
     */
    Mensagem(uint8_t tamanho_in, uint8_t origem_in, uint8_t destino_in, uint8_t tipo_in, uint8_t sequencia_in, uint8_t *array_dados);

    void printMensagem();
    void printMensagemString();
    int enviaMensagem(int soquete);
    Mensagem recebeResposta(int soquete);
    bool isEqual(Mensagem m1);
    bool verificaParidade();
};


#endif  