#include "node.h"
#include "pq.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

typedef struct PriorityQueue {
    Node **A;
    uint32_t top;
    uint32_t capacity;
} PriorityQueue;

// NOTE: This heapsort code creates a minheap because the minimum child is always picked.
uint32_t min_child(Node **A, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first, right = 2 * first + 1;
    return (right <= last && A[right - 1]->frequency < A[left - 1]->frequency) ? right : left;
}
void fix_heap(Node **A, uint32_t first, uint32_t last) {
    uint32_t mother = first, great = min_child(A, first, last);
    //printf("Fix heap with A[0] = ");
    //node_print(*A);
    //printf("\n");
    while (mother <= last / 2) {
        if (A[mother - 1]->frequency > A[great - 1]->frequency) {
            Node c = *A[mother - 1];
            *A[mother - 1] = *A[great - 1];
            *A[great - 1] = c;
            // node_swap(A[mother-1], A[great-1]);
            mother = great;
            great = min_child(A, mother, last);
        } else
            break;
    }
}
void fix_heap2(Node **A, uint32_t first, uint32_t child) {
    uint32_t mother = child >> 1;
    while (mother >= first && A[mother - 1]->frequency > A[child - 1]->frequency) {
        Node c = *A[mother - 1];
        *A[mother - 1] = *A[child - 1];
        *A[child - 1] = c;
        // node_swap(A[mother-1], A[child-1]);
        mother = mother >> 1;
        child = child >> 1;
    }
}
// END heapsort code

PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    pq->top = 0;
    pq->capacity = capacity;
    pq->A = (Node **) calloc(capacity, sizeof(Node *));
    return pq;
}
void pq_delete(PriorityQueue **q) {
    /*for (uint32_t i = 0; i < (*q)->capacity; i++) {
        node_delete(&((*q)->A[i]));
    }*/
    free((*q)->A);
    (*q)->A = NULL;
    free(*q);
    *q = NULL;
}
bool pq_empty(PriorityQueue *q) {
    return q->top == 0;
}
bool pq_full(PriorityQueue *q) {
    return q->top == q->capacity;
}
uint32_t pq_size(PriorityQueue *q) {
    return q->top;
}
bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q))
        return false;
    q->A[q->top] = n;
    q->top++;
    fix_heap2(q->A, 1, q->top);
    return true;
}
bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q))
        return false;
    *n = q->A[0];
    q->top--;
    q->A[0] = q->A[q->top];
    fix_heap(q->A, 1, q->top);
    return true;
}
void pq_print(PriorityQueue *q) {
    for (int i = 0; i < (int) (q->top); i++) {
        int c = i + 1;
        if (i && !(i & c))
            printf("\n");
        node_print(q->A[i]);
    }
    printf("\n");
}
