#include <string.h>
#include <stdlib.h>
#include "trie.h"

void insert(Trie *node, char *key, char *val){
    if (val == NULL){
        return;
    }
    if (*key == '\0'){
        node->val = malloc(sizeof(char) * (strlen(val) + 1));
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
        char* copy = malloc(sizeof(char) * (strlen(node->val) + 1));
        strcpy(copy, node->val);
        return copy;
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