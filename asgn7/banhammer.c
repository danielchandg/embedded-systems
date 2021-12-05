#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <regex.h>
#include <errno.h>
#include <unistd.h>
#include "messages.h"
#include "salts.h"
#include "speck.h"
#include "ht.h"
#include "bst.h"
#include "node.h"
#include "bf.h"
#include "bv.h"
#include "parser.h"

#define OPTIONS "ht:f:s"
#define WORD    "[a-zA-Z0-9_]+([-'][a-zA-Z0-9_]+)*"

// Helper function to print help message.
void help_string(char *cwd) {
    fprintf(stdout,
        "SYNOPSIS\n  A word filtering program for the GPRSC.\n  Filters out and reports bad words "
        "parsed from stdin.\n\nUSAGE\n  %s [-hs] [-t size] [-f size]\n\nOPTIONS\n  -h           "
        "Program usage and help.\n  -s           Print program statistics.\n  -t size      Specify "
        "hash table size (default: 2^16).\n  -f size      Specify Bloom filter size (default: "
        "2^20)",
        cwd);
}
int main(int argc, char **argv) {
    int64_t opt = 0;
    uint32_t HashTableSize = (1 << 16), BloomFilterSize = (1 << 20);
    bool stats = false;
    char *cwd = argv[0];
    FILE *badspeakfile = fopen("badspeak.txt", "r"), *newspeakfile = fopen("newspeak.txt", "r");
    if (badspeakfile == NULL || newspeakfile == NULL) {
        fprintf(stderr, "Error: badspeak.txt & newspeak.txt could not be opened.\n");
        return 1;
    }
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help_string(cwd); return 0;
        case 't':
            HashTableSize = strtoul(optarg, NULL, 10);
            if (errno == EINVAL)
                HashTableSize = (1 << 16);
            break;
        case 'f':
            BloomFilterSize = strtoul(optarg, NULL, 10);
            if (errno == EINVAL)
                BloomFilterSize = (1 << 20);
            break;
        case 's': stats = true; break;
        case '?': help_string(cwd); return 1;
        }
    }
    BloomFilter *bf = bf_create(BloomFilterSize);
    HashTable *ht = ht_create(HashTableSize);
    char *badWord = (char *) calloc(2048, sizeof(char));
    char *goodWord = (char *) calloc(2048, sizeof(char));
    int reading = fscanf(badspeakfile, "%s\n", badWord), words_read = 0;
    // Reading badspeak words
    while (reading == 1) {
        words_read++;
        bf_insert(bf, badWord);
        ht_insert(ht, badWord, NULL);
        reading = fscanf(badspeakfile, "%s\n", badWord);
    }
    if (reading != EOF) {
        fprintf(stderr, "Error: badspeak.txt is invalid.\n");
        return 1;
    }
    words_read = 0;
    reading = fscanf(newspeakfile, "%s %s\n", badWord, goodWord);
    // reading oldspeak words
    while (reading == 2) {
        words_read++;
        bf_insert(bf, badWord);
        ht_insert(ht, badWord, goodWord);
        reading = fscanf(newspeakfile, "%s %s\n", badWord, goodWord);
    }
    if (reading != EOF) {
        fprintf(stderr, "Error: newspeak.txt is invalid.\n");
    }
    // Regex expression
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return 1;
    }
    char *word = "hi";
    Node *bad_list = node_create(NULL, NULL), *old_list = node_create(NULL, NULL), *temp = NULL;
    Node *bad_point = bad_list, *old_point = old_list;
    while ((word = next_word(stdin, &re)) != NULL) {
        if (!bf_probe(bf, word))
            continue;
        temp = ht_lookup(ht, word);
        // Word is a false positive from bloom filter.
        if (temp == NULL)
            continue;
        // Word is oldspeak.
        if (temp->newspeak) {
            old_point->left = node_create(temp->oldspeak, temp->newspeak);
            old_point = old_point->left;
        }
        // Word is badspeak.
        else {
            bad_point->left = node_create(temp->oldspeak, NULL);
            bad_point = bad_point->left;
        }
    }
    bad_point = bad_list;
    old_point = old_list;
    if (stats) {
        double HashTableLoad = 100 * (double) ht_count(ht) / (double) ht_size(ht);
        double BloomFilterLoad = 100 * (double) bf_count(bf) / (double) bf_size(bf);
        fprintf(stdout, "Average BST size: %.6lf\n", ht_avg_bst_size(ht));
        fprintf(stdout, "Average BST height: %.6lf\n", ht_avg_bst_height(ht));
        fprintf(
            stdout, "Average branches traversed: %.6lf\n", (double) branches / (double) lookups);
        fprintf(stdout, "Hash table load: %.6lf%%\n", HashTableLoad);
        fprintf(stdout, "Bloom filter load: %.6lf%%\n", BloomFilterLoad);

    } else {
        bad_list = bad_list->left;
        old_list = old_list->left;
        if (bad_list && old_list)
            fprintf(stdout, "%s", mixspeak_message);
        else if (bad_list)
            fprintf(stdout, "%s", badspeak_message);
        else if (old_list)
            fprintf(stdout, "%s", goodspeak_message);
        while (bad_list != NULL) {
            node_print(bad_list);
            bad_list = bad_list->left;
        }
        while (old_list != NULL) {
            node_print(old_list);
            old_list = old_list->left;
        }
    }
    clear_words();
    regfree(&re);
    fclose(badspeakfile);
    fclose(newspeakfile);
    bf_delete(&bf);
    ht_delete(&ht);
    free(badWord);
    badWord = NULL;
    free(goodWord);
    goodWord = NULL;
    free(word);
    word = NULL;
    bst_delete(&bad_point);
    bst_delete(&old_point);
    return 0;
}
