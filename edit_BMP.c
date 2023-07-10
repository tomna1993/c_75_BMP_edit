#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CHARS 50

#define EMODE_FLIP_H "flip_h"
#define EMODE_FLIP_V "flip_v"
#define EMODE_TURN "turn"

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

    // Starting address, of the byte where the bitmap image data 
    // (pixel array) can be found
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

    // Number of bits per pixel, which is the color depth of the image. 
    // Typical values are 1, 4, 8, 16, 24 and 32.
    __int16 Bits_per_pixel;

    // Compression method being used.
    // Val: 0  Identified by: BI_RGB  Compression method: none 	Comment: Most common
    __int32 Compression_method;

    // Image size. This is the size of the raw bitmap data; 
    // a dummy 0 can be given for BI_RGB bitmaps.
    __int32 Image_size;

    // Horizontal resolution of the image. (pixel per metre, signed integer)
    __int32 Horiz_res;

    // Vertical resolution of the image. (pixel per metre, signed integer)
    __int32 Vert_res;

    // Number of colors in the color palette, or 0 to default to 2n
    __int32 Num_of_colors;

    // Number of important colors used, or 0 when every color is important
    __int32 Num_of_important_colors;

    __int32 Num_of_Pixels;

    bool Is_Bottom_Up;

    __int8 Padding_Bytes;
}
BMP_BITMAPINFOHEADER;

typedef struct BMP_Pixel
{
    __int8 Blue;
    __int8 Green;
    __int8 Red;
}
BMP_Pixel;


__int8 read_BMP_header (
    char filename[MAX_CHARS], 
    BMP_header *bmp_header, 
    BMP_BITMAPINFOHEADER *bmp_DIB_header);

void print_BMP_header (
    BMP_header *bmp_header, 
    BMP_BITMAPINFOHEADER *bmp_DIB_header );

__int8 read_pixel_data (
    char filename[MAX_CHARS], 
    BMP_header *bmp_header, 
    BMP_BITMAPINFOHEADER *bmp_bitmap_info_headerm, 
    BMP_Pixel *pixel_data );

__int8 create_new_image (
    char output[MAX_CHARS],
    BMP_header *bmp_header, 
    BMP_BITMAPINFOHEADER *bmp_DIB_header, 
    BMP_Pixel *pixel_data );
    
__int8 turn_left (
    BMP_header *bmp_header, 
    BMP_BITMAPINFOHEADER *bmp_DIB_header,
    BMP_Pixel *pixel_data );


int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf ("Usage: <program> <BMP file> <edit_mode>\n");
        return EXIT_FAILURE;   
    }

    char BMP_in[MAX_CHARS];
    char BMP_out[MAX_CHARS];
    char edit_mode[MAX_CHARS];

    strcpy_s (BMP_in, MAX_CHARS, argv[1]);
    strcpy_s (BMP_out, MAX_CHARS, "Output.bmp");
    strcpy_s (edit_mode, MAX_CHARS, argv[2]);
    
    BMP_header bmp_header = { 0 };

    BMP_BITMAPINFOHEADER bmp_DIB_header = { 0 };

    __int8 is_Error = read_BMP_header(BMP_in, &bmp_header, &bmp_DIB_header);

    if (is_Error == EXIT_FAILURE)
    {
        printf ("Failed to open the file!\n");
        return EXIT_FAILURE;   
    }
    
    print_BMP_header(&bmp_header, &bmp_DIB_header);
    
    BMP_Pixel *pixel_data = calloc (bmp_DIB_header.Num_of_Pixels * BYTES_IN_PIXEL, sizeof(__int8));

    is_Error = read_pixel_data (BMP_in, &bmp_header, &bmp_DIB_header, pixel_data);

    if (is_Error == EXIT_FAILURE)
    {
        printf ("Failed to open the file!\n");

        free(pixel_data);
        pixel_data = NULL;

        return EXIT_FAILURE;   
    }
 
    is_Error = turn_left (&bmp_header, &bmp_DIB_header, pixel_data);

    if (is_Error == EXIT_FAILURE)
    {
        printf ("Failed to run command!\n");

        free(pixel_data);
        pixel_data = NULL;

        return EXIT_FAILURE;   
    }

    is_Error = create_new_image (BMP_out, &bmp_header, &bmp_DIB_header, pixel_data);

    if (is_Error == EXIT_FAILURE)
    {
        printf ("Failed to create the file!\n");

        free(pixel_data);
        pixel_data = NULL;

        return EXIT_FAILURE;   
    }

    free(pixel_data);
    pixel_data = NULL;

    printf ("Editing picture is done!\n");

    return EXIT_SUCCESS;   
}


