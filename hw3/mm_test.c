#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

/* Function pointers to hw3 functions */
void* (*mm_malloc)(size_t);
void* (*mm_realloc)(void*, size_t);
void (*mm_free)(void*);

void load_alloc_functions() {
    void *handle = dlopen("hw3lib.so", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    char* error;
    mm_malloc = dlsym(handle, "mm_malloc");
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    mm_realloc = dlsym(handle, "mm_realloc");
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    mm_free = dlsym(handle, "mm_free");
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }
}

void test(){

    int *A = (int*) mm_malloc(sizeof(int)*15);
    int *B = (int*) mm_malloc(sizeof(int)*10);
    B[0] = 30;
    B[9] = 100;
    mm_free(A);
    int *C = (int*) mm_malloc(sizeof(int)*20000);
    C[0] = 10;
    C[9] = 20;
    C[11] = 21;
    C[12] = 22;
    C[13] = 23;
    C[14] = 24;
    C[15] = 25;
    C[16] = -10;
    C[17] = -20;
    int *newC = mm_realloc(C, sizeof(int)*10); //Should replace A's block
    printf("%p\n%p\n%p\n%p\n", A, B, C, newC);
    printf("B's First element: %d \nB's last element: %d\n",B[0], B[9]);
    printf("New C's last element:%d\n", newC[9]);
    mm_free(B);
    mm_free(newC);
}

int main() {
    load_alloc_functions();
    test();

    int *data = (int*) mm_malloc(sizeof(int));
    assert(data != NULL);
    data[0] = 0x162;
    mm_free(data);
    printf("malloc test successful!\n");
    return 0;
}
