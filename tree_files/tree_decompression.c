#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

int read_empty_byte(FILE *in) {
    char read;
    fread(&read, sizeof read, 1, in);
    return (int)(read);
}

int *realloc_buf(int *buf, int size) {
    buf = realloc(buf, size * sizeof *buf);
    return buf;
}  

unsigned char int_to_char(int *buf) {
    int i;
    char symbol = 0;

    for (i = 0; i < BYTE; i++) {
        symbol = (symbol << 1) | (char)buf[i];
    }
    return symbol; 
}

int create_tree_from_file(int **buf, tree_t *tree, int *buf_size) {
    int current_location[MAX_CHAR_SIZE];
    int length = 0;
    tree_t *tree_copy = tree;
    if(**buf == 0) {
        current_location[length] = 0;
        length++;
    } else {
        (*buf_size)--;
        (*buf)++;
        tree_copy->name = int_to_char(*buf);
        tree_copy->son0 = NULL;
        tree_copy->son1 = NULL;
        (*buf)+=(BYTE - 1);
        (*buf_size)-=(BYTE-1);
        current_location[length] = 1;
    }
    (*buf_size)--;
    (*buf)++;
    while(length > 0) {
        for(int i=0; i<length-1; i++) {
            tree_copy = (current_location[i] == 0) ? tree_copy->son0 : tree_copy->son1;
        }
        if(**buf == 0) {
            if(current_location[length - 1] == 0) {
                tree_copy->son0 = malloc(sizeof *tree);
                if (tree_copy->son0 == NULL) {
                    return -4;
                }
                tree_copy = tree_copy->son0;
                current_location[length] = 0;
            } else {
                tree_copy->son1 = malloc(sizeof *tree);
                if (tree_copy->son1 == NULL) {
                    return -4;
                }
                tree_copy = tree_copy->son1;
                current_location[length] = 0;
            }
            length++;
        } else {
            if(current_location[length - 1] == 0) {
                tree_copy->son0 = malloc(sizeof *tree);
                if (tree_copy->son0 == NULL) {
                    return -4;
                }
                (*buf_size)--;
                (*buf)++;
                tree_copy->son0->name = int_to_char(*buf);
                (*buf)+=(BYTE - 1);
                (*buf_size)-=(BYTE-1);
                tree_copy->son0->son0 = NULL;
                tree_copy->son0->son1 = NULL;
                current_location[length - 1] = 1;
            } else {
                tree_copy->son1 = malloc(sizeof *tree);
                (*buf_size)--;
                (*buf)++;
                tree_copy->son1->name = int_to_char(*buf);
                (*buf)+=(BYTE - 1);
                (*buf_size)-=(BYTE-1);
                tree_copy->son1->son0 = NULL;
                tree_copy->son1->son1 = NULL;
                while((length > 0) && (current_location[length - 1] == 1)) {
                    length--;
                }
                if(length > 0) {
                    current_location[length-1] = 1;
                }
            }
        }
        tree_copy = tree;
        (*buf_size)--;
        (*buf)++;
    } 
    return 0;
}

tree_t *read_tree_from_file(tree_t *tree, FILE *in, unsigned char *byte, int *bits_of_code) {
    int balance = 0;
    int i;
    int power;
    int skip = 0;
    int alloced_size = BYTE;
    int buf_size = 0;
    int *buf = malloc(alloced_size * sizeof *buf);
    int *point_buf = buf;
    int current_symbol;

    if (buf == NULL) {
        return NULL;
    }
    while (balance >= 0) { 
        power = MAX_POWER;
        fread(byte, sizeof *byte, 1, in);
        
        if (buf_size >= alloced_size) {           
            buf = realloc_buf(buf, alloced_size*=2);
            if (buf == NULL) {
                return NULL;
            }
        } 
        for (i = 0; i < BYTE; i++) { 
            current_symbol = (*byte & power ? 1 : 0);
            power/=2;  
            buf[buf_size] = current_symbol;
            buf_size++;
            if (skip == 0) {
                if (current_symbol == 0) {
                    balance++;
                } else {
                    balance--;
                    skip++;
                }
            } else if (skip == BYTE) {
                skip = 0;
            } else {
                skip++;
            }
        }        
    }
    
    if (skip > 0) {
        power = MAX_POWER;
        fread(byte, sizeof *byte, 1, in);
        if (buf_size >= alloced_size) {           
            buf = realloc_buf(buf, alloced_size*=2);
            if (buf == NULL) {
                return NULL;
            }
        } 
        for (i = 0; i <= BYTE - skip; i++) { 
            current_symbol = (*byte & power ? 1 : 0);
            power/=2;  
            buf[buf_size] = current_symbol;
            buf_size++;
        }
    } 
    *bits_of_code = BYTE - (buf_size%BYTE);
    buf = realloc_buf(buf, buf_size);
    if (buf == NULL) {
        return NULL;
    }
    point_buf = buf;
    if (create_tree_from_file(&point_buf, tree, &buf_size) < 0) {
        free(buf);
        return NULL;
    }
    free(buf);
    return tree;
} 

void free_tree(tree_t *tree) {
    tree_t *tree_previous = tree;
    tree_t *root = tree;
    while(root->son1 != NULL) {
        if(tree->son0 != NULL) {
            tree_previous = tree;
            tree = tree->son0;
        } else if(tree->son1 != NULL) {
            tree_previous = tree;
            tree = tree->son1;
        } else {
            if(tree_previous->son0 == tree) {
                free(tree);
                tree_previous->son0 = NULL;
            } else {
                free(tree);
                tree_previous->son1 = NULL;
            }
            tree = root;
            tree_previous = root;
        }
    }   
    free(root);
}
