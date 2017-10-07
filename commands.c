#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "variables.h"
#include "commands.h"

void cd(const char *path) {
    if (path == NULL || strcmp(path, "~") == 0) {
        char *home = lookup_variable("HOME");
        chdir(home);
        free(home);
    }
}

void assign_variable(const char *assignment) {
    if (!isalpha(assignment[0])) {
        error(assignment);
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

void error(const char *command) {
    printf("shell: %s: command not found\n", command);
}