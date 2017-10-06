#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "environment.h"
#include "command_parser.h"
#include "variables.h"

#define MAX_BUFFER_SIZE 4096


void start_shell(bool read_from_file);

void shell_loop(bool input_from_file);

void start(char *batch_file);

int main(int argc, char *argv[]) {
    setup_environment();

    // any other early configuration should be here

    if (argc > 1) {
        start(argv[1]);
    } else {
        start(NULL);
    }

    return 0;
}

void start(char *batch_file) {
    chdir("");

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
        if (command[0] == NULL){
            continue;
        }
        // exit
        if (strcmp(command[0], "exit") == 0) {
            return;
        }
        // cd (change directory)
        if (strcmp(command[0], "cd") == 0){
            if (command[1] == NULL){
                chdir("");
            }
            else {
                chdir(command[1]);
            }
            continue;
        }
        // variable assignment
        if (strchr(command[0], '=')){
            int index = (int) (strchr(command[0], '=') - command[0]);
            char key[index + 1];
            char val[strlen(command[0]) - index + 1];
            memcpy(key, command[0], index);
            memcpy(val, command[0] + index + 1, strlen(command[0]) - index);
            key[index] = '\0';
            val[strlen(command[0]) - index] = '\0';
            set_variable(key, val);
            continue;
        }

        //parse your command here
        char *full_path = get_command_path(command[0]);
        bool wait_ = true;

        pid_t child_pid = fork();
        if (child_pid == 0) {
            for (int i = 0; command[i] != NULL; i++){
                if (strcmp(command[i], "&") == 0){
                    wait_ = false;
                    command[i] = NULL;
                }
            }
            if (!wait_)
                freopen("newin", "w", stdout);
            execv(full_path, command);
            printf("Command not found\n");
            exit(1);
        } else {
            if (wait_)
                wait(NULL);
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
