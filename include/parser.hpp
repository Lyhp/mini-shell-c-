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

};
#endif