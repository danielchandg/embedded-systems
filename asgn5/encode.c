#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "node.h"
#include "pq.h"
#include "code.h"
#include "io.h"
#include "stack.h"
#include "defines.h"
#include "huffman.h"
#include "header.h"

#define OPTIONS "hi:o:v"

bool verbose = false;

// Helper function to print help message.
void help_string(char *cwd) {
    fprintf(stdout,
        "SYNOPSIS\n  A Huffman encoder.\n  Compresses a file using the Huffman coding "
        "algorithm.\n\nUSAGE\n  %s [-h] [-i infile] [o outfile]\n\nOPTIONS\n  -h             "
        "Program usage and help.\n  -v             Print compression statistics.\n  -i infile      "
        "Input file to compress.\n  -o outfile     Output of compressed data.\n",
        cwd);
}

int main(int argc, char **argv) {
    int64_t opt = 0;
    int infile = 0, outfile = 1;
    char *infile_name = "";
    char *cwd = argv[0];

    // Reading command-line options.
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help_string(cwd); return 0;
        case 'i':
            infile = open(optarg, O_RDONLY);
            if (infile == -1) {
                return 0;
            }
            infile_name = optarg;
            break;
        case 'o':
            outfile = open(optarg, O_WRONLY);
            // Create the outfile if not found.
            if (outfile == -1) {
                outfile = creat(optarg, S_IRWXU);
            }
            break;
        case 'v': verbose = true; break;
        case '?': help_string(cwd); return 0;
        }
    }

    // Create a histogram of the symbols in the file.
    uint64_t *histogram = (uint64_t *) calloc(ALPHABET, sizeof(uint64_t));
    for (int i = 0; i < ALPHABET; i++)
        histogram[i] = 0;
    uint8_t *bit = (uint8_t *) calloc(1, sizeof(uint8_t));
    histogram[0]++;
    histogram[ALPHABET - 1]++;
    int unique_symbols = 2;
    while (true) {
        int reading = read_bytes(infile, bit, 1);
        if (reading <= 0)
            break;
        histogram[*bit]++;
        if (histogram[*bit] == 1)
            unique_symbols++;
    }

    // Debug message to print the histogram.
    /*
	for(int i=0; i<ALPHABET; i++){
		if(histogram[i] > 0){
			printf("%c: %lu\n", i, histogram[i]);
		}
	}
    printf("\n\n");*/

    // Build the Huffman tree.
    Node *tree = build_tree(histogram);

    // Build the code table with post-order traversal of the tree.
    Code *table = (Code *) calloc(ALPHABET, sizeof(Code));
    for (int i = 0; i < ALPHABET; i++)
        table[i] = code_init();
    build_codes(tree, table);

    // Create the header.
    Header *h = (Header *) malloc(sizeof(Header));
    h->magic = MAGIC;
    h->tree_size = 3 * unique_symbols - 1;

    // Create a stat struct to get file size and file permissions.
    struct stat statbuf;
    fstat(infile, &statbuf);
    // statbuf.st_mode is the permissions.
    fchmod(outfile, statbuf.st_mode);
    h->permissions = statbuf.st_mode;
    h->file_size = statbuf.st_size;

    // Print things to outfile.
    write_bytes(outfile, (uint8_t *) h, sizeof(Header));
    dump_tree(outfile, tree);

    int encode_file = open(infile_name, O_RDONLY);
    if (encode_file <= 0) {
        printf("Error reopening infile\n");
        return 1;
    }

    // Write Codes to file.
    while (true) {
        int reading = read_bytes(encode_file, bit, 1);
        if (reading <= 0)
            break;
        write_code(outfile, &table[*bit]);
    }
    flush_codes(outfile);

    // Print verbose statistics if requested.
    if (verbose) {
        double space_saving = ((double) bytes_written) / ((double) bytes_read / 2);
        fprintf(stdout,
            "Uncompressed file size: %" PRIu64 " bytes\nCompressed file size: %" PRIu64
            " bytes\nSpace saving: %4.2lf%%\n",
            bytes_read / 2, bytes_written, 100 * (1 - space_saving));
    }

    // Close files and memory cleanup.
    close(infile);
    close(outfile);
    close(encode_file);

    infile_name = NULL;
    cwd = NULL;
    free(histogram);
    histogram = NULL;
    free(table);
    table = NULL;
    free(bit);
    bit = NULL;
    free(h);
    h = NULL;
    delete_tree(&tree);
    tree = NULL;
    return 0;
}
