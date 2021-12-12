#ifndef RAWSOCKETS
#define RAWSOCKETS

/**
 * @brief Função disponibilizada pelo professor para adquirir o número do soquete da conexão com o <device>. 
 * 
 * @param device Nome do descritor de arquivo a ser conectado.
 * @return int 
 * Caso -1: Erro ao conectar-se ao soquete.
 * Caso contrário: Valor do soquete conectado. 
 */
int ConexaoRawSocket(char *device);

#endif 
