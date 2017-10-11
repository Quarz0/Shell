#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "environment.h"
#include "command_parser.h"
#include "commands.h"
#include "file_processing.h"
#include "variables.h"
#include <sys/types.h>
#include <sys/wait.h>

void shell_loop(bool input_from_file);

void start(char *batch_file);

int main(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s <file path (optional)>\n", argv[0]);
        return 2;
    }

    // setup environment and necessary configurations
    setup_environment();
    signal(SIGCHLD, write_log_file);
    open_history_file();
    cd(NULL);

    if (argc > 1) {
        start(argv[1]);
    } else {
        start(NULL);
    }

    // close open files and clear memory allocated for variables
    close_commands_batch_file();
    close_history_file();
    clear_variables();
    return 0;
}

void start(char *batch_file) {
    if (batch_file != NULL) {
        open_commands_batch_file(batch_file);
        // if batch_file doesn't exist input_from_file param will be set to false
        shell_loop(get_commands_batch_file() != NULL);
    } else {
        shell_loop(false);
    }
}

void shell_loop(bool input_from_file) {
    bool from_file = input_from_file;
    char *buffer = malloc(sizeof(char) * (MAX_BUFFER_SIZE + 1));

    while (true) {
        memset(buffer, 0, sizeof buffer);
        if (from_file) {
            if (fgets(buffer, MAX_BUFFER_SIZE, get_commands_batch_file()) == NULL) {
                // EOF was reached
                from_file = false;
                continue;
            }
            printf("> %s", buffer);

        } else {
            printf("Shell> ");
            if (fgets(buffer, MAX_BUFFER_SIZE, stdin) == NULL) {
                // EOF (CTRL-D) was entered by user
                free(buffer);
                return;
            }
        }
        fprintf(get_history_file(), "%s", buffer);
        fflush(get_history_file());
        // check if command exceed max length
        if (strlen(buffer) - 1 > MAX_COMMAND_LENGTH) {
            fprintf(stderr, "command is too long (over %d characters)\n", MAX_COMMAND_LENGTH);
            continue;
        }
        char **command = parse_command(buffer);
        // handle special commands
        // empty command or comment
        if (command[0] == NULL || command[0][0] == '#') {
            continue;
        }
        // print env variables
        if (strcmp(command[0], "printenv") == 0 || strcmp(command[0], "env") == 0) {
            printenv(command[1]);
            continue;
        }
        // exit
        if (strcmp(command[0], "exit") == 0) {
            free(buffer);
            for (int i = 0; command[i] != NULL; i++) {
                free(command[i]);
            }
            free(command);
            return;
        }
        // echo
        if (strcmp(command[0], "echo") == 0) {
            command = parse_echo(buffer);
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
        if (strcmp(command[0], "export") == 0) {
            export(command[1]);
            continue;
        }
        // history
        if (strcmp(command[0], "history") == 0) {
            history();
            continue;
        }

        // get the full path of the command
        char *full_path = get_command_path(command[0]);

        // start new child process
        pid_t child_pid = fork();
        if (child_pid == 0) {
            // if background command, execute in background by directing output to temp file instead of stdout
            if (is_background_command()) {
                freopen(".temp", "w", stdout);
            }
            // execute command
            execv(full_path, command);

            // if command executed successfully it should reach this line
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
    }
}
