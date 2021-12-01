#include <filesystem>
#include "mensagem.h"

#ifndef DIRECTORY
#define DIRECTORY
namespace fs = std::filesystem;

/**
 * @brief Realiza a troca de diretórios da máquina para <nomde_dir>. Caso não ocorra, permanece
 * no diretório atual.
 * 
 * @param nome_dir Diretório que se deseja acessar. Caso seja ".." o programa tenta acessar o diretório pai.
 * 
 * @return int 
 * Caso -1: erro no acesso do diretório desejado.
 * Caso 0: diretório que deseja acessa não existe.
 * Caso 1: a troca foi realizada com sucesso.
 */
int trocaDir(std::string nome_dir);

/**
 * @brief Função chamada pelo cliente para realizar o comando de troca de diretórios no servidor para <nomde_dir> 
 * de tamanho máximo de 15 caracteres. 
 * Caso não ocorra, permanece no diretório atual. 
 * 
 * @param[out] sequencia Valor da sequência das mensagens. Ao final da execução este valor é atuallizado.
 * @param[in] soquete Valor do soquete para envio/recebimento de mensagens.
 * @param[in] nome_dir Diretório que se deseja acessar. Caso seja ".." o programa tenta acessar o diretório pai.
 * 
 * @return int 
 * Caso -1: ocorreu um erro no envio de mensagem ou timeout da resposta.
 * Caso 0: foi recebida uma resposta do tipo erro.
 * Caso 1: o comando foi realizado com sucesso.
 */
int pedidoCd(uint8_t *sequencia, int soquete, std::string nome_dir);

/**
 * @brief Função chamada pelo servidor para realizar o comando de troca de diretórios e responder 
 * a mensagem <msg_cd> de pedido de troca de diretório.
 * 
 * @param[out] sequencia Valor da sequência das mensagens. Ao final da execução este valor é atuallizado.
 * @param[in] soquete Valor do soquete para envio/recebimento de mensagens.
 * @param[in] nome_dir Diretório que se deseja acessar.
 * 
 * @return int 
 * Caso -1: ocorreu um erro no envio de mensagem ou timeout.
 * Caso 0: foi enviada uma resposta do tipo erro.
 * Caso 1: o comando foi realizado com sucesso.
 */
int respostaCd(uint8_t* sequencia, int soquete, Mensagem msg_cd);


#endif  