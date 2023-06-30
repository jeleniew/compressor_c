#include <stdio.h>
#include <stdlib.h>
#include "code.h"
#include "../tree_files/tree.h"

codes_t *initiate_codes(codes_t *codes, int count_leaf, tree_t *tree) {
    int i;

    codes = malloc(count_leaf * sizeof *codes);
    if (codes == NULL) {
        return NULL;
    }
    for (i = 0; i < count_leaf; i++) {
        (codes + i)->name = (tree + i)->name;
    }
    return codes;
}

codes_t *add_code_to_structure(codes_t *codes, int *code, int code_length, char name) {
    int i = 0;
    int j;
    int *codeStruct;

    codeStruct = malloc(code_length * sizeof *codeStruct);
    if (codeStruct == NULL) {
        return NULL;
    }
    while ((codes + i)->name != name) {
        i++;
    }
    for (j = 0; j < code_length; j++) {
        *(codeStruct + j) = code[j];
    }
    (codes + i)->code = codeStruct;
    (codes + i)->length = code_length;
    return codes;
}

void create_codes(codes_t *codes, tree_t *tree, int *code) {
    int *current_location = malloc(MAX_CHAR_SIZE * sizeof *current_location);
    int length = 0;
    int i;
    int code_length = 0;
    tree_t *root = tree;
    tree_t *previousNode = tree;

    if(tree->son1 != NULL) {
        current_location[0] = 0;
        length++;
    } else {
        tree->value = 0;
    }
    while(length > 0) {
        if(tree == root) {
            for(i = 0; i<length; i++) {
                tree = (current_location[i] == 0) ? tree->son0 : tree->son1;
                *(code + i) = (current_location[i] == 0) ? 0 : 1;
                code_length++;
            }
        }
        if(tree->son0 != NULL && tree->son0->value != -1) {
            current_location[length] = 0;
            length++;
            tree = tree->son0;
            *(code + code_length) = 0;
            code_length++;
        } else if(tree->son0 != NULL && tree->son1->value != -1) {
            current_location[length] = 1;
            length++;
            tree = tree->son1;
            *(code + code_length) = 1;
            code_length++;
        } else {
            if(tree->son0 == NULL) { 
                codes = add_code_to_structure(codes, code, code_length, tree->name);
            }
            tree->value = -1;
            tree = root;
            code_length = 0;
            if(current_location[length - 1] == 0) {
                current_location[length - 1] = 1;
            } else {
                while(length > 0 && (current_location[length - 1] == 1)) {
                    length--;
                }
            }
        }
    }
    free(current_location);
}

codes_t *get_code(codes_t *codes, char symbol) {
    codes_t *current = codes;
    while (current->name != symbol) {
        current++;
    }
    return current;
}

void free_codes(codes_t *codes, int count_leaf) {
    int i;

    for (i = 0; i < count_leaf; i++) {
        free((codes + i)->code);
    }
    free(codes);
}
