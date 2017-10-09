#ifndef COMMANDS_H_   /* Include guard */
#define COMMANDS_H_

/*
 * ex: cd . / cd $HOME / cd ..
 */
void cd(char *path);

/*
 * ex: x=5 / y=$x/7
 */
void assign_variable(const char *assignment);

/*
 * ex: printenv / env
 */
void printenv(const char *arg);

/*
 * ex: export x=15 / export HOME=/bin/
 */
void export(const char *assignment);

/*
 * history command which reads your commands history from a file and prints them.
 */
void history(void);

/*
 * prints error to stderr
 */
void error(const char *command, const char *msg);

#endif // COMMANDS_H_