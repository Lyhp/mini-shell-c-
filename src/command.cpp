
#include "command.hpp"
#include <iostream>
#include <cstring>

//Convierte Command a formato execvp: ["ls", "-la", NULL]
char** Command::to_argv() const {
    int total = 1 + args.size() + 1;
    char** argv = new char*[total];
    
    argv[0] = strdup(program.c_str());
    
    for (size_t i = 0; i < args.size(); i++) {
        argv[i + 1] = strdup(args[i].c_str());
    }
    
    argv[total - 1] = nullptr;
    return argv;
}
