#ifndef COMMANDS_H_   /* Include guard */
#define COMMANDS_H_

/* 
	- This function should be responsible for implementing the "cd" shell command
*/
void cd(const char *path);

void assign_variable(const char *assignment);

void error(const char *command);

#endif // COMMANDS_H_