/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines.
 */

#include "mm_alloc.h"
#include <stdlib.h>
#include <unistd.h>

struct block *metaHead;
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
        struct block *metaData = sbrk(size + headerSize);
        metaData->size = size;
        metaData->free = 0;
        return metaData + headerSize;
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
    return NULL;
}

void *mm_realloc(void *ptr, size_t size) {
    /* YOUR CODE HERE */
    return NULL;
}

void mm_free(void *ptr) {
    /* YOUR CODE HERE */
}