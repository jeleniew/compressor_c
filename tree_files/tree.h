#ifndef _TREE_H_IS_INCLUDED_
#define _TREE_H_IS_INCLUDED_

#define MAX_CHAR_SIZE 256
#define BYTE 8
#define MAX_POWER 128

typedef struct Tree {
    unsigned char name;
    long int value;
    struct Tree *son0;
    struct Tree *son1;
    struct Tree *next;
} tree_t;

void add_leaf(tree_t *leaves, unsigned char read_char);

int sort_leaves(tree_t **leaves, long int count_symbols[]);

void create_tree_using_leaves(tree_t *tree);

tree_t *find_root(tree_t *tree);

void print_binary(char *byte, int *count_bits, int add, FILE *out);

void print_tree(tree_t *tree, char *byte, int *count_bits, FILE *out);

void free_branches(tree_t *tree);

int read_empty_byte(FILE *in);

tree_t *read_tree_from_file(tree_t *tree, FILE *in, unsigned char *byte, int *buf_size);

void free_tree(tree_t *tree);

#endif
