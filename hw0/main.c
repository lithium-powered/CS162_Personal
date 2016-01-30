#include <stdio.h>
#include <sys/resource.h>

int main() {
    struct rlimit lim;
    printf("stack size: %ld\n", RLIMIT_STACK);
    printf("process limit: %ld\n", RLIMIT_CPU);
    printf("max file descriptors: %ld\n", RLIMIT_NOFILE-1);
    return 0;
}
