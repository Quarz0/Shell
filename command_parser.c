#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "variables.h"
#include "command_parser.h"

char **parse_command(char *command) {
    char **args = (char **) malloc((strlen(command) + 1) * sizeof(char *));

    char *pch;
    pch = strtok(command, " \n");
    int index = 0;
    while (pch != NULL) {
        if (pch[0] == '$') {
            char *var = lookup_variable(pch + 1);
            if (var != NULL) {
                args[index++] = var;
            }
        } else {
            args[index++] = pch;
        }
        pch = strtok(NULL, " \n");
    }
    if (index > 0 && args[index - 1][strlen(args[index - 1]) - 1] == '&') {
        args[index - 1][strlen(args[index - 1]) - 1] = '\0';
        args[index++] = "&";
    }
    args[index] = NULL;
    return args;
}

char *get_command_path(char *command) {
    char *pch = strtok(lookup_variable("PATH"), ":");
    char *buffer = malloc(sizeof(char) * (MAX_COMMAND_LENGTH + 1));

    while (pch != NULL) {
        strcpy(buffer, pch);
        strcat(buffer, PATH_SEPARATOR);
        strcat(buffer, command);
        if (!(access(buffer, F_OK) || access(buffer, X_OK))) {
            return buffer;
        }
        pch = strtok(NULL, ":");
    }
    free(buffer);
    return NULL;
}
