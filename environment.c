#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "environment.h"
#include "variables.h"

char **env_variables = (char *[]) {"PATH", "HOME", "ROOT", NULL};

void setup_environment(void) {
    initialize_variable_table();
    for (int i = 0; env_variables[i] != NULL; i++) {
        set_variable(env_variables[i], getenv(env_variables[i]));
    }
}

char **get_env_variables(void) {
    return env_variables;
}

void add_env_variable(char *var) {
    for (int i = 0; env_variables[i] != NULL; i++) {
        if (strcmp(env_variables[i], var) == 0) {
            return;
        }
    }
    char **env_variables_new = malloc(sizeof(env_variables) + sizeof(var) + sizeof(char *) + 1);
    int index = 0;
    while (env_variables[index] != NULL) {
        env_variables_new[index] = env_variables[index];
        index++;
    }
    env_variables_new[index++] = var;
    env_variables_new[index] = NULL;
    env_variables = env_variables_new;
}

