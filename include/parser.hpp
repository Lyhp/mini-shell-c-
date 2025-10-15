 /**
 @file parser.hpp
* @brief Parser simplificado para convertir texto en comando
*/

#ifndef PARSER_HPP
#define PARSER_HPP

#include "command.hpp"
#include <string>
#include <vector>

class Parser {
public:
//Convierte una línea completa en un Pipeline ejecutable

static Pipeline parse_line(const std::string& line);

private:
static std::vector<std::string> tokenize(const std::string& text);
static std::vector<std::string> split_by_pipes(const std::string& line);
static Command parse_command(const std::vector<std::string>& tokens);
};

#endif