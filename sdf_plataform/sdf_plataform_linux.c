#include "sdf_plataform.h"

#include <sys/mman.h>

void *
sdf_virtual_alloc(SdfU64 length)
{
    void *mem = mmap(0, length, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
    return mem;
}

int
sdf_virtual_free(void *mem, SdfU64 length)
{
    return munmap(mem, length);
}