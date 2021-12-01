#include "mensagem.h"

#ifndef ERROR
#define ERROR

/**
 * @brief Função para exibir o erro de mensagens do tipo erro (1111) na saida stdErr 
 * 
 * @param error_msg 
 */
void messageError(Mensagem error_msg);

void handleError(int error);

#endif  