#include "node.h"
#include "defines.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

Node *node_create(uint8_t symbol, uint64_t frequency){
	Node *n = (Node *)malloc(sizeof(Node));
	n->symbol = symbol;
	n->frequency = frequency;
	n->left = NULL;
	n->right = NULL;
	return n;
}
void node_delete(Node **n){
	free(*n);
	*n = NULL;
	return;
}
Node *node_join(Node *left, Node *right){
	Node *n = node_create('$', left->frequency + right->frequency);
	n->left = left;
	n->right = right;
	return n;
}
void node_swap(Node *a, Node *b){
	Node c = *a;
	*a = *b;
	*b = c;
}
void node_print(Node *n){
	if(n) printf("(%c, %" PRIu64 ") ", n->symbol, n->frequency);
}
