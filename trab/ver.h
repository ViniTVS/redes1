#include <string.h>
#include <fstream>
#include "error.h"
#include "mensagem.h"

#ifndef VER
#define VER

int pedidoVer(uint8_t* sequencia, int soquete, std::string nome_arquivo);

int respostaVer(uint8_t* sequencia, int soquete, Mensagem msg_ver);

int pedidoLinha(uint8_t* sequencia, int soquete, uint8_t linha, std::string nome_arquivo);

int respostaLinha(uint8_t* sequencia, int soquete, Mensagem msg_linha);

#endif  
