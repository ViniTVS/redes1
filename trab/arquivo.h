#include <string.h>
#include <fstream>
#include "error.h"
#include "mensagem.h"

#ifndef ARQUIVO
#define ARQUIVO

int pedidoVer(uint8_t* sequencia, int soquete, std::string nome_arquivo);

int respostaVer(uint8_t* sequencia, int soquete, Mensagem msg_ver);

#endif  