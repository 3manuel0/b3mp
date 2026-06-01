#include "includes/lib3man.h"
#include <stddef.h>

typedef struct{
    u8 * data;
    u32 width;
    u32 height;
    u32 bit_depth;
} Image;

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

void ppm_write(Image img, const char * file_name);

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
    // u8 * data = (u8 *)(buffer.buf + 0x436);
    printf("%.2s, %u, %u \n", (char *)&header.seg, header.size, header.offset);
    printf("%d %d %u %u %x\n", header.width, header.height, header.bps, header.offset,header.compression);
    u8 *img_buff = malloc(817920);

    size_t j = 817920;
    // fliping the image
    for (size_t i = 0; i < header.height; i ++){
        // img_buff[i] = buffer.buf[header.offset + j];
        j -= (header.width * 3);
        memcpy(&img_buff[i * (header.width * 3)], &buffer.buf[header.offset + j], header.width * 3);

    }

    // from BGR to RGB
    for(size_t i = 0; i < header.width * header.height * (header.bps/ 8); i += 3){
        u8 temp = img_buff[i];
        img_buff[i] = img_buff[i + 2];
        img_buff[i + 2] = temp;
    }

    Image img = {
        .width = header.width, 
        .height = header.height, 
        .bit_depth = header.bps,
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