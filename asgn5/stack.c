#include "node.h"
#include "stack.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// A Stack of Node structs, used for rebuilding a Huffman tree.
typedef struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
} Stack;
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (Node **) calloc(capacity, sizeof(Node *));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}
void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        for (uint32_t i = 0; i < (*s)->top; i++) {
            node_delete(&((*s)->items[i]));
        }
        free((*s)->items);
        (*s)->items = NULL;
        free(*s);
        *s = NULL;
    }
}
bool stack_empty(Stack *s) {
    return s->top == 0;
}
bool stack_full(Stack *s) {
    return s->top >= s->capacity;
}
uint32_t stack_size(Stack *s) {
    return s->top;
}
bool stack_push(Stack *s, Node *n) {
    if (stack_full(s))
        return false;
    s->items[s->top] = n;
    s->top++;
    return true;
}
bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s))
        return false;
    s->top--;
    *n = s->items[s->top];
    return true;
}
void stack_print(Stack *s) {
    for (uint32_t i = 0; i < s->top; i++) {
        node_print(s->items[i]);
        if (i + 1 != s->top)
            printf(" -> ");
    }
    printf("\n");
}
