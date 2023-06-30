#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "compression_files/compress.h"

int main(int argc, char **argv) {
    int opt;
    int decompression_flag = 0;
    int error_code = 0;
    const char default_output_compression[] = "compressed";
    const char default_output_decompression[] = "decompressed";
    char *input_file = NULL;
    char *output_file = NULL;
    FILE *in;
    FILE *out;

    while ((opt = getopt(argc, argv, "i:o:d")) != -1) {
        switch (opt) {
        case 'i':
            input_file = malloc((strlen(optarg) + 1) * sizeof *optarg);
            strcpy(input_file, optarg);
            break;
        case 'o':
            output_file = malloc((strlen(optarg) + 1) * sizeof *optarg);
            strcpy(output_file, optarg);
            break;
        case 'd':
            decompression_flag = 1;
            break;
        }
    }

    if (input_file == NULL) {
        fprintf(stderr, "Nie podano danych wejsciowych.\n");
        return -1;
    }

    in = fopen(input_file, "r");
    if (in == NULL) {
        fprintf(stdout, "Nie udalo sie odczytac pliku %s.\n", input_file);
        free(input_file);
        return -2;
    }
    free(input_file);

    if (decompression_flag == 0) {
        out = output_file == NULL ? fopen(default_output_compression, "w") : fopen(output_file, "w");
        if (output_file != NULL) {
            free(output_file);
        }
        error_code = compress_file(in, out);
    } else {
        out = output_file == NULL ? fopen(default_output_decompression, "w") : fopen(output_file, "w");
        if (output_file != NULL) {
            free(output_file);
        }
        error_code = decompress_file(in, out);
    }
    fclose(out);
    fclose(in);
    if (error_code == -4) {
        fprintf(stdout, "Błąd alokowania pamięci.\n");
    }
    return error_code;
}
