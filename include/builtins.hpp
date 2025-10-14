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

private:
static int cmd_cd(const std::vector<std::string>& args);
static int cmd_pwd(const std::vector<std::string>& args);
static int cmd_exit(const std::vector<std::string>& args);
static int cmd_help(const std::vector<std::string>& args);
static int cmd_history(const std::vector<std::string>& args);
static int cmd_jobs(const std::vector<std::string>& args);
};

extern std::vector<std::string> command_history;

#endif