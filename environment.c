#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "environment.h"
#include "variables.h"

const char **env_variables = (char *[]) {"PATH", "HOME", "ROOT", NULL};

void setup_environment(void) {
    initialize_variable_table();
    for (int i = 0; env_variables[i] != NULL; i++) {
        set_variable(env_variables[i], getenv(env_variables[i]));
    }
}

char **get_env_variables(void) {
    return env_variables;
}

