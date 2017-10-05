#include <stdbool.h>
#ifndef SHELL_TRIE_H
#define SHELL_TRIE_H

#define CHILDREN_SIZE 256
typedef struct Trie Trie;

struct Trie {
    char *val;
    int cnt;
    Trie *children[CHILDREN_SIZE];
};

void insert(Trie *node, char *key, char *val);

bool delete(Trie *node, char *key);

char *get(Trie *node, char *key);

Trie *create_node(void);


#endif //SHELL_TRIE_H
