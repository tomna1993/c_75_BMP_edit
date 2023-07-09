#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CHARS 50

#define EMODE_FLIP_H "flip_horizontally"
#define EMODE_FLIP_V "flip_vertically"
#define EMODE_TURN_R "turn_right"
#define EMODE_TURN_L "turn_left"

#define BYTES_IN_PIXEL 3

typedef struct BMP_header
{
    // The header field used to identify the BMP and DIB file 
    // is 0x42 0x4D in hexadecimal, same as BM in ASCII
    __int16 ID;

    // The size of the BMP file in bytes
    __int32 Size;

    // Reserved
    __int32 Reserved;

    // Starting address, of the byte where the bitmap image data (pixel array) can be found
    __int32 Image_data_address;
}
BMP_header;
    
typedef struct BMP_BITMAPINFOHEADER
{
    // Size of header, in bytes (40)
    __int32 Size;

    // Bitmap width in pixels (signed integer)
    __int32 BMP_Width;

    // Bitmap height in pixels (signed integer)
    __int32 BMP_Height;

    // Number of color planes (must be 1)
    __int16 Color_Planes;

    // Number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32.
    __int16 Bits_per_pixel;

    // Compression method being used.
    // Val: 0	Identified by: BI_RGB	Compression method: none   	Comment: Most common
    __int32 Compression_method;

    // Image size. This is the size of the raw bitmap data; a dummy 0 can be given for BI_RGB bitmaps.
    __int32 Image_size;

    // Horizontal resolution of the image. (pixel per metre, signed integer)
    __int32 Horiz_res;

    // Vertical resolution of the image. (pixel per metre, signed integer)
    __int32 Vert_res;

    // Number of colors in the color palette, or 0 to default to 2n
    __int32 Num_of_colors;

    // Number of important colors used, or 0 when every color is important; generally ignored
    __int32 Num_of_important_colors;
}
BMP_BITMAPINFOHEADER;


__int8 func_read_BMP_header(char filename[MAX_CHARS], BMP_header *bmp_header, BMP_BITMAPINFOHEADER *bmp_bitmap_info_header);
void func_print_BMP_header(BMP_header *bmp_header, BMP_BITMAPINFOHEADER *bmp_bitmap_info_header);




int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf ("Usage: <program> <BMP file> <edit_mode>\n");
        return EXIT_FAILURE;   
    }

    char BMP_in[MAX_CHARS];
    char edit_mode[MAX_CHARS];

    strcpy_s (BMP_in, MAX_CHARS, argv[1]);
    strcpy_s (edit_mode, MAX_CHARS, argv[2]);
    
    BMP_header bmp_header = { 0 };

    BMP_BITMAPINFOHEADER bmp_bitmap_info_header = { 0 };

    __int8 is_Error = func_read_BMP_header(BMP_in, &bmp_header, &bmp_bitmap_info_header);

    if (is_Error == EXIT_FAILURE)
    {
        printf ("Can not open file!\n");
        return EXIT_FAILURE;   
    }
    
    func_print_BMP_header(&bmp_header, &bmp_bitmap_info_header);
    
    return EXIT_SUCCESS;   

}


__int8 func_read_BMP_header(char filename[MAX_CHARS], BMP_header *bmp_header, BMP_BITMAPINFOHEADER *bmp_bitmap_info_header)
{
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf ("Can not open file!\n");
        return EXIT_FAILURE;
    }

    fread (&bmp_header->ID, sizeof(bmp_header->ID), 1, fp);

    if (bmp_header->ID != 0x4D42)
    {
        printf ("The file is not a BMP image!\n");
        return EXIT_FAILURE;
    }

    fread (&bmp_header->Size, sizeof(bmp_header->Size), 1, fp);

    fread (&bmp_header->Reserved, sizeof(bmp_header->Reserved), 1, fp);

    fread (&bmp_header->Image_data_address, sizeof(bmp_header->Image_data_address), 1, fp);


    // BMP DIB    
    fread (&bmp_bitmap_info_header->Size, sizeof(bmp_bitmap_info_header->Size), 1, fp);
    fread (&bmp_bitmap_info_header->BMP_Width, sizeof(bmp_bitmap_info_header->BMP_Width), 1, fp);
    fread (&bmp_bitmap_info_header->BMP_Height, sizeof(bmp_bitmap_info_header->BMP_Height), 1, fp);
    fread (&bmp_bitmap_info_header->Color_Planes, sizeof(bmp_bitmap_info_header->Color_Planes), 1, fp);
    fread (&bmp_bitmap_info_header->Bits_per_pixel, sizeof(bmp_bitmap_info_header->Bits_per_pixel), 1, fp);
    fread (&bmp_bitmap_info_header->Compression_method, sizeof(bmp_bitmap_info_header->Compression_method), 1, fp);
    fread (&bmp_bitmap_info_header->Image_size, sizeof(bmp_bitmap_info_header->Image_size), 1, fp);
    fread (&bmp_bitmap_info_header->Horiz_res, sizeof(bmp_bitmap_info_header->Horiz_res), 1, fp);
    fread (&bmp_bitmap_info_header->Vert_res, sizeof(bmp_bitmap_info_header->Vert_res), 1, fp);
    fread (&bmp_bitmap_info_header->Num_of_colors, sizeof(bmp_bitmap_info_header->Num_of_colors), 1, fp);
    fread (&bmp_bitmap_info_header->Num_of_important_colors, sizeof(bmp_bitmap_info_header->Num_of_important_colors), 1, fp);

    fclose(fp);

    return EXIT_SUCCESS;
}


void func_print_BMP_header(BMP_header *bmp_header, BMP_BITMAPINFOHEADER *bmp_bitmap_info_header)
{
    printf ("File type: %x\n", bmp_header->ID);

    printf ("The size of the BMP file in bytes: %i\n", bmp_header->Size);

    printf ("Image data starting address: %i\n", bmp_header->Image_data_address);


    // BMP DIB    
    printf ("Size of header, in bytes: %i\n", bmp_bitmap_info_header->Size);
    printf ("Bitmap width in pixels: %i\n", bmp_bitmap_info_header->BMP_Width);
    printf ("Bitmap height in pixels: %i\n", bmp_bitmap_info_header->BMP_Height);
    printf ("Number of color planes: %i\n", bmp_bitmap_info_header->Color_Planes);
    printf ("Number of bits per pixel: %i\n", bmp_bitmap_info_header->Bits_per_pixel);
    printf ("Compression method: %i\n", bmp_bitmap_info_header->Compression_method);
    printf ("Image size: %i\n", bmp_bitmap_info_header->Image_size);
    printf ("Horizontal resolution: %i\n", bmp_bitmap_info_header->Horiz_res);
    printf ("Vertical resolution: %i\n", bmp_bitmap_info_header->Vert_res);
    printf ("Number of colors in the color palette: %i\n", bmp_bitmap_info_header->Num_of_colors);
    printf ("Number of important colors used: %i\n", bmp_bitmap_info_header->Num_of_important_colors);
}