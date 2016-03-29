/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines.
 */

#include "mm_alloc.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

struct block *metaHead;
struct block *metaTail;
int firstMalloc = 1;

#define headerSize sizeof(*metaHead)

void *mm_malloc(size_t size) {
    /* YOUR CODE HERE */
    struct block *currentMeta = metaHead;
    if(size == 0){
        return NULL;
    }
    if(firstMalloc){
        firstMalloc = 0;
        metaHead = sbrk(size + headerSize);
        currentMeta = metaHead;
        if (!currentMeta){
            return NULL;
        }
        currentMeta->prev = NULL;
        currentMeta->next = NULL;
        currentMeta->size = size;
        currentMeta->free = 0;
        metaTail = metaHead;
        zeroData(currentMeta);
        return (char *)currentMeta + headerSize;
    }
    while(currentMeta != NULL){
        if((currentMeta->free) && (currentMeta->size >= size)){
            if(currentMeta->size > size+headerSize){
                struct block *newMeta = (struct block *)((char *)currentMeta + headerSize + size);
                newMeta->prev = currentMeta;
                newMeta->next = currentMeta->next;
                newMeta->free = 1;
                newMeta->size = currentMeta->size - size - headerSize;
                zeroData(newMeta);
                currentMeta->next = newMeta;
                currentMeta->free = 0;
                currentMeta->size = size;

            }else{
                currentMeta->free = 0;
            }
            zeroData(currentMeta);
            return (char *)currentMeta + headerSize;
        }
        currentMeta = currentMeta->next;
    }
    if ((currentMeta = sbrk(size+headerSize)) != -1){
        currentMeta->prev = metaTail;
        currentMeta->next = NULL;
        currentMeta->free = 0;
        currentMeta->size = size;
        metaTail->next = currentMeta;
        metaTail = currentMeta;
        zeroData(currentMeta);
        return (char *) currentMeta + headerSize;
    }
    return NULL;
}

void *mm_realloc(void *ptr, size_t size) {
    /* YOUR CODE HERE */
    return NULL;
}

void mm_free(void *ptr) {
    if (ptr){
        struct block *currentMeta = (struct block*)(((char *) ptr) - headerSize);
        struct block *tail = currentMeta;
        struct block *head = currentMeta;
        while((head->prev != NULL) && (head->prev->free)){
            head = head->prev;
        }
        while((tail->next != NULL) && (tail->next->free)){
            tail = tail->next;
        }
        head->next = tail->next;
        head->free = 1;
        head->size = (char *) tail - (char *) head + tail->size;
        zeroData(head);

    }
}

void zeroData(struct block *currentMeta){
    memset((char *)currentMeta+headerSize,0,currentMeta->size);
}