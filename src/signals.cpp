#include "signals.hpp"
#include <iostream>
#include <csignal>
#include <unistd.h>

//Handler para Ctrl+C
void sigint_handler(int sig) {
    //No terminar el shell, solo imprimir nueva línea
    std::cout << std::endl;
    write(STDOUT_FILENO, "minishell$ ", 11);
}