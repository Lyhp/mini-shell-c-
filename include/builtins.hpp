/**
* @file builtins.hpp
* @brief Comandos internos del shell (no usan fork)
*/

#ifndef BUILTINS_HPP
#define BUILTINS_HPP

#include "command.hpp"
#include <string>
#include <vector>

class Builtins {
public:

static bool is_builtin(const std::string& program);

static int execute(const Command& cmd);

#endif