__int8 read_BMP_header (
    char filename[MAX_CHARS], 
    BMP_header *bmp_header, 
    BMP_BITMAPINFOHEADER *bmp_DIB_header )
{
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf ("Failed to open the file!\n");
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
    fread (&bmp_DIB_header->Size, sizeof(bmp_DIB_header->Size), 1, fp);
    fread (&bmp_DIB_header->BMP_Width, sizeof(bmp_DIB_header->BMP_Width), 1, fp);
    fread (&bmp_DIB_header->BMP_Height, sizeof(bmp_DIB_header->BMP_Height), 1, fp);
    fread (&bmp_DIB_header->Color_Planes, sizeof(bmp_DIB_header->Color_Planes), 1, fp);
    fread (&bmp_DIB_header->Bits_per_pixel, sizeof(bmp_DIB_header->Bits_per_pixel), 1, fp);
    fread (&bmp_DIB_header->Compression_method, sizeof(bmp_DIB_header->Compression_method), 1, fp);
    fread (&bmp_DIB_header->Image_size, sizeof(bmp_DIB_header->Image_size), 1, fp);
    fread (&bmp_DIB_header->Horiz_res, sizeof(bmp_DIB_header->Horiz_res), 1, fp);
    fread (&bmp_DIB_header->Vert_res, sizeof(bmp_DIB_header->Vert_res), 1, fp);
    fread (&bmp_DIB_header->Num_of_colors, sizeof(bmp_DIB_header->Num_of_colors), 1, fp);
    fread (&bmp_DIB_header->Num_of_important_colors, sizeof(bmp_DIB_header->Num_of_important_colors), 1, fp);

    fclose(fp);

    if (bmp_DIB_header->BMP_Height < 0)
    {
        bmp_DIB_header->Is_Bottom_Up = false;   

        bmp_DIB_header->Num_of_Pixels = bmp_DIB_header->BMP_Width * (bmp_DIB_header->BMP_Height * - 1);
    }
    else
    {
        bmp_DIB_header->Is_Bottom_Up = true;   

        bmp_DIB_header->Num_of_Pixels = bmp_DIB_header->BMP_Width * bmp_DIB_header->BMP_Height;
    }

    bmp_DIB_header->Padding_Bytes = ((bmp_DIB_header->BMP_Width * BYTES_IN_PIXEL) % 4);

    if (bmp_DIB_header->Padding_Bytes > 0)
    {
        bmp_DIB_header->Padding_Bytes = 4 - bmp_DIB_header->Padding_Bytes;     
    }

    return EXIT_SUCCESS;
}

void print_BMP_header (
    BMP_header *bmp_header, 
    BMP_BITMAPINFOHEADER *bmp_DIB_header )
{
    printf ("File type: %x\n", bmp_header->ID);
    printf ("The size of the BMP file in bytes: %i\n", bmp_header->Size);
    printf ("Image data starting address: %x\n", bmp_header->Image_data_address);

    // BMP DIB    
    printf ("Size of header, in bytes: %i\n", bmp_DIB_header->Size);
    printf ("Bitmap width in pixels: %i\n", bmp_DIB_header->BMP_Width);
    printf ("Bitmap height in pixels: %i\n", bmp_DIB_header->BMP_Height);
    printf ("Number of color planes: %i\n", bmp_DIB_header->Color_Planes);
    printf ("Number of bits per pixel: %i\n", bmp_DIB_header->Bits_per_pixel);
    printf ("Compression method: %i\n", bmp_DIB_header->Compression_method);
    printf ("Image size: %i\n", bmp_DIB_header->Image_size);
    printf ("Horizontal resolution: %i\n", bmp_DIB_header->Horiz_res);
    printf ("Vertical resolution: %i\n", bmp_DIB_header->Vert_res);
    printf ("Number of colors in the color palette: %i\n", bmp_DIB_header->Num_of_colors);
    printf ("Number of important colors used: %i\n", bmp_DIB_header->Num_of_important_colors);
    printf ("Read BMP from Bottom-up: %i\n", bmp_DIB_header->Is_Bottom_Up);
    printf ("Number of pixels: %i\n", bmp_DIB_header->Num_of_Pixels);
    printf ("Number of padding bytes: %i\n", bmp_DIB_header->Padding_Bytes);
}

