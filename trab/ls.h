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

std::string get_working_path();
std::string list();
void criaMensagemLs(uint8_t sequencia, uint8_t *mensagem);

#endif  