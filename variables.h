#ifndef VARIABLES_H_   /* Include guard */
#define VARIABLES_H_

/*
 * create a new Trie
 */
void initialize_variable_table(void);

/*
 * looks up a variable in the Trie root and return its value or NULL if it doesn't exist
 */
char *lookup_variable(char *key);

/*
 * sets the value of a variable by inserting it into the Trie
*/
void set_variable(char *key, char *value);

/*
 * prints all the variables stored including the environment variables
*/
void print_all_variables(void);

/*
 * clears memory allocated in the Trie and deletes all variables from memory
 */
void clear_variables(void);

#endif // VARIABLES_H_