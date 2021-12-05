#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "node.h"

Node *node_create(char *oldspeak, char *newspeak){
	Node *n = (Node *)malloc(sizeof(Node));
	n->left = NULL;
	n->right = NULL;
	if(oldspeak) n->oldspeak = strdup(oldspeak);
	else n->oldspeak = NULL;
	if(newspeak) n->newspeak = strdup(newspeak);
	else n->newspeak = NULL;
	return n;
}
void node_delete(Node **n){
	if((*n)->oldspeak){
		free((*n)->oldspeak);
		(*n)->oldspeak = NULL;
	}
	if((*n)->newspeak){
		free((*n)->newspeak);
		(*n)->newspeak = NULL;
	}
	if(*n){
		free(*n);
		*n = NULL;
	}
}
void node_print(Node *n){
	if(n->oldspeak && n->newspeak)
		printf("%s -> %s\n", n->oldspeak, n->newspeak);
	else if(n->oldspeak)
		printf("%s\n", n->oldspeak);
}
