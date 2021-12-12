#include "compilar.h"

int compila(std::string args){
    std::string comando = "gcc " + args + " 2>&1";
    FILE *fp;
    int status;
    char path[PATH_MAX];


    fp = popen(comando.c_str(), "r");
    if (fp == NULL)
        std::cout << "porra";
        /* Handle error */;


    while (fgets(path, PATH_MAX, fp) != NULL)
        std::cout << path;

    
    status = pclose(fp);

    return status;
}