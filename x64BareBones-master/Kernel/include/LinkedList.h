#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdint.h>

typedef struct Node {
    void * info;
    Node * next;
    Node * previous;
} Node;

typedef struct LinkedListCDT * LinkedListADT;

#endif
