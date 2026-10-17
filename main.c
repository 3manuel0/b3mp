#include "bmp.c"


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
    
    // testing
    Image img2 = {};
    img2.data = malloc(800 * 600 * 4);
    img2.bit_depth = 32;
    img2.height = 600;
    img2.width = 800;
    for(size_t i = 0; i < img2.height * img2.width * (img2.bit_depth / 8); i++)
        img2.data[i] = 255;
    ppm_write(img, "test.ppm");
    bmp_write(img, "test2.bmp");
    ppm_write(img2, "test3.ppm");
    bmp_write(img2, "test3.bmp");//generating a white bmp image
    return 0;
}
