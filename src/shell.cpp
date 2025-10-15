#include "shell.hpp"
#include "parser.hpp"
#include "executor.hpp"
#include "builtins.hpp"
#include "signals.hpp"
#include <iostream>
#include <unistd.h>
#include <climits>

Shell::Shell() : last_exit_code(0) {
    setup_signals();
}

void Shell::run() {
    std::cout << "\n╔════════════════════════════════════╗\n";
    std::cout << "║   Mini Shell en C++                  ║\n";
    std::cout << "║   Escribe 'help' para ayuda          ║\n";
    std::cout << "╚════════════════════════════════════  ╝\n\n";
    
    while (true) {
        show_prompt();
        
        std::string line = read_line();
        
        // EOF (Ctrl+D) termina el shell
        if (std::cin.eof()) {
            std::cout << "\nSaliendo..." << std::endl;
            break;
        }
        
        // Procesar línea
        if (!line.empty()) {
            command_history.push_back(line);
            process_line(line);
        }
    }
   }