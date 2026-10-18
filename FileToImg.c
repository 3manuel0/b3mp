#include "includes/bmp.h"
#include "bmp.c"
#include "includes/lib3man.h"
#include <stddef.h>
#include <stdlib.h>

i32 writeFileBmp();
i32 writeBmpFile();

int main(){
    writeBmpFile();
    return 0;
}


i32 writeFileBmp(){
    
    // u8 * ext = file_name;
    // while(*ext){
    //     if(*ext == '.'){
    //         break;
    //     }
    //     ext++;
    // }
    // if(*ext == 0) return;
    // ext++;
    // u8 strlen = str_len((i8 *)ext);
    // u32 offset;
    // for(offset = 0; offset < 4; offset++){
    //     image_array[offset] = SIGNITURE[offset];
    // }
    // wprintf("extension= %s %d", ext, size);
    // for(u32 i = 0; i < strlen; offset++){
    //     image_array[offset] = ext[i];
    //     i++;
    // }
    // wprintf("offset = %d", size);
    // image_array[offset] = 0;
    // offset++;
    // u8 * size_in_bytes = (u8 *) &size; 
    // for(i32 i = 0; i < 4; i++){
    //     image_array[offset] = size_in_bytes[i];
    //     wprintf("byte = %d", size_in_bytes[i]);
    //     offset++;
    // } 
    // u32 j = 0;
    // for(u32 i = offset; i < BUFF_SIZE; i++){
    //     image_array[i] = file_array[j];
    //     if(j >= size){
    //         j = 0;
    //     }
    //     j++;
    // }
    return 0;
}

i32 writeBmpFile(){
    
    // TODO : finish this
    Buffer txt =  buffer_read_file("test.txt");
    Image newimg = {.width = 800, .height = 600, .bit_depth = 32, .data = malloc(800 * 600 * 4)};
    
    size_t i = 0;
    char * ext = ".txt";

    for(; i < 4; i++){
        newimg.data[i] = ext[i];
    }

    newimg.data[i] = 0;
    i++;
    printf("i = %zu\n", i);

    *(i32*)(&newimg.data[i]) = txt.size;
    i += 4;

    printf("i = %zu\n", i);

    for(size_t j = 0; j < txt.size; j++){
        newimg.data[i + j] = txt.buf[j];
    }

    bmp_write(newimg, "test.bmp");
    // u32 i = 4;
    // while(image_array[i]){
    //     file_name[i - 4] = image_array[i];
    //     wprintf("%d ext_char %d", i, file_name[i - 4]);
    //     i++;
    // }
    // file_name[i] = 0;
    // i++;
    // u32 file_size = *(u32 *)(image_array + i );
    // i += 4;
    // wprintf("%d", file_size);

    // for(u32 j = 0; j < file_size; j++){
    //     file_array[j] = image_array[i];
    //     i++;
    // }
    // return file_size;
    Image img = bmp_read("test.bmp");
    printf("%p width = %u height = %u bdepth = %u\n",img.data, img.width, img.height, img.bit_depth);

    // fix the bugs 
    for(size_t t = 0; t < 4022/*img.height*img.width*4*/; t++){
        // if(img.data[t] == 't')
            printf("%zu %c\n", t, img.data[t]);
    }
    return 0;
}