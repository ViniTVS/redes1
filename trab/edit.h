#include <string.h>
#include <fstream>
#include "error.h"
#include "mensagem.h"

#ifndef EDIT
#define EDIT


int pedidoEdit(uint8_t* sequencia, int soquete, uint8_t linha, std::string nome_arquivo, std::string novo_texto);

int respostaEdit(uint8_t* sequencia, int soquete, Mensagem msg_linha);

#endif  