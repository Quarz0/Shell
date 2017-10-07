#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "environment.h"
#include "command_parser.h"
#include "commands.h"

#define MAX_BUFFER_SIZE 4096


void start_shell(bool read_from_file);

void shell_loop(bool input_from_file);

void start(char *batch_file);

int main(int argc, char *argv[]) {
    setup_environment();

    // any other early configuration should be here
    cd(NULL);

    if (argc > 1) {
        start(argv[1]);
    } else {
        start(NULL);
    }

    return 0;
}

void start(char *batch_file) {

    if (batch_file != NULL) {
        // file processing functions should be called from here
//        open_commands_batch_file(batch_file);

        shell_loop(true);
    } else {
        shell_loop(false);
    }
}


void shell_loop(bool input_from_file) {
    bool from_file = input_from_file;
    char *buffer = malloc(sizeof(char) * (MAX_COMMAND_LENGTH + 1));

    while (true) {
        if (from_file) {
            //read next instruction from file

            // if end of file {from_file = false; continue;}
        } else {
            printf("Shell> ");
            fgets(buffer, MAX_COMMAND_LENGTH + 1, stdin);
        }
        char **command = parse_command(buffer);
        // handle special commands
        // empty command
        if (command[0] == NULL) {
            continue;
        }
        // exit
        if (strcmp(command[0], "exit") == 0) {
            return;
        }
        // cd (change directory)
        if (strcmp(command[0], "cd") == 0) {
            cd(command[1]);
            continue;
        }
        // variable assignment
        if (strchr(command[0], '=')) {
            assign_variable(command[0]);
            continue;
        }

        //parse your command here
        char *full_path = get_command_path(command[0]);

        pid_t child_pid = fork();
        if (child_pid == 0) {
            if (is_background_command()) {
                freopen("newin", "w", stdout);
            }
            execv(full_path, command);
            error(command[0]);
            exit(1);
        } else {
            if (!is_background_command()) {
                wait(NULL);
            }
            printf("\n");
            free(command);
            free(full_path);
        }

        //execute your command here

        /*
            you don't need to write all logic here, a better practice is to call functions,
            each one contains a coherent set of logical instructions
        */
    }
}
