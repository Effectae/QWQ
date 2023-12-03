#include <stdio.h>
typedef struct _MODULE_BASE
{
        int pid;
        char *name;
        uintptr_t base;
} MODULE_BASE, *PMODULE_BASE;