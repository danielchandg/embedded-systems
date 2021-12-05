#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "bv.h"
#include "bf.h"
#include "salts.h"
#include "speck.h"
typedef struct BloomFilter{
	uint64_t primary[2];
	uint64_t secondary[2];
	uint64_t tertiary[2];
	BitVector *filter;
}BloomFilter;
BloomFilter *bf_create(uint32_t size){
	BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
       	bf->primary[0] = SALT_PRIMARY_LO;
	bf->primary[1] = SALT_PRIMARY_HI;
	bf->secondary[0] = SALT_SECONDARY_LO;
	bf->secondary[1] = SALT_SECONDARY_HI;
	bf->tertiary[0] = SALT_TERTIARY_LO;
	bf->tertiary[1] = SALT_TERTIARY_HI;
	bf->filter = bv_create(size);
	if(bf->filter == NULL) printf("Error: BloomFilter creation of BitVector didn't work\n");
	return bf;
}
void bf_delete(BloomFilter **bf){
	if((*bf)->filter) bv_delete(&((*bf)->filter));
	if(*bf){
		free(*bf);
		*bf = NULL;
	}
}
uint32_t bf_size(BloomFilter *bf){
	return bv_length(bf->filter);
}
void bf_insert(BloomFilter *bf, char *oldspeak){
	uint32_t a = hash(bf->primary, oldspeak), b = hash(bf->secondary, oldspeak), c = hash(bf->tertiary, oldspeak);
	a = a % bv_length(bf->filter);
	b = b % bv_length(bf->filter);
	c = c % bv_length(bf->filter);
	bv_set_bit(bf->filter, a);
	bv_set_bit(bf->filter, b);
	bv_set_bit(bf->filter, c);
}
bool bf_probe(BloomFilter *bf, char *oldspeak){
	uint32_t a = hash(bf->primary, oldspeak), b = hash(bf->secondary, oldspeak), c = hash(bf->tertiary, oldspeak);
        a = a % bv_length(bf->filter);
        b = b % bv_length(bf->filter);
        c = c % bv_length(bf->filter);
	return bv_get_bit(bf->filter, a) && bv_get_bit(bf->filter, b) && bv_get_bit(bf->filter, c);	
}
uint32_t bf_count(BloomFilter *bf){
	uint32_t ret = 0;
	for(uint32_t i=0; i<bv_length(bf->filter); i++){
		if(bv_get_bit(bf->filter, i)) ret++;
	}
	return ret;
}
void bf_print(BloomFilter *bf){
	printf("Bloom Filter (%u set bits):\n", bf_count(bf));
	bv_print(bf->filter);	
}
