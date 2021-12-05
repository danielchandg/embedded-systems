#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "bv.h"
typedef struct BitVector {
    uint32_t length;
    uint8_t *vector;
} BitVector;
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (!bv)
        return NULL;
    bv->length = length;
    uint32_t num_bytes = (length + 7) / 8;
    bv->vector = (uint8_t *) calloc(num_bytes, sizeof(uint8_t));
    if (!bv->vector)
        return NULL;
    for (uint32_t i = 0; i < num_bytes; i++)
        bv->vector[i] = 0;
    return bv;
}
void bv_delete(BitVector **bv) {
    if ((*bv)->vector) {
        free((*bv)->vector);
        (*bv)->vector = NULL;
    }
    if (*bv) {
        free(*bv);
        *bv = NULL;
    }
}
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i >= bv->length)
        return false;
    bv->vector[i / 8] |= (1 << i % 8);
    return true;
}
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i >= bv->length)
        return false;
    uint8_t val = ~(1 << i % 8);
    bv->vector[i / 8] &= val;
    return true;
}
bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (i >= bv->length)
        return false;
    return bv->vector[i / 8] & (1 << i % 8);
}
void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv->length; i++) {
        printf("%d", bv_get_bit(bv, i));
        if (i != bv->length - 1 && i % 64 == 63)
            printf("\n");
        else if (i != bv->length - 1 && i % 8 == 7)
            printf(" ");
    }
    printf("\n");
}
