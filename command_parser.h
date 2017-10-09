#ifndef COMMAND_PARSER_H   /* Include guard */
#define COMMAND_PARSER_H
#define MAX_COMMAND_LENGTH 512
#define MAX_BUFFER_SIZE 4096

#include <stdbool.h>

#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

/*
 * This function parses the string command and substitues variables if exists.
 * It also parses the & to detect background command and sets a flag so that is_background_command can use it.
 * Returns array of strings where first is the command and the rest are the command parameters.
 */
char **parse_command(const char *command_);

/*
 * Similar to parse_command but maintains the spaces in the parameter to echo
 * if surrounded by quotes. Also assumes that the first word is echo
 */
char **parse_echo(char *command_);

/*
 * Searches the path for the command and if found return the full path
 * else return the command as it is.
 */
char *get_command_path(const char *command);

/*
 * Returns true if the command ends with & meaning that the command should be executed in the background.
 */
bool is_background_command(void);

#endif // COMMAND_PARSER_H