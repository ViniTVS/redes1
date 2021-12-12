#include <string.h>
#include <fstream>
#include "error.h"
#include "mensagem.h"

#ifndef LINHAS
#define LINHAS


/**
 * @brief Função chamada pelo cliente para realizar o pedido "linhas" ao servidor para o arquivo <nome_arquivo>, visualizando
 * o conteúdo de suas linhas entre <linha_inical> e <linha_final>. 
 * 
 * @param[out] sequencia Valor da sequência das mensagens. Ao final da execução este valor é atuallizado.
 * @param[in] soquete Valor do soquete para envio/recebimento de mensagens.
 * @param[in] linha_inical Número da primeira linha a ser visualizada.
 * @param[in] linha_final Número da última linha a ser visualizada.
 * @param[in] nome_arquivo Nome do arquivo a ser visualizado.
 * @return int 
 * Caso -1: ocorreu um erro no envio de mensagem ou timeout da resposta.
 * Caso 0: foi recebida uma resposta do tipo erro.
 * Caso 1: o comando foi realizado com sucesso e o conteúdo do arquivo é impresso na tela.
 */
int pedidoLinhas(uint8_t* sequencia, int soquete, uint8_t linha_inical, uint8_t linha_final, std::string nome_arquivo);

/**
 * @brief Função chamada pelo servidor para realizar o comando "linhas" com base nas informações da Mensagem <msg_linhas>.
 * 
 * @param[out] sequencia Valor da sequência das mensagens. Ao final da execução este valor é atuallizado.
 * @param[in] soquete Valor do soquete para envio/recebimento de mensagens.
 * @param[in] msg_linhas A Mensagem a qual contém o pedido do comando "linhas".
 * @return int 
 * Caso -1: ocorreu um erro no envio de mensagem ou timeout da resposta.
 * Caso 0: foi enviada uma resposta do tipo erro.
 * Caso 1: o comando foi realizado com sucesso.
 */
int respostaLinhas(uint8_t* sequencia, int soquete, Mensagem msg_linhas);

#endif  