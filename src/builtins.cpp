#include "builtins.hpp"
#include "executor.hpp"
#include <iostream>
#include <unistd.h>
#include <climits>

//Historial de comandos
std::vector<std::string> command_history;

//VERIFICACIÓN Y EJECUCIÓN

bool Builtins::is_builtin(const std::string& program) {
    return program == "cd" ||
           program == "pwd" ||
           program == "exit" ||
           program == "help" ||
           program == "history" ||
           program == "jobs";
}

int Builtins::execute(const Command& cmd) {
    const std::string& prog = cmd.program;
    
    if (prog == "cd")       return cmd_cd(cmd.args);
    if (prog == "pwd")      return cmd_pwd(cmd.args);
    if (prog == "exit")     return cmd_exit(cmd.args);
    if (prog == "help")     return cmd_help(cmd.args);
    if (prog == "history")  return cmd_history(cmd.args);
    if (prog == "jobs")     return cmd_jobs(cmd.args);
    
    return 1;
}

int Builtins::cmd_cd(const std::vector<std::string>& args) {
    std::string target;
    
    if (args.empty()) {
        // cd sin argumentos → ir a HOME
        const char* home = getenv("HOME");
        if (!home) {
            std::cerr << "cd: HOME no definido" << std::endl;
            return 1;
        }
        target = home;
    } else {
        target = args[0];
    }
    if (chdir(target.c_str()) != 0) {
        perror("cd");
        return 1;
    }
    
    return 0;
}

// COMANDO: pwd (mostrar directorio actual)

int Builtins::cmd_pwd(const std::vector<std::string>& args) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == nullptr) {
        perror("pwd");
        return 1;
    }
    
    std::cout << cwd << std::endl;
    return 0;
}

// COMANDO: exit (salir del shell)

int Builtins::cmd_exit(const std::vector<std::string>& args) {
    int code =0;
    
    if (!args.empty()) {
        try {
            code = std::stoi(args[0]);
        } catch (...) {
            std::cerr << "exit: argumento inválido" << std::endl;
            return 1;
        }
    }
    std::cout << "Saliendo del shell..." << std::endl;
    exit(code);
}

//COMANDO: help (ayuda) 

int Builtins::cmd_help(const std::vector<std::string>& args) {
    std::cout << "\n=== minishell - Mini Shell en C++ ===\n\n";
    std::cout << "COMANDOS INTERNOS:\n";
    std::cout << "  cd [dir]     - Cambiar directorio\n";
    std::cout << "  pwd          - Mostrar directorio actual\n";
    std::cout << "  exit [code]  - Salir del shell\n";
    std::cout << "  help         - Mostrar esta ayuda\n";
    std::cout << "  history      - Mostrar historial de comandos\n";
    std::cout << "  jobs         - Mostrar procesos en background\n";
    std::cout << "\nFUNCIONALIDAD:\n";
    std::cout << "  cmd1 | cmd2  - Pipes\n";
    std::cout << "  cmd > file   - Redirección de salida\n";
    std::cout << "  cmd >> file  - Redirección append\n";
    std::cout << "  cmd < file   - Redirección de entrada\n";
    std::cout << "  cmd &        - Ejecución en background\n";
    std::cout << "\nEJEMPLOS:\n";
    std::cout << "  ls -la | grep txt\n";
    std::cout << "  cat file.txt | sort > sorted.txt\n";
    std::cout << "  sleep 10 &\n";
    std::cout << std::endl;
    return 0;
}
