#include "includes/lib3man.h"
#include <stddef.h>
#include <stdio.h>


typedef struct{
    u16 seg;
    u32 size;
    u32 res;
    u32 offset;
    i32 width;
    i32 height;
    u32 bps;
    u32 compression;
} bmpheader;

bmpheader bmp_get_header(Buffer buffer){
    bmpheader header = {0};


    header.seg = *(u16 *)buffer.buf;
    header.size = *(u32 *)(buffer.buf + 2);
    header.res = *(u32 *)(buffer.buf + 6);
    header.offset = *(u32 *)(buffer.buf + 10);
    header.width = *(i32 *)(buffer.buf + 18);
    header.height = *(i32 *)(buffer.buf + 22);
    header.bps = *(u32 *)(buffer.buf + 0x1c);
    header.compression = *(i32 *)(buffer.buf + 30);

    return header;
}

int main(){
    
    Buffer buffer = buffer_read_file("test.bmp");


    // printf("\n%u", *(unsigned int *)buffer.buf);
    bmpheader header = bmp_get_header(buffer);
    FILE * f = fopen("test.ppm", "wb");

    // u8 * data = (u8 *)(buffer.buf + 0x436);
    printf("%.2s, %u, %u \n", (char *)&header.seg, header.size, header.offset);
    printf("%d %d %u %u %x\n", header.width, header.height, header.bps, header.offset,header.compression);
    for(size_t i = header.offset; i < header.offset + 817920; i++){
        printf("%u \n", buffer.buf[i]);
    }
    // trying to write a ppm image to see how the bytes look
    // P6
    // 640 426
    // 255
    fwrite("P6\n", 1, 3, f);
    fwrite("640 426\n", 1, 8, f);
    fwrite("255\n", 1, 4, f);
    // the bytes are flipped and pixels are inverted BGR instead of RGB
    fwrite(&buffer.buf[header.offset], 1, 817920, f);
}
