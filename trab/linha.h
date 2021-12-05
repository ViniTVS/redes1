#include <string.h>
#include <fstream>
#include "error.h"
#include "mensagem.h"

#ifndef LINHA
#define LINHA


int pedidoLinha(uint8_t* sequencia, int soquete, uint8_t linha, std::string nome_arquivo);

int respostaLinha(uint8_t* sequencia, int soquete, Mensagem msg_linha);

#endif  