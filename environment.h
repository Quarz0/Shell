#ifndef ENVIRONMENT_H_   /* Include guard */
#define ENVIRONMENT_H_

/*
 * initializes the variable table and sets some environment variables (HOME, PATH, ROOT)
 * by adding them to the table
 */
void setup_environment(void);

/*
 * returns list of environment variables
 */
char **get_env_variables(void);

/*
 * add new environment variable to the list of environment variables
 */
void add_env_variable(char *var);

#endif // ENVIRONMENT_H_