#ifndef COMMANDS_H_   /* Include guard */
#define COMMANDS_H_

/* 
	- This function should be responsible for implementing the "cd" shell command
*/
void cd(const char *path);

void assign_variable(const char *assignment);

void printenv(const char *arg);

void export(const char *assignment);

void history(void);

void error(const char *command, const char *msg);

#endif // COMMANDS_H_