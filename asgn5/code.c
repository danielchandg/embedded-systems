#include "defines.h"
#include "code.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// Code is a bit vector of size 256.
Code code_init(void) {
    Code c;
    c.top = 0;
    for (int i = 0; i < MAX_CODE_SIZE; i++)
        c.bits[i] = 0;
    return c;
}
uint32_t code_size(Code *c) {
    return c->top;
}
bool code_empty(Code *c) {
    return c->top == 0;
}
bool code_full(Code *c) {
    return c->top == ALPHABET;
}
bool code_set_bit(Code *c, uint32_t i) {
    if (i > c->top)
        return false;
    c->bits[i / 8] |= (1 << i % 8);
    return true;
}
bool code_clr_bit(Code *c, uint32_t i) {
    if (i > c->top)
        return false;
    uint8_t val = ~(1 << i % 8);
    c->bits[i / 8] &= val;
    return true;
}
bool code_get_bit(Code *c, uint32_t i) {
    if (i >= c->top)
        return false;
    return c->bits[i / 8] & (1 << i % 8);
}
// Assume the value of bit is 0 or 1; pushed onto code.
bool code_push_bit(Code *c, uint8_t bit) {
    if (c->top == ALPHABET)
        return false;
    if (bit == 1)
        code_set_bit(c, c->top);
    else
        code_clr_bit(c, c->top);
    c->top++;
    return true;
}
bool code_pop_bit(Code *c, uint8_t *bit) {
    if (c->top == 0)
        return false;
    *bit = code_get_bit(c, c->top - 1);
    c->top--;
    return true;
}
void code_print(Code *c) {
    printf("Code: ");
    for (uint32_t i = 0; i < c->top; i++) {
        if (code_get_bit(c, i))
            printf("1");
        else
            printf("0");
        if (i != c->top - 1 && i % 8 == 7)
            printf(" ");
    }
    printf("\n");
}
