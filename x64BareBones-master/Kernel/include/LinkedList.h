#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdint.h>
#include "process.h"

typedef struct Node {
    void * info;
    struct Node * next;
    struct Node * previous;
} Node;

typedef struct LinkedListCDT * LinkedListADT;

LinkedListADT initializeLinkedList();
void queue(LinkedListADT list, Node * node);
void push(LinkedListADT list, Node * node);
void * dequeue(LinkedListADT list);
void removeFromQueue(LinkedListADT list, Node * node);
int isEmpty(LinkedListADT list);
int listLength(LinkedListADT list);
int listHasNext(LinkedListADT list);
void * listNext(LinkedListADT list);
void listIterator(LinkedListADT list);




#endif
