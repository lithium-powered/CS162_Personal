/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines.
 */

#include "mm_alloc.h"
#include <stdlib.h>

void *metaHead = sbrk(0);
void *metaTail = sbrk(0);
bool firstMalloc = true;

void *mm_malloc(size_t size) {
    /* YOUR CODE HERE */
    struct block *currentMeta = metaHead;
    if(size == 0){
    	return NULL;
    }
    if(firstMalloc){
    	firstMalloc = false;
    	struct block *metaData = sbrk(size + headerSize);
    	*metaData = new block;
    	metaData->size = size;
    	metaData->free = false;
    	metaTail = metaData;
    	return metaData + headersize;
    }
    while(currentMeta != NULL){
    	if(currentMeta->free = TRUE && size)
    	}


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
