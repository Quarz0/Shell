#ifndef COMMAND_PARSER_H   /* Include guard */
#define COMMAND_PARSER_H
#define MAX_COMMAND_LENGTH 512
#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

/* 
	- This function should be responsible for importing all details of the command 
	- Should specify the type of the command "comment, cd, echo, expression - X=5 -, else"
	- Should specify the arguments of the command
	- Should specify if the command is background or foreground
	- Should consider all parsing special cases, example: many spaces in  "ls     -a"

	- You're left free to decide how to return your imported details of this command

	- Best practice is to use helper function for each collection of logical instructions,
	  example: function for splitting the command by space into array of strings, ..etc
*/
char **parse_command(char *command);

char *get_command_path(char *command);

bool is_background_command(void);

#endif // COMMAND_PARSER_H