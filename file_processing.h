#include <stdio.h>

#ifndef FILE_PROCESSING_H_   /* Include guard */
#define FILE_PROCESSING_H_

/* 
	history file basic functions' prototypes
*/
void start_read_history_file();

void finish_read_history_file();

void open_history_file();

FILE *get_history_file();

void close_history_file();


/* 
	log file basic functions' prototypes
*/
void write_log_file();

/* 
	CommandsBatch file basic functions' prototypes
*/
void open_commands_batch_file(char *path);

FILE *get_commands_batch_file();

void close_commands_batch_file();


#endif // FILE_PROCESSING_H_