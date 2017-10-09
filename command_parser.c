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

// replaces variables in the command with their values from the variable table or remove them if they don't exist
char *parse_vars(const char *command) {
    char buffer[MAX_COMMAND_LENGTH + 2];
    int index = 0;
    int buffer_index = 0;
    int var_index = -1;
    while (command[index] != '\0') {
        // if a $ has been found previously and the next char is not a valid var character
        // this means the variable name ends here
        if (var_index != -1 && !valid_var_char(command[index + 1])) {
            // search the variable table for this variable
            char *key = malloc(sizeof(char) * (index - var_index + 1));
            memcpy(key, command + var_index + 1, index - var_index);
            key[index - var_index] = '\0';
            char *val = lookup_variable(key);
            // if variable exist concatenate it to buffer and adjust buffer index
            if (val != NULL) {
                buffer[buffer_index] = '\0';
                strcat(buffer, val);
                buffer_index = strlen(buffer);
            }
            // set it to -1 meaning we're not currently reading a variable
            var_index = -1;
            index++;
            free(key);
        }
        // set variable index to indicate a variable name starts here
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
    // substitute variables first
    char *command = parse_vars(command_);
    char **args = (char **) malloc((strlen(command) + 1) * sizeof(char *));

    // split command by spaces and newlines and add each string to args
    char *pch;
    pch = strtok(command, " \t\r\n");
    int index = 0;
    while (pch != NULL) {
        args[index++] = pch;
        pch = strtok(NULL, " \t\r\n");
    }
    // if the last string in args is 2 or more chars and the last char is &, add it as a new string to the end of the args array
    if (index > 0 && strlen(args[index - 1]) > 1 && args[index - 1][strlen(args[index - 1]) - 1] == '&') {
        args[index - 1][strlen(args[index - 1]) - 1] = '\0';
        args[index++] = "&";
    }
    // if the last string in args is & that means it's a background process
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
    // skip all white spaces before echo
    while (*(command + index) == ' ' || *(command + index) == '\t') {
        index++;
    }
    // index now is at 'e' in echo
    command[index + 4] = '\n';

    // split by white spaces to add echo as first arg
    char *pch;
    pch = strtok(command, " \t\r\n");
    index = 0;
    // split by newline only to add everything after echo as 2nd arg
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

    // process the 2nd arg to maintain spaces inside quotes
    if (args[1] != NULL) {
        char *temp = malloc(sizeof(char) * (strlen(args[1]) + 1));
        int k = 0;
        int temp_index = 0;
        bool in_quotes = false;
        // skip whitespace in the very beginning
        while (args[1][k] == ' ' || args[1][k] == '\t') k++;
        // keep everything between quotes as is and keep only a single space between words if outside quotes
        while (args[1][k] != '\0') {
            if (args[1][k] == '"' || args[1][k] == '\'') {
                in_quotes = !in_quotes;
            } else if (in_quotes) {
                temp[temp_index++] = args[1][k];
            } else if ((args[1][k] == ' ' || args[1][k] == '\t') && args[1][k - 1] != ' ' && args[1][k - 1] != '\t') {
                temp[temp_index++] = ' ';
            } else if (args[1][k] != ' ' && args[1][k] != '\t') {
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
        // concatenate the path, separator and the command then check if it can be accessed, then return it
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

