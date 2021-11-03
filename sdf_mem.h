/* date = July 25th 2021 11:58 am */

#ifndef SDF_MEM_H
#define SDF_MEM_H

#ifndef SDF_assert
#define SDF_assert(x)
#endif

#ifndef SDF_DEFAULT_ALIGNMENT
#define SDF_DEFAULT_ALIGNMENT (2*sizeof(void *))
#endif

#ifndef SDF_INT
#include <stdint.h>
#define SDF_INT

typedef int8_t  SdfI8;
typedef int16_t SdfI16;
typedef int32_t SdfI32;
typedef int64_t SdfI64;

typedef uint8_t  SdfU8;
typedef uint16_t SdfU16;
typedef uint32_t SdfU32;
typedef uint64_t SdfU64;

typedef uintptr_t SdfUPtr;

#endif // SDF_INT


typedef struct {
    void *buffer;
    SdfUPtr buffer_size;
    SdfUPtr offset;
} SdfArena;


#ifdef __cplusplus
extern "C" {
#endif
    void sdf_memset8(void *mem, SdfU8 value, SdfU64 size);
    SdfUPtr sdf_align_forward(SdfUPtr ptr, SdfU64 align);
    
    SdfArena sdf_arena_create(void *buffer, SdfUPtr buffer_size);
    void *sdf_arena_alloc_align(SdfArena *arena, SdfU64 size, SdfU64 align);
    void *sdf_arena_alloc(SdfArena *arena, SdfU64 size);
    void sdf_arena_free(SdfArena *arena, void *ptr);
    void sdf_arena_free_all(SdfArena *arena);
#ifdef __cplusplus
}
#endif


#endif //SDF_MEM_H


#ifdef SDF_MEM_IMPLEMENTATION

inline int
sdf_is_power_of_two(SdfUPtr x) {
	return (x & (x-1)) == 0;
}

void
sdf_memset8(void *mem, SdfU8 value, SdfU64 size)
{
    for (int i = 0; i < size; i++) {
        ((SdfU8 *)mem)[i] = value;
    }
}

SdfUPtr
sdf_align_forward(SdfUPtr ptr, SdfU64 align)
{
    SDF_assert(sdf_is_power_of_two(align));
    
    SdfUPtr p = ptr;
    SdfUPtr a = (SdfUPtr)align;
    SdfUPtr modulo = p & (a - 1);
    
    if (modulo != 0) {
        p += a - modulo;
    }
    
    return p;
}


SdfArena
sdf_arena_create(void *buffer, SdfUPtr buffer_size)
{
    SdfArena result = {buffer, buffer_size};
    return result;
}


void *
sdf_arena_alloc_align(SdfArena *arena, SdfUPtr size, SdfU64 align)
{
    void *result = 0;
    
    SdfUPtr offset = (SdfUPtr)arena->buffer + (SdfUPtr)arena->offset; 
    offset = sdf_align_forward(offset, align);
    offset -= (SdfUPtr)arena->buffer;
    
    if (offset + size <= arena->buffer_size) {
        result = (SdfU8 *)arena->buffer + offset;
        arena->offset = offset + size;
        
        sdf_memset8(result, 0, size);
    }
    
    return result;
}


void *
sdf_arena_alloc(SdfArena *arena, SdfUPtr size)
{
    return sdf_arena_alloc_align(arena, size, SDF_DEFAULT_ALIGNMENT);
}

void
sdf_arena_free(SdfArena *arena, void *ptr) {
    
}


void
sdf_arena_free_all(SdfArena *arena)
{
    sdf_memset8(arena->buffer, 0, arena->buffer_size);
    arena->offset = 0;
}

#endif //SDF_MEM_IMPLEMENTATION
