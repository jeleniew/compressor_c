#include <stdio.h>
#include <stdlib.h>
#include "compress.h"
#include "../tree_files/tree.h"
#include "../coding_files/code.h"

int decompress_file(FILE *in, FILE *out) {
    int current_symbol;
    int i;
    int power = 1;	
    int left_bits_after_read_tree = 0;
	int empty_bits = 0;
	unsigned char byte = 0;	
    unsigned char *next_byte = NULL;
    tree_t *tree = NULL;
    tree_t *current_branch = NULL;

    empty_bits = read_empty_byte(in);
    if (empty_bits >= BYTE) {
        fprintf(stdout, "Podano nieprawidłowy plik do dekompresji.\n");
        return -3;
    }
    tree = malloc(sizeof *tree);
    if (tree == NULL) {
        return -4;
    }
    tree = read_tree_from_file(tree, in, &byte, &left_bits_after_read_tree);
    if (tree == NULL) {
        return -4;
    }
    current_branch = tree;
    if ((left_bits_after_read_tree)%8 != 0) {
        for (i = 0; i < left_bits_after_read_tree - 1; i++) {
            power*=2;
        }
    }
    next_byte = malloc(sizeof *next_byte);
    if (next_byte == NULL) {
        return -4;
    }
    while (fread(next_byte, sizeof *next_byte, 1, in) != 0) {
        while (power > 0) {
            if (current_branch->son0 == NULL) {
                fwrite(&(current_branch->name), sizeof (current_branch->name), 1, out);
                current_branch = tree;
            }
            current_branch = (byte & power) ? current_branch->son1 : current_branch->son0;
            power/=2;
        }
        power = MAX_POWER;
        byte = *next_byte;
    } 
    free(next_byte); 
    for(i = 0; i < BYTE - empty_bits; i++) {
        if(current_branch->son0 == NULL) {
            fwrite(&(current_branch->name), sizeof (current_branch->name), 1, out);
            current_branch = tree;
        } 
        current_branch = (byte & power) ? current_branch->son1 : current_branch->son0;
        power/=2;
    }
    if(current_branch->son0 == NULL) {
        fwrite(&(current_branch->name), sizeof (current_branch->name), 1, out);
        current_branch = tree;
    } 
    free_tree(tree);
    return 0;
}