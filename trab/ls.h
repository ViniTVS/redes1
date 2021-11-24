#include <iostream>
#include <string>
#include <filesystem>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <linux/if.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <net/ethernet.h>
#include "mensagem.h"

#ifndef LS
#define LS

int trocaDir(std::string nome_dir);

std::string list();

int enviaRespostaLs(uint8_t sequencia, int soquete);

uint8_t criaPedidoLs(uint8_t sequencia, int soquete);

#endif  