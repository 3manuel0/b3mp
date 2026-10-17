#ifndef B3MP
#define B3MP
#include "lib3man.h"
#include <stdlib.h>

typedef struct{
    u8 * data;
    u32 width;
    u32 height;
    u32 bit_depth;
} Image;

typedef struct{
    u16 segniture;
    u32 file_size;
    u32 reserved;
    u32 bit_map_offset;
} bmp_file_header;

typedef struct{
    u32 dip_header_size;
    i32 width;
    i32 height;
    u16 planes;
    u16 bits_per_pixel;
    u32 compression;
    u32 bitmap_data_size;
    u32 hresolution;
    u32 vresolution;
    u32 colors;
    u32 important_colors;
    u32 palette;
}bmp_dip_header;

void ppm_write(Image img, const char * file_name);
void bmp_write(Image img, const char * file_name);

#endif 