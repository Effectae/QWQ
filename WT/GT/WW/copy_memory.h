#include <stdio.h>

typedef struct _COPY_MEMORY
{
        int pid;
        uintptr_t addr;
        void *buffer;
        size_t size;
} COPY_MEMORY, *PCOPY_MEMORY;