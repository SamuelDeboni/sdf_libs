/* date = October 6th 2021 3:28 pm */

#ifndef SDF_IMAGE_H
#define SDF_IMAGE_H


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
    SdfU32 w, h;
    SdfU8 *data;
} SdfImage;

#define PACK __attribute__((__packed__))

typedef  struct {
    SdfU8 id_lenth;
    SdfU8 colour_mapa_type;
    SdfU8 data_type_code;
    
    SdfU16 color_map_origin;
    SdfU16 color_map_length;
    
    SdfU8 color_map_depth;
    
    SdfU16 x_origin;
    SdfU16 y_origin;
    
    SdfU16 width;
    SdfU16 height;
    
    SdfU8 bits_per_pixel;
    SdfU8 image_descriptor;
} PACK TGAHeader;

#undef PACK

SdfImage sdf_image_from_tga_data(SdfU8 *tga_data, SdfU64 tga_data_len, SdfU8 *image_buffer);

#endif //SDF_IMAGE_H

#ifdef SDF_TGA_IMPLEMENTATION

SdfImage
sdf_image_from_tga_data(SdfU8 *tga_data, SdfU64 tga_data_len, SdfU8 *image_buffer)
{
    SdfImage im = {0};
    
    if (!tga_data) return im;
    
    TGAHeader header = *((TGAHeader *)tga_data);
    
    if (header.data_type_code != 10) {
        return im;
    }
    
    if (header.bits_per_pixel != 32 && header.bits_per_pixel != 24) {
        return im;
    }
    
    int byte_advance = header.bits_per_pixel == 32 ? 4 : 3;
    
    SdfU8 *data = tga_data + sizeof(TGAHeader) + header.id_lenth;
    
    im.w = (SdfU32)header.width;
    im.h = (SdfU32)header.height;
    
    if (!image_buffer) return im;
    
    im.data = image_buffer;
    SdfU64 image_data_len = im.w * im.h * 4;
    
    SdfU64 tex_idx = 0;
    for (int idx = 0; idx < tga_data_len; ) {
        SdfU8 pb = data[idx];
        idx += 1;
        int packet_len = (pb & 0x7f);
        
        if ((pb & 0x80) == 0) { // raw packet
            for (int i = 0; i <= packet_len; i += 1) {
                SdfU8 alpha = data[idx + 3];
                SdfU8 alpha_multi = alpha != 0;
                
                im.data[tex_idx]     = data[idx + 2] * alpha_multi;
                im.data[tex_idx + 1] = data[idx + 1] * alpha_multi;
                im.data[tex_idx + 2] = data[idx]     * alpha_multi;
                
                im.data[tex_idx + 3] = header.bits_per_pixel == 32 ? alpha : 255;
                
                tex_idx += 4;
                if (tex_idx >= image_data_len - 3) goto end_loop;
                idx += byte_advance;
            }
        } else { // rl packet
            for (int i = 0; i <= packet_len; i += 1) {
                SdfU8 alpha = data[idx + 3];
                SdfU8 alpha_multi = alpha != 0;
                
                im.data[tex_idx]     = data[idx + 2] * alpha_multi;
                im.data[tex_idx + 1] = data[idx + 1] * alpha_multi;
                im.data[tex_idx + 2] = data[idx]     * alpha_multi;
                
                im.data[tex_idx + 3] = header.bits_per_pixel == 32 ? alpha : 255;
                
                tex_idx += 4;
                if (tex_idx >= image_data_len - 3) goto end_loop;
            }
            idx += byte_advance;
        } 
    }
    
    end_loop:
    
    return im;
}

#endif // SDA_TGA_IMPLEMENTATION

