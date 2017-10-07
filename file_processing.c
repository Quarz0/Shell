#include <stdlib.h>
#include "file_processing.h"

FILE *commands = NULL;

/*
	history file section
*/
void open_history_file() {
    // you should implement this function
}

FILE *get_history_file() {
    // you should implement this function
    return NULL;
}

void close_history_file() {
    // you should implement this function
}


/* 
	log file section
*/
void open_log_file() {
    // you should implement this function
}

FILE *get_log_file() {
    // you should implement this function
    return NULL;
}

void close_log_file() {
    // you should implement this function
}


/* 
	CommandsBatch file section
*/
void open_commands_batch_file(char *path) {
    commands = fopen(path, "r");
    if (commands == NULL) {
        printf("%s: file not found\n", path);
    }
}

FILE *get_commands_batch_file() {
    return commands;
}

void close_commands_batch_file() {
    if (commands != NULL) {
        fclose(commands);
    }
}