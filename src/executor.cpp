#include "executor.hpp"
#include "builtins.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>
#include <algorithm>


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

//EJECUCIÓN PIPELINE (con pipes)

int Executor::execute_pipeline(const Pipeline& pipeline) {
    int num_cmds = pipeline.size();
    std::vector<pid_t> pids;
    
    //Crear pipes: necesitamos (num_cmds - 1) pipes
    //Usamos un array plano: [pipe0_read, pipe0_write, pipe1_read, pipe1_write, ...]
    std::vector<int> pipes((num_cmds - 1) * 2);
    for (int i = 0; i < num_cmds - 1; i++) {
        int pipe_fds[2];
        if (pipe(pipe_fds) < 0) {
            std::cerr << "Error en pipe()" << std::endl;
            return 1;
        }
        pipes[i * 2] = pipe_fds[0];      // lectura
        pipes[i * 2 + 1] = pipe_fds[1];  // escritura
    }
    
    // Crear procesos para cada comando
    for (int i = 0; i < num_cmds; i++) {
        const Command& cmd = pipeline.commands[i];
        
        pid_t pid = fork();
        
        if (pid < 0) {
            std::cerr << "Error en fork()" << std::endl;
            return 1;
        }
        
        if (pid == 0) {
            //PROCESO HIJO
            
            if (i > 0) {
                dup2(pipes[(i-1) * 2], STDIN_FILENO);  // lectura del pipe anterior
            }
            
            if (i < num_cmds - 1) {
                dup2(pipes[i * 2 + 1], STDOUT_FILENO);  
            }
            
            for (int j = 0; j < (num_cmds - 1) * 2; j++) {
                close(pipes[j]);
            }
            
            setup_redirections(cmd);
            
            //Ejecutar comando
            char** argv = cmd.to_argv();
            std::string path = resolve_path(cmd.program);
            execvp(path.c_str(), argv);
            
            std::cerr << "Error: " << cmd.program << std::endl;
            Command::free_argv(argv);
            exit(127);
        }
        
        //PROCESO PADRE
        pids.push_back(pid);
    }
    
    for (int i = 0; i < (num_cmds - 1) * 2; i++) {
        close(pipes[i]);
    }
    
    //Esperar a todos los procesos
    int last_status = 0;
    if (!pipeline.background) {
        for (pid_t pid : pids) {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                last_status = WEXITSTATUS(status);
            }
        }
    } else {
        std::cout << "[Pipeline] " << pids.size() << " procesos en background" << std::endl;
        for (pid_t pid : pids) {
            background_jobs.push_back(pid);
        }
    }
    
    return last_status;
}

// REDIRECCIONES (< > >>)

bool Executor::setup_redirections(const Command& cmd) {
    if (!cmd.input_file.empty()) {
        int fd = open(cmd.input_file.c_str(), O_RDONLY);
        if (fd < 0) {
            std::cerr << "Error: no se puede abrir " << cmd.input_file << std::endl;
            return false;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    
    if (!cmd.output_file.empty()) {
        int flags = O_WRONLY | O_CREAT;
        flags |= cmd.append_output ? O_APPEND : O_TRUNC;
        
        int fd = open(cmd.output_file.c_str(), flags, 0644);
        if (fd < 0) {
            std::cerr << "Error: no se puede crear " << cmd.output_file << std::endl;
            return false;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    return true;
}

//BUSQUEDA EN PATH

std::string Executor::resolve_path(const std::string& program) {
    if (program.find('/') != std::string::npos) {
        return program;
    }
    
    //Buscar en PATH
    const char* path_env = getenv("PATH");
    if (!path_env) {
        return program;
    }
    
    std::string path_str(path_env);
    size_t start = 0;
    size_t end;
    
    while ((end = path_str.find(':', start)) != std::string::npos) {
        std::string dir = path_str.substr(start, end - start);
        std::string full_path = dir + "/" + program;
        
        if (access(full_path.c_str(), X_OK) == 0) {
            return full_path;
        }
        start = end + 1;
    }
    
    //Ultima entrada en PATH
    std::string dir = path_str.substr(start);
    std::string full_path = dir + "/" + program;
    if (access(full_path.c_str(), X_OK) == 0) {
        return full_path;
    }
    
    return program;
}

//RECOLECCIÓN DE ZOMBIES

void Executor::collect_zombies() {
    //Recolectar procesos terminados sin bloquear
    pid_t pid;
    while ((pid = waitpid(-1, nullptr, WNOHANG)) > 0) {
        auto it = std::find(background_jobs.begin(), background_jobs.end(), pid);
        if (it != background_jobs.end()) {
            std::cout << "[Job completado] PID " << pid << std::endl;
            background_jobs.erase(it);
        }
    }
}