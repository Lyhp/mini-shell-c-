#include "shell.hpp"
#include "parser.hpp"
#include "executor.hpp"
#include "builtins.hpp"
#include "signals.hpp"
#include <iostream>
#include <unistd.h>
#include <climits>

Shell::Shell() : last_exit_code(0) {
    setup_signals();
}