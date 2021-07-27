/* date = July 25th 2021 8:30 am */

#ifndef SDF_PLATAFORM_H
#define SDF_PLATAFORM_H

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

#endif // SDF_INT

#ifndef SDF_BOOL
#define SDF_BOOL
typedef uint32_t SdfBool;
#define SDF_TRUE  (0 == 0)
#define SDF_FALSE (0 == 1)
#endif


#ifdef __cplusplus
extern "C" {
#endif
    
#ifdef SDF_ALLOCATORS
    void *sdf_virtual_alloc(SdfU64 size);
    int sdf_virtual_free(void *mem, SdfU64 length);
#endif // SDF_ALLOCATORS
    
#ifdef __cplusplus
}
#endif


#endif //SDF_PLATAFORM_H
