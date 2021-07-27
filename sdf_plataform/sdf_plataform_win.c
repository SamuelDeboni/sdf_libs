#include "sdf_plataform.h"

#include <windows.h>

#ifdef SDF_ALLOCATORS


void *
sdf_virtual_alloc(SdfU64 length)
{
    void *mem = VirtualAlloc(0, length, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    return mem;
}

int
sdf_virtual_free(void *mem, SdfU64 length)
{
    return VirtualFree(mem, 0, MEM_RELEASE);
}

#endif // SDF_ALLOCATORS