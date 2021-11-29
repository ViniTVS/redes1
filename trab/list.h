// #include <iostream>
// #include <string>
// #include <unistd.h>
// #include <sys/param.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <sys/ioctl.h>
// #include <net/ethernet.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <limits.h>
// #include <net/ethernet.h>
#include "mensagem.h"
#include "directory.h"

#ifndef LIST
#define LIST

std::string list();

int enviaRespostaLs(uint8_t *sequencia, int soquete);

int pedidoLs(uint8_t *sequencia, int soquete);

#endif  