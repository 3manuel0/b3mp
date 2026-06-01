#include "includes/lib3man.h"
#include <stddef.h>

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
    u32 bmp_header_size;
    i32 width;
    i32 height;
    u32 planes;
    u32 bits_per_pixel;
    u32 compression;
    u32 bitmap_data_size;
} bmp_header;

void ppm_write(Image img, const char * file_name);

bmp_header bmp_get_header(Buffer buffer){
    bmp_header header = {0};

    header.segniture = *(u16 *)buffer.buf;
    header.file_size = *(u32 *)(buffer.buf + 0x2);
    header.reserved = *(u32 *)(buffer.buf + 0x6);
    header.bit_map_offset = *(u32 *)(buffer.buf + 0xa);
    header.bmp_header_size = *(u32 *)(buffer.buf + 0xe);
    header.width = *(i32 *)(buffer.buf + 0x12);
    header.height = *(i32 *)(buffer.buf + 0x16);
    header.planes = *(u32 *)(buffer.buf + 0x1a);
    header.bits_per_pixel = *(u32 *)(buffer.buf + 0x1c);
    header.compression = *(u32 *)(buffer.buf + 0x1e);
    header.bitmap_data_size = *(u32 *)(buffer.buf + 0x22);
    return header;
}

int main(){
    
    Buffer buffer = buffer_read_file("test.bmp");
    // printf("\n%u", *(unsigned int *)buffer.buf);
    bmp_header header = bmp_get_header(buffer);
    // u8 * data = (u8 *)(buffer.buf + 0x436);
    printf("%.2s, %u, %u \n", (char *)&header.segniture, header.file_size, header.bit_map_offset);
    printf("%d %d %u %u %x\n", header.width, header.height, header.bits_per_pixel, header.bit_map_offset,header.compression);
    u8 *img_buff = malloc(817920);

    size_t j = 817920;
    // fliping the image
    for (size_t i = 0; i < header.height; i ++){
        // img_buff[i] = buffer.buf[header.offset + j];
        j -= (header.width * 3);
        memcpy(&img_buff[i * (header.width * 3)], &buffer.buf[header.bit_map_offset + j], header.width * 3);

    }

    // from BGR to RGB
    for(size_t i = 0; i < header.width * header.height * (header.bits_per_pixel/ 8); i += 3){
        u8 temp = img_buff[i];
        img_buff[i] = img_buff[i + 2];
        img_buff[i + 2] = temp;
    }

    Image img = {
        .width = header.width, 
        .height = header.height, 
        .bit_depth = header.bits_per_pixel,
        // .data = &buffer.buf[header.offset]
        .data = img_buff,
    };
    
    ppm_write(img, "test.ppm");
    return 0;
}


void ppm_write(Image img, const char * file_name){

    FILE * f = fopen(file_name, "wb");

    if(f == NULL){
        printf("ERROR OPENNING THE PPM FILE\n");
        return;
    }

    fwrite("P6\n", 1, 3, f);
    i8 buffer[24];
    u32 len = snprintf((char *)buffer, 24, "%u %u\n", img.width, img.height);
    fwrite(buffer, 1, len, f);
    fwrite("255\n", 1, 4, f);
    fwrite(img.data, 1, img.width * img.height * (img.bit_depth / 8), f);
    fclose(f);
}