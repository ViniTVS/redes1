#include "mensagem.h"
#include "directory.h"

#ifndef LIST
#define LIST

/**
 * @brief Realiza a leitura de arquivos existentes no diretório atual da máquina. 
 * 
 * @return std::string Os arquivos do diretório separados por "\n". 
 */
std::string ls();

/**
 * @brief Função chamada pelo servidor para enviar as mensagens pelo <soquete> contendo os arquivos de seu
 * diretório atual. 
 * 
 * @param[out] sequencia Valor da sequência das mensagens. Ao final da execução este valor é atuallizado.
 * @param[in] soquete Valor do soquete para envio/recebimento de mensagens.
 * 
 * @return int 
 * Caso -1: ocorreu um erro no envio de mensagem ou timeout.
 * Caso 1: o comando foi realizado com sucesso.
 */
int respostaLs(uint8_t *sequencia, int soquete);

/**
 * @brief Função chamada pelo cliente para realizar o comando de listar os arquivos do diretório corrente do servidor.
 * 
 * @param[out] sequencia Valor da sequência das mensagens. Ao final da execução este valor é atuallizado.
 * @param[in] soquete Valor do soquete para envio/recebimento de mensagens.
 * 
 * @return int 
 * Caso -1: ocorreu um erro no envio de mensagem ou timeout da resposta.
 * Caso 0: foi recebida uma resposta do tipo erro.
 * Caso 1: o comando foi realizado com sucesso.
 */
int pedidoLs(uint8_t *sequencia, int soquete);

#endif  