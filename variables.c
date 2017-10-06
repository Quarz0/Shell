#include <stdio.h>
#include "variables.h"
#include "trie.h"

const Trie *root;

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

