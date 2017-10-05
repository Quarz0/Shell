#include <string.h>
#include <stdlib.h>
#include "trie.h"

void insert(Trie *node, char *key, char *val){
    if (*key == '\0'){
        node->val = malloc(sizeof(val));
        strcpy(node->val, val);
        return;
    }
    if (node->children[(int) *key] == NULL) {
        node->children[(int) *key] = create_node();
    }
    return insert(node->children[(int) *key], key + 1, val);
}

bool delete(Trie *node, char *key){
    if (*key == '\0'){
        bool res = node->val != NULL;
        if (res) {
            free(node->val);
            node->val = NULL;
        }
        return res;
    }
    if (node->children[(int) *key] == NULL){
        return false;
    }
    return delete(node->children[(int) *key], key + 1);
}

char *get(Trie *node, char *key){
    if (*key == '\0'){
        return node->val;
    }
    if (node->children[(int) *key] == NULL) {
        return NULL;
    }
    return get(node->children[(int) *key], key + 1);
}

Trie *create_node(void) {
    Trie *t = malloc(sizeof(Trie));
    t->val = NULL;
    for (int i = 0; i < CHILDREN_SIZE; i++) {
        t->children[i] = NULL;
    }
    return t;
}