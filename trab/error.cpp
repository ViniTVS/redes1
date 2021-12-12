#include "error.h"

void messageError(Mensagem error_msg){
    if (error_msg.corpo.tipo != 0b1111 || error_msg.corpo.tamanho != 1 || !error_msg.verificaParidade())
        return;
    
    std::cerr << "ERRO:\n";
    
    switch(error_msg.dados[0].num) {
        case 1:
            std::cerr << "Acesso proibido/sem permissão\n";
            break;
        case 2:
            std::cerr << "Diretório inexistente\n";
            break;
        case 3:
            std::cerr << "Arquivo inexistente\n";
            break;
        case 4:
            std::cerr << "Linha inexistente\n";
            break;
        default:
            break;
    }
    
    // if (m != NULL){}
}

