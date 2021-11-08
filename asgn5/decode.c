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

// Helper function to print help string.
void help_string(char *cwd) {
    fprintf(stdout,
        "SYNOPSIS\n  A Huffman decoder.\n  Decompresses a file using the Huffman coding "
        "algorithm.\n\nUSAGE\n  %s [-h] [-i infile] [-o outfile]\n\nOPTIONS\n  -h             "
        "Program usage and help.\n  -v             Print compression statistics.\n  -i infile      "
        "Input file to decompress.\n  -o outfile     Output of decompressed data.\n",
        cwd);
}

// Helper function to print the tree of a root node.
void print_tree(Node *root) {
    if (!root->left || !root->right)
        return;
    node_print(root);
    printf(" points to: ");
    node_print(root->left);
    printf(" & ");
    node_print(root->right);
    printf("\n");
    if (root->left)
        print_tree(root->left);
    if (root->right)
        print_tree(root->right);
}

int main(int argc, char **argv) {
    int64_t opt = 0;
    int infile = 0, outfile = 1;
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
            break;
        case 'o':
            outfile = open(optarg, O_WRONLY);
            // If file not found, create the file.
            if (outfile == -1) {
                outfile = creat(optarg, S_IRWXU);
            }
            break;
        case 'v': verbose = true; break;
        case '?': help_string(cwd); return 0;
        }
    }

    // Create a Header.
    Header h;
    if (read_bytes(infile, (uint8_t *) &h, sizeof(Header)) < (int) sizeof(Header)) {
        fprintf(stderr, "Failed to open file for decoding.\n");
        return 1;
    }
    if (h.magic != MAGIC) {
        fprintf(stderr, "Failed to verify header's magic number.\n");
        return 1;
    }
    fchmod(outfile, h.permissions);

    // Reading the tree dump and rebuilding the tree.
    uint8_t *buf = (uint8_t *) calloc(h.tree_size, sizeof(uint8_t));
    if (read_bytes(infile, buf, (int) h.tree_size) < (int) h.tree_size) {
        fprintf(stderr, "Failed to read tree dump.\n");
        return 1;
    }
    Node *tree = rebuild_tree(h.tree_size, buf);
    // print_tree(tree);

    // Decoding the compressed file. Read bits one at a time while updating *node.
    Node *node = tree;
    uint64_t decoded_bytes = 0;
    while (decoded_bytes < h.file_size) {
        read_bit(infile, buf);
        if (*buf == 0)
            node = node->left;
        else
            node = node->right;
        if (!node) {
            fprintf(stderr, "Error: Invalid encoded file.\n");
            return 1;
        }
        if (!node->left && !node->right) {
            *buf = node->symbol;
            decoded_bytes++;
            //printf("Decoded byte #%" PRIu64 ": %c\n", decoded_bytes, *buf);
            write_bytes(outfile, buf, 1);
            node = tree;
        }
    }

    // Print verbose statistics if requested.
    if (verbose) {
        double space_saving = ((double) bytes_read) / ((double) bytes_written);
        fprintf(stdout,
            "Compressed file size: %" PRIu64 " bytes\nDecompressed file size: %" PRIu64
            " bytes\nSpace saving: %4.2lf%%\n",
            bytes_read, bytes_written, 100 * (1 - space_saving));
    }

    // Memory cleanup.
    close(infile);
    close(outfile);

    delete_tree(&tree);
    tree = NULL;
    node = NULL;
    free(buf);
    buf = NULL;

    return 0;
}
