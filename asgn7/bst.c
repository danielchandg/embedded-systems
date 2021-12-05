#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "node.h"
#include "bst.h"
uint64_t branches = 0;
Node *bst_create(){
	return NULL;	
}
uint32_t bst_height(Node *root){
	if(root == NULL) return 0;
	uint32_t lheight = bst_height(root->left), rheight = bst_height(root->right);
	return (lheight > rheight ? lheight : rheight) + 1;
}
uint32_t bst_size(Node *root){
	if(root == NULL) return 0;
	return 1 + bst_size(root->left) + bst_size(root->right);
}
Node *bst_find(Node *root, char *oldspeak){
	branches++;
	if(root == NULL) return NULL;
	int comp = strcmp(oldspeak, root->oldspeak);
	if(comp < 0)
		return bst_find(root->left, oldspeak);
	if(comp > 0)
		return bst_find(root->right, oldspeak);
	return root;
}
// Returns Root of updated tree, or NULL if key is already present in the tree.
Node *bst_insert(Node *root, char *oldspeak, char *newspeak){
	branches++;
	if(root == NULL){
		root = node_create(oldspeak, newspeak);
		return root;
	}
	int comp = strcmp(oldspeak, root->oldspeak);
	if(comp < 0)
		root->left =  bst_insert(root->left, oldspeak, newspeak);
	if(comp > 0)
		root->right = bst_insert(root->right, oldspeak, newspeak);
	return root;
}
void bst_print(Node *root){
	if(root == NULL) return;
	bst_print(root->left);
	node_print(root);
	bst_print(root->right);
}
void bst_delete(Node **root){
	if(*root) {
		bst_delete(&((*root)->left));
		bst_delete(&((*root)->right));
		node_delete(root);
	}
}
