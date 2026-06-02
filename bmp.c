#include "includes/lib3man.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

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
    u16 planes;
    u16 bits_per_pixel;
    u32 compression;
    u32 bitmap_data_size;
    u32 hresolution;
    u32 vresolution;
    u32 colors;
    u32 important_colors;
    u32 palette;
} bmp_header;

void ppm_write(Image img, const char * file_name);
void bmp_write(Image img, const char * file_name);

bmp_header bmp_get_header(Buffer buffer){
    bmp_header header = {0};

    header.segniture = *(u16 *)buffer.buf;
    header.file_size = *(u32 *)(buffer.buf + 0x2);
    header.reserved = *(u32 *)(buffer.buf + 0x6);
    header.bit_map_offset = *(u32 *)(buffer.buf + 0xa);
    header.bmp_header_size = *(u32 *)(buffer.buf + 0xe);
    header.width = *(i32 *)(buffer.buf + 0x12);
    header.height = *(i32 *)(buffer.buf + 0x16);
    header.planes = *(u16 *)(buffer.buf + 0x1a);
    header.bits_per_pixel = *(u16 *)(buffer.buf + 0x1c);
    header.compression = *(u32 *)(buffer.buf + 0x1e);
    header.bitmap_data_size = *(u32 *)(buffer.buf + 0x22);
    header.hresolution = *(u32 *)(buffer.buf + 0x26);
    header.vresolution = *(u32 *)(buffer.buf + 0x2a);
    header.colors = *(u32 *)(buffer.buf + 0x2e);
    header.important_colors = *(u32 *)(buffer.buf + 0x32);
    header.palette = *(u32 *)(buffer.buf + 0x36);

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
    bmp_write(img, "test2.bmp");
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

void bmp_write(Image img, const char * file_name){
    // TODO : FINISH THIS 
    assert(img.data != NULL && img.width > 0
        && img.width > 0 && img.bit_depth > 0
    );
    u8 padding = (img.width * img.height / (img.bit_depth / 8) ) % 4;
    printf("padding %u\n", padding);
    FILE * bitmp_file = fopen(file_name, "wb");

    if(bitmp_file == NULL ){
        printf("BMP FILE WRITING/OPENNING FAILED\n");
        return;
    }
    // signiture
    fwrite("BM", 1, 2, bitmp_file);
    // file_size
    u32 file_size = 54 + ((img.width * img.height) * (img.bit_depth / 8) + (padding * img.height) );
    fwrite(&file_size, 4, 1, bitmp_file);
    // reserved 
    u32 res = 0;
    fwrite(&res, 4, 1, bitmp_file);
    // byte offset 
    u32 boff = 54;
    fwrite(&boff, 4, 1, bitmp_file);
    // size of the header
    u32 sfh = 40;
    fwrite(&sfh, 4, 1, bitmp_file);
    // width 
    fwrite(&img.width, 4, 1, bitmp_file);
    //heigt
    i32 height = -img.height;
    fwrite(&height, 4, 1, bitmp_file);
    //planes
    u16 planes= 1;
    fwrite(&planes, 2, 1, bitmp_file);
    //bit count
    fwrite(&img.bit_depth, 2, 1, bitmp_file);
    // end of  header 
    u32 fill = 0;
    fwrite(&fill, 4, 6, bitmp_file);
    for(size_t i = 0; i < img.height * (img.bit_depth  / 8); i++){
        u8 * data = &img.data[img.width * i];
        fwrite(data, img.width, 1, bitmp_file);
        if(padding > 0)
            fwrite(&fill, 1, padding, bitmp_file);

    }

    fclose(bitmp_file);
}