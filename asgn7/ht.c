#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "bst.h"
#include "ht.h"
#include "node.h"
#include "salts.h"
#include "speck.h"
uint64_t lookups = 0;
typedef struct HashTable{
	uint64_t salt[2];
	uint32_t size;
	Node **trees;
}HashTable;
HashTable *ht_create(uint32_t size){
	HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
	ht->salt[0] = SALT_HASHTABLE_LO;
	ht->salt[1] = SALT_HASHTABLE_HI;
	ht->size = size;
	ht->trees = (Node **)calloc(size, sizeof(Node *));
	for(uint32_t i=0; i<ht->size; i++) ht->trees[i] = bst_create();
	return ht;
}
void ht_delete(HashTable **ht){
	for(uint32_t i=0; i<(*ht)->size; i++) bst_delete(&((*ht)->trees[i]));
	free((*ht)->trees);
	(*ht)->trees = NULL;
	free(*ht);
	*ht = NULL;
}
uint32_t ht_size(HashTable *ht){
	return ht->size;
}
Node *ht_lookup(HashTable *ht, char *oldspeak){
	lookups++;
	uint32_t index = hash(ht->salt, oldspeak) % ht->size;
	return bst_find(ht->trees[index], oldspeak);
}
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak){
	lookups++;
	uint32_t index = hash(ht->salt, oldspeak) % ht->size;
	ht->trees[index] = bst_insert(ht->trees[index], oldspeak, newspeak);
}
uint32_t ht_count(HashTable *ht){
	uint32_t ret = 0;
	for(uint32_t i=0; i<ht->size; i++) if(ht->trees[i]) ret++;
	return ret;
}
double ht_avg_bst_size(HashTable *ht){
	double total = 0;
	for(uint32_t i=0; i<ht->size; i++) total += (double)bst_size(ht->trees[i]);
	return total / (double)ht_count(ht);
}
double ht_avg_bst_height(HashTable *ht){
	double total = 0;
	for(uint32_t i=0; i<ht->size; i++) total += (double)bst_height(ht->trees[i]);
	return total / (double)ht_count(ht);
}
void ht_print(HashTable *ht){
	printf("Hash Table:\n");
	for(uint32_t i=0; i<ht->size; i++){
		if(ht->trees[i]){
			printf("%d: %u, %s\n", i, bst_size(ht->trees[i]), ht->trees[i]->oldspeak);
		}
	}
}
