#include <string.h>
#include <fstream>
#include "error.h"
#include "mensagem.h"

#ifndef VER
#define VER

/**
 * @brief Função chamada pelo cliente para realizar o pedido "ver" ao servidor para o arquivo <nome_arquivo>, visualizando
 * o seu conteúdo linha por linha. 
 * 
 * @param[out] sequencia Valor da sequência das mensagens. Ao final da execução este valor é atuallizado.
 * @param[in] soquete Valor do soquete para envio/recebimento de mensagens.
 * @param[in] nome_arquivo Nome do arquivo a ser visualizado.
 * @return int 
 * Caso -1: ocorreu um erro no envio de mensagem ou timeout da resposta.
 * Caso 0: foi recebida uma resposta do tipo erro.
 * Caso 1: o comando foi realizado com sucesso e o conteúdo do arquivo é impresso na tela.
 */
int pedidoVer(uint8_t* sequencia, int soquete, std::string nome_arquivo);

/**
 * @brief Função chamada pelo servidor para realizar o comando "ver" com base nas informações da Mensagem <msg_ver>.
 * 
 * @param[out] sequencia Valor da sequência das mensagens. Ao final da execução este valor é atuallizado.
 * @param[in] soquete Valor do soquete para envio/recebimento de mensagens.
 * @param[in] msg_ver A Mensagem a qual contém o pedido do comando "ver".
 * @return int 
 * Caso -1: ocorreu um erro no envio de mensagem ou timeout da resposta.
 * Caso 0: foi enviada uma resposta do tipo erro.
 * Caso 1: o comando foi realizado com sucesso.
 */
int respostaVer(uint8_t* sequencia, int soquete, Mensagem msg_ver);

#endif  
