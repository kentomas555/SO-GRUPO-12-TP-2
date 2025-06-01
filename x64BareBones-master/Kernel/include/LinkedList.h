#include <stdint.h>

typedef struct node {
    void * info;
    node * next;
    node * previous;
} node;

typedef struct LinkedListCDT * LinkedListADT;
