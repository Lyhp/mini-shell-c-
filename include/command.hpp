/**
 * @file command.hpp
 * @brief Define estructuras para representar comandos y pipelines.
 */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

/**
 * Representa un comando individual.
 * Ejemplo: "ls -la > output.txt"
 */
struct Command {
    std::string program;              
    std::vector<std::string> args;    
    std::string input_file;           
    std::string output_file;         
    bool append_output;               
    bool background;                
    
    Command() : append_output(false), background(false) {}
    
    char** to_argv() const;   
    static void free_argv(char** argv); 
    void print() const;      
};

#endif