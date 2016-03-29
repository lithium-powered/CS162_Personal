/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines.
 */

#include "mm_alloc.h"
#include <stdlib.h>
#include <unistd.h>

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
        metaHead = (struct block*) sbrk(0);
        firstMalloc = 0;
        currentMeta = sbrk(size + headerSize);
        if (!currentMeta){
            return NULL;
        }
        currentMeta->prev = NULL;
        currentMeta->next = NULL;
        currentMeta->size = size;
        currentMeta->free = 0;
        metaTail = metaHead;
        return currentMeta + headerSize;
    }
    while(currentMeta != NULL){
        if((currentMeta->free)&& (currentMeta->size >= size+headerSize)){
            if(currentMeta->size > size+2*headerSize){
                struct block *newMeta = currentMeta + size+headerSize;
                newMeta->prev = currentMeta;
                newMeta->next = currentMeta->prev;
                newMeta->size = currentMeta->size - size - headerSize;
                currentMeta->next = newMeta;
                currentMeta->size = size;
                return currentMeta + headerSize;
            }else{
                currentMeta->free = 0;
                return currentMeta + headerSize;
            }
        }
        currentMeta = currentMeta->next;
    }
    if ((currentMeta = sbrk(size+headerSize))){
        currentMeta->prev = metaTail;
        currentMeta->next = NULL;
        currentMeta->free = 0;
        currentMeta->size = size;
        metaTail->next = currentMeta;
        metaTail = currentMeta;
        return metaTail;

    }
    return NULL;
}

void *mm_realloc(void *ptr, size_t size) {
    /* YOUR CODE HERE */
    return NULL;
}

void mm_free(void *ptr) {
    struct block *currentMeta = (struct block*) (ptr - headerSize);
    currentMeta->free = 1;
}