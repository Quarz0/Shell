#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "variables.h"
#include "commands.h"
#include "environment.h"

void cd(const char *path) {
    if (path == NULL || strcmp(path, "~") == 0) {
        char *home = lookup_variable("HOME");
        chdir(home);
        free(home);
    }
}

void assign_variable(const char *assignment) {
    if (!isalpha(assignment[0])) {
        error(assignment, "command not found");
        return;
    }
    int index = (int) (strchr(assignment, '=') - assignment);
    char key[index + 1];
    char val[strlen(assignment) - index + 1];
    memcpy(key, assignment, index);
    memcpy(val, assignment + index + 1, strlen(assignment) - index);
    key[index] = '\0';
    val[strlen(assignment) - index] = '\0';
    set_variable(key, val);
}

void printenv(const char *arg) {
    if (arg == NULL) {
        for (int i = 0; get_env_variables()[i] != NULL; i++) {
            char *val = lookup_variable(get_env_variables()[i]);
            printf("%s=%s\n", get_env_variables()[i], val);
            free(val);
        }
        printf("\n");
    } else {
        error(arg, "No such file or directory");
    }
}

void export(const char *assignment) {
    if (assignment == NULL || !strchr(assignment, '=')) {
        return;
    }
    if (!isalpha(assignment[0])) {
        error(assignment, "is not a valid identifier");
        return;
    }
    int index = (int) (strchr(assignment, '=') - assignment);
    char *key = malloc(sizeof(char) * (index + 1));
    char val[strlen(assignment) - index + 1];
    memcpy(key, assignment, index);
    memcpy(val, assignment + index + 1, strlen(assignment) - index);
    key[index] = '\0';
    val[strlen(assignment) - index] = '\0';
    set_variable(key, val);
    add_env_variable(key);
}

void error(const char *command, const char *msg) {
    printf("%s: %s\n", command, msg);
}