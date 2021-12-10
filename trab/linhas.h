#include <string.h>
#include <fstream>
#include "error.h"
#include "mensagem.h"

#ifndef LINHAS
#define LINHAS


int pedidoLinhas(uint8_t* sequencia, int soquete, uint8_t linha_inical, uint8_t linha_final, std::string nome_arquivo);

int respostaLinhas(uint8_t* sequencia, int soquete, Mensagem msg_linha);

#endif  