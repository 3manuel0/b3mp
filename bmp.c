#include "includes/lib3man.h"

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

bmp_file_header bmp_get_file_header(Buffer * buffer){
    bmp_file_header file_header = {0};
    file_header.segniture = *(u16 *)buffer_get_next_sz(buffer, sizeof(u16));//*(u16 *)buffer.buf;
    file_header.file_size = *(u32 *)buffer_get_next_sz(buffer, sizeof(u32));//*(u32 *)(buffer.buf + 0x2);
    file_header.reserved = *(u32 *)buffer_get_next_sz(buffer, sizeof(u32));//*(u32 *)(buffer.buf + 0x6);
    file_header.bit_map_offset = *(u32 *)buffer_get_next_sz(buffer, sizeof(u32));//*(u32 *)(buffer.buf + 0xa);
    return file_header;
}

// this is onlt one dip header type handle more 
bmp_dip_header bmp_get_dip_header(Buffer * buffer){
    bmp_dip_header dip_header = {0};
    dip_header.dip_header_size = *(u32 *)buffer_get_next_sz(buffer, sizeof(u32));//*(u32 *)(buffer.buf + 0xe);
    dip_header.width = *(u32 *)buffer_get_next_sz(buffer, sizeof(u32));//*(i32 *)(buffer.buf + 0x12);
    dip_header.height = *(i32 *)buffer_get_next_sz(buffer, sizeof(i32));//*(i32 *)(buffer.buf + 0x16);
    dip_header.planes = *(u16 *)buffer_get_next_sz(buffer, sizeof(u16));//*(u16 *)(buffer.buf + 0x1a);
    dip_header.bits_per_pixel = *(u16 *)buffer_get_next_sz(buffer, sizeof(u16));//*(u16 *)(buffer.buf + 0x1c);
    dip_header.compression = *(u32 *)buffer_get_next_sz(buffer, sizeof(u32));//*(u32 *)(buffer.buf + 0x1e);
    dip_header.bitmap_data_size = *(u32 *)buffer_get_next_sz(buffer, sizeof(u32));//*(u32 *)(buffer.buf + 0x22);
    dip_header.hresolution = *(u32 *)buffer_get_next_sz(buffer, sizeof(u32));//*(u32 *)(buffer.buf + 0x26);
    dip_header.vresolution = *(u32 *)buffer_get_next_sz(buffer, sizeof(u32));//*(u32 *)(buffer.buf + 0x2a);
    dip_header.colors = *(u32 *)buffer_get_next_sz(buffer, sizeof(u32));//*(u32 *)(buffer.buf + 0x2e);
    dip_header.important_colors = *(u32 *)buffer_get_next_sz(buffer, sizeof(u32));//*(u32 *)(buffer.buf + 0x32);
    dip_header.palette = *(u32 *)buffer_get_next_sz(buffer, sizeof(u32));//*(u32 *)(buffer.buf + 0x36);

    return dip_header;
}

int main(){
    
    Buffer buffer = buffer_read_file("test.bmp");
    // printf("\n%u", *(unsigned int *)buffer.buf);
    bmp_file_header fheader = bmp_get_file_header(&buffer);
    bmp_dip_header header = bmp_get_dip_header(&buffer);
    // u8 * data = (u8 *)(buffer.buf + 0x436);
    // printf("%.2s, %u, %u \n", (char *)&header.segniture, header.file_size, header.bit_map_offset);
    printf("%d %d %u %u %x\n", header.width, header.height, header.bits_per_pixel, fheader.bit_map_offset,header.compression);
    u8 *img_buff = malloc(header.width * header.height * (header.bits_per_pixel / 8));

    size_t j = header.width * header.height * (header.bits_per_pixel / 8);
    // fliping the image
    for (size_t i = 0; i < header.height; i ++){
        // img_buff[i] = buffer.buf[header.offset + j];
        j -= (header.width * 3);
        memcpy(&img_buff[i * (header.width * 3)], &buffer.buf[fheader.bit_map_offset + j], header.width * 3);

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

    assert(img.data != NULL && img.width > 0
        && img.width > 0 && img.bit_depth > 0
    );
    u8 bytes_per_pixel = (img.bit_depth / 8);
    u8 padding = (img.width * img.height / bytes_per_pixel ) % 4;
    u32 img_data_size = img.width * img.height * bytes_per_pixel;
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
    char * s = "3MAN";//my signature
    u32 res = *(u32 *)s;
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
    i32 height = img.height;
    fwrite(&height, 4, 1, bitmp_file);
    //planes
    u16 planes= 1;
    fwrite(&planes, 2, 1, bitmp_file);
    //bit count
    fwrite(&img.bit_depth, 2, 1, bitmp_file);
    // end of  header 
    u32 fill = 0;
    fwrite(&fill, 4, 6, bitmp_file);

    // from GBR TO RGB or GBRA TO RGBA 
    for(size_t i = 0; i < img_data_size; i += bytes_per_pixel){
        u8 temp = img.data[i];
        img.data[i] = img.data[i + 2];
        img.data[i + 2] = temp;
    }

    // saving the image with flipped rows top -> bottom
    for(size_t i = img.height; i > 0; ){
        i--;
        u8 * data = &img.data[img.width * i * bytes_per_pixel];
        fwrite(data, img.width * bytes_per_pixel, 1, bitmp_file);
        if(padding > 0)
            fwrite(&fill, 1, padding, bitmp_file);

    }

    // for(size_t i = 0; i < img.height; i++){
    //     u8 * data = &img.data[img.width * i * bytes_per_pixel];
    //     fwrite(data, img.width * bytes_per_pixel, 1, bitmp_file);
    //     if(padding > 0)
    //         fwrite(&fill, 1, padding, bitmp_file);

    // }

    fclose(bitmp_file);
}