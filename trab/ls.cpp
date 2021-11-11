#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <sys/param.h>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

std::string get_working_path()
{
   char temp[MAXPATHLEN];
   return ( getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("") );
}

int list() {
    std::string path = get_working_path();
    for (const auto & entry : fs::directory_iterator(path)){
        std::cout << entry.path().filename().string() << std::endl;
    }
    return 1;
}
