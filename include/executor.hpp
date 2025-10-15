#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "command.hpp"
#include <vector>
#include <sys/types.h>

class Executor {
public:
//Ejecuta un pipeline compelto
static int execute(const Pipeline& pipeline);

private:

static int execute_simple(const Command& cmd);
static int execute_pipeline(const Pipeline& pipeline);
static bool setup_redirections(const Command& cmd);
static std::string resolve_path(const std::string& program);
static void collect_zombies();
};

#endif