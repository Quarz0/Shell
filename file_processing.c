#include <stdlib.h>
#include "file_processing.h"
#include "commands.h"

FILE *commands_file = NULL;
FILE *history_file = NULL;

void open_history_file() {
    history_file = fopen(".history", "a+");
}

FILE *get_history_file() {
    return history_file;
}

void close_history_file() {
    if (history_file != NULL) {
        fclose(history_file);
    }
}

void write_log_file() {
    FILE *log = fopen(".log", "a");
    fprintf(log, "Child process was terminated.\n");
    fclose(log);
}

void open_commands_batch_file(char *path) {
    commands_file = fopen(path, "r");
    if (commands_file == NULL) {
        error(path, "file not found");
    }
}

FILE *get_commands_batch_file() {
    return commands_file;
}

void close_commands_batch_file() {
    if (commands_file != NULL) {
        fclose(commands_file);
    }
}