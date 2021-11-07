#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "node.h"
#include "code.h"
#include "defines.h"
#include "huffman.h"
#include "pq.h"
#include "stack.h"
#include "io.h"

Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *pq = pq_create(ALPHABET);
    for (int i = 0; i < ALPHABET; i++) {
        if (hist[i] > 0) {
            Node *n = node_create((uint8_t) i, hist[i]);
            enqueue(pq, n);
        }
    }
    Node *a = node_create('a', 0), *b = node_create('a', 0), *c;
    while (pq_size(pq) > 1) {
        dequeue(pq, &a);
        dequeue(pq, &b);
        c = node_create('$', a->frequency + b->frequency);
        c->left = a;
        c->right = b;
        enqueue(pq, c);
    }
    dequeue(pq, &c);
    return c;
}
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
void build_codes(Node *root, Code table[static ALPHABET]) {
    static Code c;
    uint8_t temp = 0;
    uint8_t *bit = &temp;
    if (root != NULL) {
        if (!root->left && !root->right) {
            table[root->symbol] = c;
        } else {
            code_push_bit(&c, 0);
            build_codes(root->left, table);
            code_pop_bit(&c, bit);
            code_push_bit(&c, 1);
            build_codes(root->right, table);
            code_pop_bit(&c, bit);
        }
    }
}
void dump_tree(int outfile, Node *root) {
    if (root) {
        uint8_t *buf = (uint8_t *) calloc(2, sizeof(uint8_t));
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);
        if (!root->left && !root->right) {
            buf[0] = 'L';
            buf[1] = root->symbol;
            write_bytes(outfile, buf, 2);
            // print 'L' + root->symbol
        } else {
            buf[0] = 'I';
            buf[1] = '\0';
            write_bytes(outfile, buf, 1);
            // print 'I'
        }
        free(buf);
        buf = NULL;
    }
}
Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    // read_bytes(infile, tree_dump, tree_size);
    Stack *s = stack_create(nbytes);
    for (uint16_t i = 0; i < nbytes; i++) {
        if (tree[i] == 'L') {
            Node *next = node_create(tree[i + 1], 0);
            stack_push(s, next);
            i++;
        } else if (tree[i] == 'I') {
            Node *a, *b, *c;
            stack_pop(s, &a);
            stack_pop(s, &b);
            c = node_join(a, b);
            stack_push(s, c);
        } else {
            printf("Invalid tree dump: ");
            for (uint16_t j = 0; j < nbytes; j++)
                printf("%" PRIu8, tree[i]);
            printf("\n");
        }
    }
    Node *root;
    stack_pop(s, &root);
    return root;
}
void delete_tree(Node **root) {
    if (*root) {
        delete_tree(&((*root)->left));
        delete_tree(&((*root)->right));
        node_delete(root);
    }
}
