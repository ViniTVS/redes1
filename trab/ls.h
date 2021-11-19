#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <sys/param.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

#ifndef LS
#define LS

std::string getPath();
std::string list();
int enviaLs(uint8_t sequencia);
void criaPedidoLs(uint8_t sequencia, uint8_t *mensagem);

#endif  