#include <stdio.h>
#include <iostream>
#include <string>
#include <linux/limits.h>

#ifndef COMPILAR
#define COMPILAR

/**
 * @brief Executa o comando do gcc utilizando os <args> via terminal e imprime sua saída.
 * 
 * @param args Argumentos para executar junto ao comando gcc.
 * @return int Status ao fechar o terminal com pclose().
 */
int compila(std::string args);


#endif  