__int8 read_pixel_data (
    char filename[MAX_CHARS], 
    BMP_header *bmp_header, 
    BMP_BITMAPINFOHEADER *bmp_DIB_header, 
    BMP_Pixel *pixel_data )
{
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf ("Failed to open the file!\n");
        return EXIT_FAILURE;
    }

    fseek (fp, bmp_header->Image_data_address, SEEK_SET);

    for (__int32 i = 0; i < bmp_DIB_header->Num_of_Pixels; i++)
    {
        if (i > 0 && i % bmp_DIB_header->BMP_Width == 0 && bmp_DIB_header->Padding_Bytes > 0)
        {
            fseek (fp, bmp_DIB_header->Padding_Bytes, SEEK_CUR);
        }

        fread (&(pixel_data + i)->Blue, sizeof(__int8), 1, fp);   
        fread (&(pixel_data + i)->Green, sizeof(__int8), 1, fp);   
        fread (&(pixel_data + i)->Red, sizeof(__int8), 1, fp);   
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

__int8 create_new_image (
    char output[MAX_CHARS],
    BMP_header *bmp_header, 
    BMP_BITMAPINFOHEADER *bmp_DIB_header, 
    BMP_Pixel *pixel_data )
{
    FILE *fp = fopen(output, "wb");

    if (fp == NULL)
    {
        printf ("Failed to create the file!\n");
        return EXIT_FAILURE;
    }

    fwrite (&bmp_header->ID, sizeof(bmp_header->ID), 1, fp);
    fwrite (&bmp_header->Size, sizeof(bmp_header->Size), 1, fp);
    fwrite (&bmp_header->Reserved, sizeof(bmp_header->Reserved), 1, fp);
    fwrite (&bmp_header->Image_data_address, sizeof(bmp_header->Image_data_address), 1, fp);


    // BMP DIB    
    fwrite (&bmp_DIB_header->Size, sizeof(bmp_DIB_header->Size), 1, fp);
    fwrite (&bmp_DIB_header->BMP_Width, sizeof(bmp_DIB_header->BMP_Width), 1, fp);
    fwrite (&bmp_DIB_header->BMP_Height, sizeof(bmp_DIB_header->BMP_Height), 1, fp);
    fwrite (&bmp_DIB_header->Color_Planes, sizeof(bmp_DIB_header->Color_Planes), 1, fp);
    fwrite (&bmp_DIB_header->Bits_per_pixel, sizeof(bmp_DIB_header->Bits_per_pixel), 1, fp);
    fwrite (&bmp_DIB_header->Compression_method, sizeof(bmp_DIB_header->Compression_method), 1, fp);
    fwrite (&bmp_DIB_header->Image_size, sizeof(bmp_DIB_header->Image_size), 1, fp);
    fwrite (&bmp_DIB_header->Horiz_res, sizeof(bmp_DIB_header->Horiz_res), 1, fp);
    fwrite (&bmp_DIB_header->Vert_res, sizeof(bmp_DIB_header->Vert_res), 1, fp);
    fwrite (&bmp_DIB_header->Num_of_colors, sizeof(bmp_DIB_header->Num_of_colors), 1, fp);
    fwrite (&bmp_DIB_header->Num_of_important_colors, sizeof(bmp_DIB_header->Num_of_important_colors), 1, fp);

    fseek (fp, bmp_header->Image_data_address, SEEK_SET);

    for (__int32 i = 0; i < bmp_DIB_header->Num_of_Pixels; i++)
    {
        if (i > 0 && i % bmp_DIB_header->BMP_Width == 0 && bmp_DIB_header->Padding_Bytes > 0)
        {
            for (int j = 0; j < bmp_DIB_header->Padding_Bytes; j++)
            {
                fputc (0, fp);
            }
        }

        fwrite (&(pixel_data + i)->Blue, sizeof(__int8), 1, fp);   
        fwrite (&(pixel_data + i)->Green, sizeof(__int8), 1, fp);   
        fwrite (&(pixel_data + i)->Red, sizeof(__int8), 1, fp);   
    }   

    for (int j = 0; j < bmp_DIB_header->Padding_Bytes; j++)
    {
        fputc (0, fp);
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

__int8 turn_left (
    BMP_header *bmp_header, 
    BMP_BITMAPINFOHEADER *bmp_DIB_header,
    BMP_Pixel *pixel_data )
{

    BMP_Pixel *turn_left = calloc (bmp_DIB_header->Num_of_Pixels * BYTES_IN_PIXEL, sizeof(__int8));

    __int32 j = 0;
    __int8 abs = 1;

    __int32 index = 0;

    if (bmp_DIB_header->Is_Bottom_Up)
    {
        abs = 1;
    }    
    else
    {
        abs = -1;
    }
    
    
    for (__int32 col = 0; col < bmp_DIB_header->BMP_Width; col++)
    {
        if (abs > 0)
        {
            index = (bmp_DIB_header->Num_of_Pixels - bmp_DIB_header->BMP_Width) + col;
        }
        else
        {
            index = bmp_DIB_header->BMP_Width - col;
        }

        for (__int32 row = 0, end = bmp_DIB_header->BMP_Height * abs; row < end; row++)
        {
            (turn_left + j)->Blue = (pixel_data + index)->Blue;
            (turn_left + j)->Green = (pixel_data + index)->Green;
            (turn_left + j)->Red = (pixel_data + index)->Red;

            index -= bmp_DIB_header->BMP_Width * abs;

            j++;
        }
    }

    __int32 temp = bmp_DIB_header->BMP_Width * abs;
    bmp_DIB_header->BMP_Width = bmp_DIB_header->BMP_Height * abs;
    bmp_DIB_header->BMP_Height = temp;

    bmp_DIB_header->Padding_Bytes = ((bmp_DIB_header->BMP_Width * BYTES_IN_PIXEL) % 4);

    if (bmp_DIB_header->Padding_Bytes > 0)
    {
        bmp_DIB_header->Padding_Bytes = 4 - bmp_DIB_header->Padding_Bytes;     
    }


    void *temp_point = memcpy(pixel_data, turn_left, bmp_DIB_header->Num_of_Pixels * BYTES_IN_PIXEL);

    if (temp_point == NULL)
    {
        printf ("Error copying data!\n");

        free(turn_left);
        turn_left = NULL;

        return EXIT_FAILURE;
    }

    free(turn_left);
    turn_left = NULL;

    return EXIT_SUCCESS;
}