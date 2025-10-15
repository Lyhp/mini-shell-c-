/**
* @file shell.hpp
* @brief Shell principal - orquesta todo el sistema
*/

#ifndef SHELL_HPP
#define SHELL_HPP

#include <string>

class Shell {
public:
Shell();
//Inicia el loop principal del shell
void run();

private:
void show_prompt();
std::string read_line();
void process_line(const std::string& line);
int last_exit_code;
};


#endif