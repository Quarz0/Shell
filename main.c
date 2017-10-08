#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "environment.h"
#include "command_parser.h"
#include "commands.h"
#include "file_processing.h"



void start_shell(bool read_from_file);

void shell_loop(bool input_from_file);

void start(char *batch_file);

int main(int argc, char *argv[]) {
    setup_environment();

    // any other early configuration should be here
    cd(NULL);
    open_history_file();

    if (argc > 2){
        fprintf(stderr, "Usage: %s <file path (optional)>\n", argv[0]);
        return 2;
    }
    if (argc > 1) {
        start(argv[1]);
    } else {
        start(NULL);
    }

    close_commands_batch_file();
    close_history_file();
    return 0;
}

void start(char *batch_file) {
    if (batch_file != NULL) {
        open_commands_batch_file(batch_file);
        shell_loop(get_commands_batch_file() != NULL);
    } else {
        shell_loop(false);
    }
}

void shell_loop(bool input_from_file) {
    bool from_file = input_from_file;
    char *buffer = malloc(sizeof(char) * (MAX_BUFFER_SIZE + 1));

    while (true) {
        if (from_file) {
            if (fgets(buffer, MAX_BUFFER_SIZE, get_commands_batch_file()) == NULL){
                // EOF (CTRL-D)
                from_file = false;
                continue;
            }
            printf("> %s", buffer);

        } else {
            printf("Shell> ");
            if (fgets(buffer, MAX_BUFFER_SIZE, stdin) == NULL){
                // EOF (CTRL-D)
                return;
            }
        }
        fprintf(get_history_file(), "%s", buffer);
        if (strlen(buffer) - 1 > MAX_COMMAND_LENGTH){
            fprintf(stderr, "command is too long (over %d characters)\n", MAX_COMMAND_LENGTH);
            continue;
        }
        char **command = parse_command(buffer);
        // handle special commands
        // empty command
        if (command[0] == NULL) {
            continue;
        }
        if (strcmp(command[0], "printenv") == 0 || strcmp(command[0], "env") == 0) {
            printenv(command[1]);
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
        // export command
        if (strcmp(command[0], "export") == 0){
            export(command[1]);
            continue;
        }
        if (strcmp(command[0], "history") == 0){
            history();
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
            error(command[0], "command not found");
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
