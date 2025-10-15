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

 // PROMPT
void Shell::show_prompt() {
    // Obtener usuario
    const char* user = getenv("USER");
    if (!user) user = "user";
    
    // Obtener hostname
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    
    // Obtener directorio actual
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == nullptr) {
        strcpy(cwd, "?");
    }
    
    // Formato: usuario@host:ruta$
    std::cout << "\033[1;32m" << user << "@" << hostname << "\033[0m:";
    std::cout << "\033[1;34m" << cwd << "\033[0m$ ";
    std::cout.flush();
  }

  // LECTURA

std::string Shell::read_line() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

// PROCESAMIENTO

void Shell::process_line(const std::string& line) {
    // Parsear línea → Pipeline
    Pipeline pipeline = Parser::parse_line(line);
    
    if (pipeline.is_empty()) {
        return;
    }
    
    // Ejecutar pipeline
    last_exit_code = Executor::execute(pipeline);
}

