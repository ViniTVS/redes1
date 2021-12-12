#include <string.h>
#include <fstream>
#include "error.h"
#include "mensagem.h"

#ifndef LINHA
#define LINHA


/**
 * @brief Função chamada pelo cliente para realizar o pedido "linha" ao servidor para o arquivo <nome_arquivo>, visualizando
 * o conteúdo de sua linha <linha>. 
 * 
 * @param[out] sequencia Valor da sequência das mensagens. Ao final da execução este valor é atuallizado.
 * @param[in] soquete Valor do soquete para envio/recebimento de mensagens.
 * @param[in] linha Número da linha a ser visualizada.
 * @param[in] nome_arquivo Nome do arquivo a ser visualizado.
 * @return int 
 * Caso -1: ocorreu um erro no envio de mensagem ou timeout da resposta.
 * Caso 0: foi recebida uma resposta do tipo erro.
 * Caso 1: o comando foi realizado com sucesso e o conteúdo do arquivo é impresso na tela.
 */
int pedidoLinha(uint8_t* sequencia, int soquete, uint8_t linha, std::string nome_arquivo);

/**
 * @brief Função chamada pelo servidor para realizar o comando "linha" com base nas informações da Mensagem <msg_linha>.
 * 
 * @param[out] sequencia Valor da sequência das mensagens. Ao final da execução este valor é atuallizado.
 * @param[in] soquete Valor do soquete para envio/recebimento de mensagens.
 * @param[in] msg_linha A Mensagem a qual contém o pedido do comando "linha".
 * @return int 
 * Caso -1: ocorreu um erro no envio de mensagem ou timeout da resposta.
 * Caso 0: foi enviada uma resposta do tipo erro.
 * Caso 1: o comando foi realizado com sucesso.
 */
int respostaLinha(uint8_t* sequencia, int soquete, Mensagem msg_linha);

#endif  