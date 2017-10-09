#include <stdbool.h>

#ifndef SHELL_TRIE_H
#define SHELL_TRIE_H

#define CHILDREN_SIZE 256
typedef struct Trie Trie;

struct Trie {
    char *val;
    Trie *children[CHILDREN_SIZE];
};

/*
 * insert a key value pair into the trie, if key already exist update the value only
 */
void insert(Trie *node, char *key, char *val);

/*
 * delete a key from the trie
 * return true if key exists, false otherwise
 */
bool delete(Trie *node, char *key);

/*
 * returns a copy of the value corresponding to the given key, or NULL if key doesn't exist
 */
char *get(Trie *node, char *key);

/*
 * allocate memory for a Trie and return a pointer to it
 */
Trie *create_node(void);


#endif //SHELL_TRIE_H
