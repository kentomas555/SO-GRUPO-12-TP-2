#include "include/LinkedList.h"
#incldue "include/MemoryManager.h"

typedef struct LinkedListCDT {
    node * current;
    node * first;
    node * last;
    int size;
} LinkedListCDT;

LinkedListADT initializeLinkedList() {
    LinkedListADT list = (LinkedListADT) allocMemory(sizeof(LinkedListCDT));
    list->current = NULL;
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
    return list;
}

void queue(LinkedListADT list, node * node) {
    if(list->size == 0) {
        list->first = node;
        list->last = node;
        node->previous = NULL;
        node->next = NULL;
    }
    else {
        node->previous = list->last;
        node->next = NULL;
        list->last->next = node;
        list->last = node;
    }
    list->size++;
}

void push(LinkedListADT list, node * node){
    if(list->size == 0) {
        list->first = node;
        list->last = node;
        node->previous = NULL;
        node->next = NULL;
    }
    else {
        node->next = list->first;
        node->previous = NULL;
        list->first->previous = node;
        list->first = node;
    }
    list->size++;    
}

void * dequeue(LinkedListADT list) {
    if(list->size == 0) {
        return NULL;
    }
    node * nodeToFree = list->first;
    void * info = nodeToFree->info;
    list->first = nodeToFree->next;
    if(list->first == NULL) {
        list->last == NULL;
    }
    else {
        list->first->previous = NULL;
    }
    list->size--;
    freeMemory(nodeToFree);
    return info;
    
}

void remove(LinkedListADT list, node * node){
    if (list == NULL || node == NULL || list->size == 0){
        return;
    }

    if(node->next != NULL){
        node->next->previous = node->previous;
    }
    else{
        list->last = node->previous;
    }

    if(node->previous != NULL){
        node->previous->next = node->next;
    }
    else{
        list->first = node->next;
    }

    list->size--;
    return;
}

int isEmpty(LinkedListADT list){
    if(list == NULL){
        return 0;
    }
    return list->size == 0;
}

int listLength(LinkedListADT list){
    if(list == NULL){
        return 0;
    }
    return list->size;
}

int listHasNext(LinkedListADT list){
    if(list == NULL){
        return 0;
    }
    
    return list->current != NULL;
}

void * listNext(LinkedListADT list){
    if(list != NULL && listHasNext(list)){
        node * aux = list->current;
        list->current = aux->next;
        return aux;
    }
    return NULL;
}

//TO-DO freeList(LinkedListADT list)