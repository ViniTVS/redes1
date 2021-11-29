#include <filesystem>
#include "mensagem.h"

#ifndef DIRECTORY
#define DIRECTORY
namespace fs = std::filesystem;

int trocaDir(std::string nome_dir);

int pedidoCd(uint8_t *sequencia, int soquete, std::string nome_dir);

int respostaCd(uint8_t* sequencia, int soquete, Mensagem msg_cd);


#endif  