/* date = July 31st 2021 0:13 pm */

#ifndef SDF_PLOT_H
#define SDF_PLOT_H


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

typedef struct
{
	u32 width;
	u32 height;
	u32 *pixels;
}SDFImageU32;


#endif //SDF_PLOT_H
