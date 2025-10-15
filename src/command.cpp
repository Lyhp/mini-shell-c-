
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

//Libera memoria de to_argv()
void Command::free_argv(char** argv) {
    if (!argv) return;
    
    //Liberar cada string
    for (int i = 0; argv[i] != nullptr; i++) {
        free(argv[i]);
    }
    
    //Liberar array
    delete[] argv;
}

//Para debugging
void Command::print() const {
    std::cout << "Comando: " << program;
    for (const auto& arg : args) {
        std::cout << " " << arg;
    }
    if(!input_file.empty()) {
        std::cout << " < " << input_file;
    }
    if(!output_file.empty()) {
        std::cout << (append_output ? " >> " : " > ") << output_file;
    }
    if(background) {
        std::cout << " &";
    }
    std::cout << std::endl;
}