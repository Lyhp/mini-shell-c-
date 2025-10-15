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

void setup_signals() {
    //Configurar SIGINT (Ctrl+C)
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
    if (sigaction(SIGINT, &sa, nullptr) < 0) {
        std::cerr << "Error configurando SIGINT" << std::endl;
    }
}
