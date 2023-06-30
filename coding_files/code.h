#ifndef _CODE_H_IS_INCLUDED_
#define _CODE_H_IS_INCLUDED_
#include "../tree_files/tree.h"

typedef struct Codes {
	char name;
	int *code;
	int length;
} codes_t;

codes_t *initiate_codes(codes_t *codes, int count_leaf, tree_t *tree);

void create_codes(codes_t *codes, tree_t *tree, int *code);

codes_t *get_code(codes_t *codes, char symbol);

void free_codes(codes_t *codes, int count_leaf);

#endif
