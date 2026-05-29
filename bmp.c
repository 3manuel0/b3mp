#include "includes/lib3man.h"


typedef struct{
    u16 seg;
    u32 size;
    u32 res;
    u32 offset;
} bmpheader;

int main(){
    
    Buffer buffer = buffer_read_file("test.bmp");


    // printf("\n%u", *(unsigned int *)buffer.buf);
    bmpheader header = {};
    header.seg = *(u16 *)buffer.buf;
    header.size = *(u32 *)(buffer.buf + 2);
    header.res = *(u32 *)(buffer.buf + 6);
    header.offset = *(u32 *)(buffer.buf + 10);
    i32 width = *(i32 *)(buffer.buf + 18);
    i32 height = *(i32 *)(buffer.buf + 22);
    printf("%.2s, %u, %u \n", (char *)&header.seg, header.size, header.offset);
    printf("%d %d \n", width, height);

}
