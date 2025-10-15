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

