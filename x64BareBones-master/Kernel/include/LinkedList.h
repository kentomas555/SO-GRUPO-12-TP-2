#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdint.h>

typedef struct Node {
    void * info;
    Node * next;
    Node * previous;
} Node;

LinkedListADT initializeLinkedList();
void queue(LinkedListADT list, Node * node);
void push(LinkedListADT list, Node * node);
void * dequeue(LinkedListADT list);
void remove(LinkedListADT list, Node * node);
int isEmpty(LinkedListADT list);
int listLength(LinkedListADT list);
int listHasNext(LinkedListADT list);
void * listNext(LinkedListADT list);

typedef struct LinkedListCDT * LinkedListADT;

#endif
