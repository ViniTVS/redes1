#include <string.h>
#include <fstream>
#include "error.h"
#include "mensagem.h"

#ifndef EDIT
#define EDIT

/**
 * @brief Função chamada pelo cliente para realizar o pedido de edição "edit" ao servidor para o arquivo <nome_arquivo>, substituindo
 * o conteúdo de sua linha <linha> para o <novo_texto>. 
 * 
 * @param[out] sequencia Valor da sequência das mensagens. Ao final da execução este valor é atuallizado.
 * @param[in] soquete Valor do soquete para envio/recebimento de mensagens.
 * @param[in] linha Número da linha a ser editada.
 * @param[in] nome_arquivo Nome do arquivo a ser editado.
 * @param[in] novo_texto Texto para ser adicionado ao arquivo desejado.
 * @return int 
 * Caso -1: ocorreu um erro no envio de mensagem ou timeout da resposta.
 * Caso 0: foi recebida uma resposta do tipo erro.
 * Caso 1: o comando foi realizado com sucesso.
 */
int pedidoEdit(uint8_t* sequencia, int soquete, uint8_t linha, std::string nome_arquivo, std::string novo_texto);

/**
 * @brief Função chamada pelo servidor para realizar o comando de editar edição "edit" com base nas informações do
 * Mensagem <msg_edit>.
 * 
 * @param[out] sequencia Valor da sequência das mensagens. Ao final da execução este valor é atuallizado.
 * @param[in] soquete Valor do soquete para envio/recebimento de mensagens.
 * @param[in] msg_edit A Mensagem a qual contém o pedido do comando "edit".
 * @return int 
 * Caso -1: ocorreu um erro no envio de mensagem ou timeout da resposta.
 * Caso 0: foi enviada uma resposta do tipo erro.
 * Caso 1: o comando foi realizado com sucesso.
 */
int respostaEdit(uint8_t* sequencia, int soquete, Mensagem msg_edit);

#endif  