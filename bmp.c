#include "includes/lib3man.h"


typedef struct{
    u16 seg;
    u32 size;
    u32 res;
    u32 offset;
    i32 width;
    i32 height;
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
    header.compression = *(i32 *)(buffer.buf + 30);

    return header;
}

int main(){
    
    Buffer buffer = buffer_read_file("test.bmp");


    // printf("\n%u", *(unsigned int *)buffer.buf);
    bmpheader header = bmp_get_header(buffer);
    printf("%.2s, %u, %u \n", (char *)&header.seg, header.size, header.offset);
    printf("%d %d %x\n", header.width, header.height, header.compression);

}
