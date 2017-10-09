#include <stdio.h>

#ifndef FILE_PROCESSING_H_   /* Include guard */
#define FILE_PROCESSING_H_

/*
 * close history file and reopen it in read mode
 */
void start_read_history_file();

/*
 * close history file and reopen it in append mode
 */
void finish_read_history_file();

/*
 * open history file in append mode
 */
void open_history_file();

/*
 * return pointer to history file
 */
FILE *get_history_file();

/*
 * close history file if already open
 */
void close_history_file();

/*
 * open log file to log that a child process was terminated then close it again
 */
void write_log_file();

/*
 * open commands file in read mode
 */
void open_commands_batch_file(char *path);

/*
 * return pointer to commands file
 */
FILE *get_commands_batch_file();

/*
 * close commands file if already open
 */
void close_commands_batch_file();


#endif // FILE_PROCESSING_H_