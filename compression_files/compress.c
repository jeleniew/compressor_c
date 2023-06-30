#include <stdio.h>
#include <stdlib.h>
#include "compress.h"
#include "../tree_files/tree.h"
#include "../coding_files/code.h"

int compress_file(FILE *in, FILE *out) {
    int i = 0;
    int count_leaf = 0;
    int count_bits = 0;
    int empty_bits = 0;
    long int count_symbols[MAX_CHAR_SIZE];
    int code[MAX_CHAR_SIZE];
    unsigned char read;
    unsigned char byte = 0;
    unsigned char read_char;
    tree_t *leaves = NULL;
    tree_t *tree = NULL;
    codes_t *codes = NULL;
    codes_t *code_of_read = NULL;

    for (i = 0; i < MAX_CHAR_SIZE; i++) {
        count_symbols[i] = 0;
    }
    while (fread(&read_char, sizeof read_char, 1, in) != 0) {
        count_symbols[read_char]++;
    }
    count_leaf = sort_leaves(&leaves, count_symbols);
    if (count_leaf <= 0) {
        free(leaves);
        return count_leaf;
    }
    for (i = 0; i < count_leaf - 1; i++) {
		(leaves + i)->next = (leaves + i + 1);
	}
    (leaves + count_leaf - 1)->next = NULL;
    create_tree_using_leaves(leaves);
	tree = find_root(leaves);
    codes = initiate_codes(codes, count_leaf, leaves);
    if (codes == NULL) {
        return -4;
    }
	create_codes(codes, tree, code);
    fwrite(&byte, sizeof byte, 1, out);
    print_tree(tree, &byte, &count_bits, out);
    rewind(in);

    while (fread(&read, sizeof read, 1, in) != 0) {
        code_of_read = get_code(codes, read);
        for (i = 0; i < (code_of_read->length); i++) {
            print_binary(&byte, &count_bits, *(code_of_read->code + i), out);  
        }
    }
    free_codes(codes, count_leaf);
    while (count_bits != 0) {
        print_binary(&byte, &count_bits, 0, out);
        empty_bits++;
    }
    if (empty_bits != 0) { 
        rewind(out);
        byte = empty_bits;
        fwrite(&byte, sizeof(char), 1, out);
    }
    free_branches(leaves);
    free(leaves);
    return 0;
}
