#include "mensagem.h"
#include "directory.h"

#ifndef LIST
#define LIST

std::string ls();

int respostaLs(uint8_t *sequencia, int soquete);

int pedidoLs(uint8_t *sequencia, int soquete);

#endif  