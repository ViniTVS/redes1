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

    /**
     * @brief Envia a mensagem utilizando a função send da socket.h via <soquete>.
     * O método adapta o objeto do tipo Mensagem para um array de 20 inteiros de 8 bits sem sinal. Os Bytes iniciais sendo
     * transmitidos possuem os dados em si, respeitando o padrão do protocolo especificado pelo trabalho (podendo 
     * variar entre 4 e 19 Bytes, dependendo do tamanho do campo de dados da Mensagem), enquanto o restante possue 
     * valores não iniciados. 
     * 
     * @param soquete O valor do soquete para enviar a mensagem.
     * 
     * @return int Retorna o número de bytes enviados via soquete.
     */
    int enviaMensagem(int soquete);

    /**
     * @brief Recebe a mensagem utilizando a função recv da socket.h via <soquete>.
     * O método faz a leitura dos dados presentes no soquete até encontrar a Mensagem utilizada para a chamada
     * do método. Após isso é verificada a existência de uma Mensagem seguinte disponível para leitura com um timeout
     * de 3 segundos. Caso exista uma nova mensagem, esta será enviada. Caso contrário, será enviada a mensagem utilizada
     * na chamada deste método. 
     * 
     * @param soquete O valor do soquete para se fazer leitura de dados.
     * 
     * @return Mensagem 
     * Caso exista, uma nova Mensagem, seguinte da atual.
     * Caso contrário, a Mensagem utilizada para a chamada deste método.
     */
    Mensagem recebeResposta(int soquete);

    /**
     * @brief Verifica se a Mensagem utilizada para a chamada do método possui as mesmas informações da Mensagem <m>.
     * 
     * @param m Mensagem para se comparar.
     * @return true Caso possuam as mesmas informações.
     * @return false Caso não possuam as mesmas informações.
     */
    bool isEqual(Mensagem m);

    /**
     * @brief Verifica se a Mensagem possui seu campo de paridade conforme esperado.
     * 
     * @return true Caso seu campo de paridade esteja ìntegro.
     * @return false Caso contrário.
     */
    bool verificaParidade();
};


#endif  