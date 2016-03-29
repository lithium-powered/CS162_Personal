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
        return currentMeta + headerSize;
    }
    while(currentMeta != NULL){
        if((currentMeta->free)&& (currentMeta->size >= size)){
            if(currentMeta->size > size+headerSize){
                struct block *newMeta = currentMeta + headerSize + size;
                newMeta->prev = currentMeta;
                newMeta->next = currentMeta->prev;
                newMeta->size = currentMeta->size - size - headerSize;
                currentMeta->next = newMeta;
                currentMeta->free = 0;
                currentMeta->size = size;

            }else{
                currentMeta->free = 0;
            }
            zeroData(currentMeta);
            return currentMeta + headerSize;
        }
        currentMeta = currentMeta->next;
    }
    /*if ((currentMeta = sbrk(size+headerSize))){
        currentMeta->prev = metaTail;
        currentMeta->next = NULL;
        currentMeta->free = 0;
        currentMeta->size = size;
        metaTail->next = currentMeta;
        metaTail = currentMeta;
        zeroData(currentMeta);
        return metaTail;
    }*/
    return NULL;
}

void *mm_realloc(void *ptr, size_t size) {
    /* YOUR CODE HERE */
    return NULL;
}

void mm_free(void *ptr) {
    if (ptr){
        struct block *currentMeta = (struct block*) (ptr - headerSize);
        currentMeta->free = 1;
        zeroData(currentMeta);
    }
}

void zeroData(struct block *currentMeta){
    memset(currentMeta+headerSize,0,currentMeta->size);
}