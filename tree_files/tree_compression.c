#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

tree_t *initiate_leaves(tree_t *leaves, int count_chars) {
    int i;

    leaves = malloc(count_chars * sizeof *leaves);
    if (leaves == NULL) {
        return NULL;
    }
    for (i = 0; i < count_chars; i++) {
        (leaves + i)->name = 0;
        (leaves + i)->value = 0;
        (leaves + i)->son0 = NULL;
        (leaves + i)->son1 = NULL;
        (leaves + i)->next = NULL;
    }
    return leaves;
}

int sort_leaves(tree_t **leaves, long int count_symbols[]) {
    int i, j = 0;
    int count_char = 0;
    tree_t tmp;
    
    for (i = 0; i < MAX_CHAR_SIZE; i++) {
        if(count_symbols[i] != 0) {
            count_char++;
        }
    }
    *leaves = initiate_leaves(*leaves, count_char);
    if (*leaves == NULL) {
        return -4;
    }
    for (i = 0; i < MAX_CHAR_SIZE; i++) {
        if(count_symbols[i] != 0) {
            ((*leaves) + j)->name = i;
            ((*leaves) + j)->value = count_symbols[i];
            j++;
        }
    }
    for (i = 0; i < count_char - 1; i++) {
        for (j = i + 1; j < count_char; j++) {
            if (((*leaves) + i)->value > ((*leaves) + j)->value) {
                tmp = *((*leaves) + i);
                *((*leaves) + i) = *((*leaves) + j);
                *((*leaves) + j) = tmp;
            }
        }
    }
    if (leaves == NULL) {
        return -4;
    } 
    return count_char;
}

tree_t *initiate_branch(tree_t *son0, tree_t *son1) {
    tree_t *branch = malloc(sizeof *branch);
    branch->value = son0->value + son1->value;
    branch->son0 = son0;
    branch->son1 = son1;
    branch->next = NULL;
    return branch;
}

void add_to_tree(tree_t *tree, tree_t *new_element) {
    tree_t *current = tree;
    tree_t *previous = tree;
    while ((current->next != NULL) && (current->value <= new_element->value)) {
        previous = current;
        current = current->next;        
    } 
    if (current->next == NULL) {
        if (current->value <= new_element->value) {
            new_element->next = NULL;
            current->next = new_element;
        }
        else {
            previous->next = new_element; 
            new_element->next = current;         
        } 

    } else {   
        previous->next = new_element;
        new_element->next = current;   
    }
}

void create_tree_using_leaves(tree_t *tree) {
    tree_t *branch;
    tree_t *current = tree;

    while (current->next != NULL && current != NULL) {
        branch = initiate_branch(current, current->next);
        add_to_tree(tree, branch);
        current = current->next->next;
    } 
}

tree_t *find_root(tree_t *tree) {
    while (tree->next != NULL)
        tree = tree->next;
    return tree;
}

void print_binary(char *byte, int *count_bits, int add, FILE *out) {
    if (add)
        *byte |= (1 << (BYTE - (*count_bits) - 1));
    else
        *byte &= ~(1 << (BYTE - (*count_bits) - 1));
    (*count_bits)++;
    
    if (*count_bits == 8) {
        fwrite(byte, sizeof *byte, 1, out);
        (*count_bits) = 0;
    }
}

void char_binary(char symbol, char *byte, int *count_bits, FILE *out) {
    int power = MAX_POWER;

    while (power > 0) {
        print_binary(byte, count_bits, (symbol & power ? 1 : 0), out);       
        power/=2; 
    }
}

void print_tree(tree_t *tree, char *byte, int *count_bits, FILE *out) {
    int *current_location = malloc(MAX_CHAR_SIZE * sizeof *current_location);
    int length = 0;
    int i;
    tree_t *root = tree;
    tree_t *previousNode = tree;

    if(tree->son1 != NULL) {
        current_location[0] = 0;
        length++;
        print_binary(byte, count_bits, 0, out);
    } else {
        tree->value = 0;
    }
    while(length > 0) {
        if(tree == root) {
            for(i = 0; i<length; i++) {
                tree = (current_location[i] == 0) ? tree->son0 : tree->son1;
            }            
        }
        if(tree->son0 != NULL && tree->son0->value != 0) {
            current_location[length] = 0;
            length++;
            tree = tree->son0;
            print_binary(byte, count_bits, 0, out);
        } else if(tree->son0 != NULL && tree->son1->value != 0) {
            current_location[length] = 1;
            length++;
            tree = tree->son1;
        } else {
            if(tree->son0 == NULL) {
                print_binary(byte, count_bits, 1, out);
                char_binary(tree->name, byte, count_bits, out);
            }
            tree->value = 0;
            tree = root;
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

void free_branches(tree_t *tree) { 
    tree_t *tree_copy;
    while(tree->next != NULL) {
        if(tree->son0 != NULL) {
            tree_copy = tree;
            tree = tree->next;
            free(tree_copy);
        } else {
            tree = tree->next;
        }
    }
    if(tree->son0 != NULL) {
        tree_copy = tree;
        tree = tree->next;
        free(tree_copy);
    } else {
        tree = tree->next;
    }
}