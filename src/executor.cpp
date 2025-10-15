#include "executor.hpp"
#include "builtins.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>

//Jobs en background
std::vector<pid_t> background_jobs;

//FUNCIÓN PRINCIPAL

int Executor::execute(const Pipeline& pipeline) {
    if (pipeline.is_empty()) {
        return 0;
    }
    
    //Recolectar zombies antes de ejecutar
    collect_zombies();
    
    //Pipeline de 1 comando → ejecución simple
    if (pipeline.size() == 1) {
        return execute_simple(pipeline.commands[0]);
    }
    
    //Pipeline con pipes → ejecución compleja
    return execute_pipeline(pipeline);
}

// EJECUCIÓN SIMPLE (sin pipes)

int Executor::execute_simple(const Command& cmd) {
    // Verificar si es builtin
    if (Builtins::is_builtin(cmd.program)) {
        return Builtins::execute(cmd);
    }
    
    //Fork: crear proceso hijo
    pid_t pid = fork();
    
    if (pid < 0) {
        std::cerr << "Error en fork()" << std::endl;
        return 1;
    }
    
    if (pid == 0) {
        //PROCESO HIJO
        
        //Configurar redirecciones (< > >>)
        if (!setup_redirections(cmd)) {
            exit(1);
        }
        
        //Preparar argv para execvp
        char** argv = cmd.to_argv();
        
        //Buscar ejecutable en PATH
        std::string path = resolve_path(cmd.program);
        
        //Ejecutar programa
        execvp(path.c_str(), argv);
        
        //Si llegamos aqui, execvp fallo
        std::cerr << "Error: comando no encontrado: " << cmd.program << std::endl;
        Command::free_argv(argv);
        exit(127);
    }
    
    //PROCESO PADRE
    
    if (cmd.background) {
        //Background: no esperar
        std::cout << "[Job] PID " << pid << " en background" << std::endl;
        background_jobs.push_back(pid);
        return 0;
    } else {
        //Foreground: esperar a que termine
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        return 1;
    }
}