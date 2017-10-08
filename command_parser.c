#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "variables.h"
#include "command_parser.h"

bool background = false;

bool is_background_command(void) {
    return background;
}

bool valid_var_char(char c) {
    return isalpha(c) || (c >= '0' && c <= '9') || c == '_';
}

char *parse_vars(const char *command) {
    char buffer[MAX_COMMAND_LENGTH + 2];
    int index = 0;
    int buffer_index = 0;
    int var_index = -1;
    while (command[index] != '\0') {
        if (var_index != -1 && !valid_var_char(command[index + 1])) {
            char *key = malloc(sizeof(char) * (index - var_index + 1));
            memcpy(key, command + var_index + 1, index - var_index);
            key[index - var_index] = '\0';
            char *val = lookup_variable(key);
            if (val != NULL) {
                buffer[buffer_index] = '\0';
                strcat(buffer, val);
                buffer_index = strlen(buffer);
            }
            var_index = -1;
            index++;
            free(key);
        }
        if (command[index] == '$') {
            var_index = index;
        } else if (var_index == -1) {
            buffer[buffer_index++] = command[index];
        }
        index++;
    }
    buffer[buffer_index] = '\0';
    char *parsed = malloc(sizeof(char) * buffer_index);
    memcpy(parsed, buffer, buffer_index + 1);
    return parsed;
}

char **parse_command(const char *command_) {
    char *command = parse_vars(command_);
    char **args = (char **) malloc((strlen(command) + 1) * sizeof(char *));

    char *pch;
    pch = strtok(command, " \t\r\n");
    int index = 0;
    while (pch != NULL) {
        args[index++] = pch;
        pch = strtok(NULL, " \t\r\n");
    }
    if (index > 0 && strlen(args[index - 1]) > 1 && args[index - 1][strlen(args[index - 1]) - 1] == '&') {
        args[index - 1][strlen(args[index - 1]) - 1] = '\0';
        args[index++] = "&";
    }
    if ((background = (index > 0 && strcmp(args[index - 1], "&") == 0))) {
        index--;
    }
    args[index] = NULL;
    return args;
}

char **parse_echo(char *command_) {
    char *command = parse_vars(command_);
    char **args = (char **) malloc((strlen(command) + 1) * sizeof(char *));
    int index = 0;
    while (*(command + index) == ' ' || *(command + index) == '\t') {
        index++;
    }
    // index now is at 'e' in echo
    command[index + 4] = '\n';

    char *pch;
    pch = strtok(command, " \t\r\n");
    index = 0;
    while (pch != NULL) {
        args[index++] = pch;
        pch = strtok(NULL, "\r\n");
    }
    if (index > 0 && strlen(args[index - 1]) > 1 && args[index - 1][strlen(args[index - 1]) - 1] == '&') {
        args[index - 1][strlen(args[index - 1]) - 1] = '\0';
        args[index++] = "&";
    }
    if ((background = (index > 0 && strcmp(args[index - 1], "&") == 0))) {
        index--;
    }
    args[index] = NULL;

    if (args[1] != NULL){
        char *temp = malloc(sizeof(char) * (strlen(args[1]) + 1));
        int k = 0;
        int temp_index = 0;
        bool in_quotes = false;
        while (args[1][k] == ' ' || args[1][k] == '\t') k++;
        while (args[1][k] != '\0'){
            if (args[1][k] == '"' || args[1][k] == '\''){
                in_quotes = !in_quotes;
            }
            else if (in_quotes){
                temp[temp_index++] = args[1][k];
            }
            else if ((args[1][k] == ' ' || args[1][k] == '\t') && args[1][k-1] != ' ' && args[1][k-1] != '\t'){
                temp[temp_index++] = ' ';
            }
            else if (args[1][k] != ' ' && args[1][k] != '\t'){
                temp[temp_index++] = args[1][k];
            }
            k++;
        }
        temp[k] = '\0';
        args[1] = temp;
    }
    return args;
}

char *get_command_path(const char *command) {
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
    memcpy(buffer, command, strlen(command) + 1);
    return buffer;
}

