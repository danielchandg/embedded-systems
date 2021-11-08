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
                //fprintf(stderr, "Error: failed to open infile.\n");
                return 0;
            }
            infile_name = optarg;
            break;
        case 'o':
            outfile = open(optarg, O_WRONLY);
            if (outfile == -1) {
                outfile = creat(optarg, S_IRWXU);
                //fprintf(stderr, "Error: failed to open outfile.\n");
                //return 0;
            }
            break;
        case 'v': verbose = true; break;
        case '?': help_string(cwd); return 0;
        }
    }
    uint64_t histogram[ALPHABET];
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
    /*
	for(int i=0; i<ALPHABET; i++){
		if(histogram[i] > 0){
			printf("%c: %lu\n", i, histogram[i]);
		}
	}
	printf("\n\n");*/
    Node *tree = build_tree(histogram);
    //print_tree(tree);

    Code table[ALPHABET];
    for (int i = 0; i < ALPHABET; i++)
        table[i] = code_init();
    build_codes(tree, table);

    /*
	for(int i=0; i<ALPHABET; i++){
		if(!code_empty(&table[i])){
			printf("%c's ", i);
			code_print(&table[i]);	
		}
	}
	printf("\nTree Dump:\n");
	dump_tree(1, tree);
	printf("\n\n");
	*/

    Header h;
    h.magic = MAGIC;
    h.tree_size = 3 * unique_symbols - 1;

    struct stat statbuf;
    fstat(infile, &statbuf);
    // statbuf.st_mode is the permissions.
    fchmod(outfile, statbuf.st_mode);
    h.permissions = statbuf.st_mode;
    h.file_size = statbuf.st_size;

    write_bytes(outfile, (uint8_t *) &h, sizeof(Header));
    //read_bytes(in, (uint8_t *) &h, sizeof(header));

    dump_tree(outfile, tree);

    int encode_file = open(infile_name, O_RDONLY);
    if (encode_file <= 0) {
        printf("Error reopening infile\n");
        return 1;
    }
    while (true) {
        int reading = read_bytes(encode_file, bit, 1);
        if (reading <= 0)
            break;
        write_code(outfile, &table[*bit]);
    }
    flush_codes(outfile);
    if(verbose){
	double space_saving = ((double)bytes_written) / ((double)bytes_read/2);
	fprintf(stdout, "Uncompressed file size: %" PRIu64 " bytes\nCompressed file size: %" PRIu64 " bytes\nSpace saving: %4.2lf%%\n", bytes_read/2, bytes_written, 100*(1-space_saving));
    }
    close(infile);
    close(outfile);
    close(encode_file);

	free(bit);
	bit = NULL;
	delete_tree(&tree);
    return 0;
}
