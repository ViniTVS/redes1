#include "mensagem.h"
#include "directory.h"

#ifndef LIST
#define LIST

std::string list();

int respostaLs(uint8_t *sequencia, int soquete);

int pedidoLs(uint8_t *sequencia, int soquete);

#endif  