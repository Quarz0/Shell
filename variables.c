#include <stdio.h>
#include <stdlib.h>
#include "variables.h"
#include "trie.h"

Trie *root;

const initialize_variable_table(void) {
    root = create_node();
}

char *lookup_variable(const char *key) {
    return get(root, key);
}

void set_variable(const char *key, const char *value) {
    insert(root, key, value);
}

void print_variables_helper(Trie *node) {
    if (node->val != NULL) {
        printf("%s\n", node->val);
    }
    for (int i = 0; i < CHILDREN_SIZE; i++) {
        if (node->children[i] != NULL) {
            print_variables_helper(node->children[i]);
        }
    }
}

void print_all_variables(void) {
    print_variables_helper(root);
}

void clear_helper(Trie *node){
    if (node->val != NULL) {
        free(node->val);
    }
    for (int i = 0; i < CHILDREN_SIZE; i++) {
        if (node->children[i] != NULL) {
            clear_helper(node->children[i]);
        }
    }
    free(node);
}

void clear_variables(void) {
    clear_helper(root);
}
