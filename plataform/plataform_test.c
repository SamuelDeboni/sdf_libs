#define SDF_ALLOCATORS
#include "sdf_plataform_win.c"

#include <stdio.h>

int
main()
{
    SdfU64 len = 4096;
    SdfU32 *mem = (SdfU32 *)sdf_alloc(len * sizeof(SdfU32));
    printf("mem = %llx\n", (SdfU64)mem);
    
    for (int i = 0; i < len; i++) {
        mem[i] = i;
    }
    
    for (int i = 0; i < len; i++) {
        printf("mem[%d] = %d\n", i, mem[i]);
    }
    
    sdf_free(mem, len * sizeof(SdfU32));
    
    return 0;
}