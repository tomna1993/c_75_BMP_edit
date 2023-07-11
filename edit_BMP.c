#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CHARS 50

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

    // Pixel data reading/writing method
    // true: read/write pixel data starting from bottom-left pixel from left to right
    //       each row from bottom-up
    // false: read/write pixel data starting from top-left pixel from left to right
    //       each row from top-down
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
    BMP_BITMAPINFOHEADER *bmp_DIB_header, 
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

__int8 flip_h (
    BMP_header *bmp_header, 
    BMP_BITMAPINFOHEADER *bmp_DIB_header,
    BMP_Pixel *pixel_data );

__int8 flip_v (
    BMP_header *bmp_header, 
    BMP_BITMAPINFOHEADER *bmp_DIB_header,
    BMP_Pixel *pixel_data );



int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf ("Usage: <program> <BMP file>\n");
        return EXIT_FAILURE;   
    }

    char BMP_in[MAX_CHARS] = { '\0' };
    char BMP_out[MAX_CHARS] = { '\0' };

    strcpy_s (BMP_in, MAX_CHARS, argv[1]);
    strcpy_s (BMP_out, MAX_CHARS, "Output.bmp");
    
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
 
    printf ("\n");
    printf ("Choose one option:\n");
    printf ("1: Turn left\n");
    printf ("2: Flip vertically\n");
    printf ("3: Flip horizontally\n");
    printf ("4: Save and Quit\n");
    printf ("\n");

    __int32 edit_mode = 0;

    do
    {
        printf ("Edit: ");
        scanf_s ("%i", &edit_mode);

        switch (edit_mode)
        {
            case 1:
                printf ("Turn left\n");
                is_Error = turn_left (&bmp_header, &bmp_DIB_header, pixel_data);                
                break;

            case 2:
                printf ("Flip vertically\n");
                is_Error = flip_v (&bmp_header, &bmp_DIB_header, pixel_data);
                break;
            
            case 3:
                printf ("Flip horizontally\n");
                is_Error = flip_h (&bmp_header, &bmp_DIB_header, pixel_data);
                break;

            case 4:
                printf ("Exit editing, file is saved\n");
                break;

            default:
                printf ("Wrong command!\n");
                break;
        }

        if (is_Error == EXIT_FAILURE)
        {
            printf ("Failed to edit the file!\n");

            free(pixel_data);
            pixel_data = NULL;

            return EXIT_FAILURE;   
        }

        is_Error = create_new_image (BMP_out, &bmp_header, &bmp_DIB_header, pixel_data);

        if (is_Error == EXIT_FAILURE)
        {
            printf ("Failed to create/modify the file!\n");

            free(pixel_data);
            pixel_data = NULL;

            return EXIT_FAILURE;   
        }
    }   
    while (edit_mode != 4);

    free(pixel_data);
    pixel_data = NULL;

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
    
    __int8 abs = 1;
    // If image read/write method is bottom-up then image height will be positive
    // Else it's negative
    abs = bmp_DIB_header->Is_Bottom_Up ? 1 : -1;

    __int32 start_index = 0;

    if (abs == 1)
    {
        start_index = bmp_DIB_header->Num_of_Pixels - bmp_DIB_header->BMP_Width;
    }
    else
    {
        start_index = bmp_DIB_header->BMP_Width;
    }

    __int32 index = 0;
    __int32 j = 0;
    
    for (__int32 col = 0; col < bmp_DIB_header->BMP_Width; col++)
    {
        index = start_index + (col * abs);

        for (__int32 row = 0, end = bmp_DIB_header->BMP_Height * abs; row < end; row++)
        {
            *(turn_left + j++) = *(pixel_data + index);

            index -= bmp_DIB_header->BMP_Width * abs;
        }
    }

    // Swap picture width with height
    __int32 temp = bmp_DIB_header->BMP_Width * abs;
    bmp_DIB_header->BMP_Width = bmp_DIB_header->BMP_Height * abs;
    bmp_DIB_header->BMP_Height = temp;

    // Recalculate the padding bytes
    bmp_DIB_header->Padding_Bytes = ((bmp_DIB_header->BMP_Width * BYTES_IN_PIXEL) % 4);

    if (bmp_DIB_header->Padding_Bytes > 0)
    {
        bmp_DIB_header->Padding_Bytes = 4 - bmp_DIB_header->Padding_Bytes;     
    }

    // Copy the modified pixel data back into the original pixel data memory field
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

__int8 flip_h (
    BMP_header *bmp_header, 
    BMP_BITMAPINFOHEADER *bmp_DIB_header,
    BMP_Pixel *pixel_data )
{

    BMP_Pixel *flip_temp = calloc (bmp_DIB_header->Num_of_Pixels * BYTES_IN_PIXEL, sizeof(__int8));

    __int32 index = 0;
    __int32 j = 0;
    __int32 row = 1;

    while (j < bmp_DIB_header->Num_of_Pixels)
    {
        // Reach the end of row
        if (j % bmp_DIB_header->BMP_Width == 0)
        {
            index = bmp_DIB_header->Num_of_Pixels - (bmp_DIB_header->BMP_Width * row++);
        }

        *(flip_temp + index++) = *(pixel_data + j++);
    }

    // Copy the modified pixel data back into the original pixel data memory field
    void *temp_point = memcpy(pixel_data, flip_temp, bmp_DIB_header->Num_of_Pixels * BYTES_IN_PIXEL);

    if (temp_point == NULL)
    {
        printf ("Error copying data!\n");

        free(flip_temp);
        flip_temp = NULL;

        return EXIT_FAILURE;
    }

    free(flip_temp);
    flip_temp = NULL;

    return EXIT_SUCCESS;
}

__int8 flip_v (
    BMP_header *bmp_header, 
    BMP_BITMAPINFOHEADER *bmp_DIB_header,
    BMP_Pixel *pixel_data )
{

    BMP_Pixel *flip_temp = calloc (bmp_DIB_header->Num_of_Pixels * BYTES_IN_PIXEL, sizeof(__int8));

     __int8 abs = 1;
    // If image read/write method is bottom-up then image height will be positive
    // Else it's negative
    abs = bmp_DIB_header->Is_Bottom_Up ? 1 : -1;

    __int32 count_temp = 0;
    __int32 count_data = 0;

    for (__int32 col = 0; col < bmp_DIB_header->BMP_Width; col++)
    {
        count_temp = bmp_DIB_header->BMP_Width - col;
        count_data = col;
        
        for (__int32 row = 0, end = bmp_DIB_header->BMP_Height * abs; row < end; row++)
        {
            *(flip_temp + count_temp) = *(pixel_data + count_data);

            count_temp += bmp_DIB_header->BMP_Width;
            count_data += bmp_DIB_header->BMP_Width;
        }
    }

    // Copy the modified pixel data back into the original pixel data memory field
    void *temp_point = memcpy(pixel_data, flip_temp, bmp_DIB_header->Num_of_Pixels * BYTES_IN_PIXEL);

    if (temp_point == NULL)
    {
        printf ("Error copying data!\n");

        free(flip_temp);
        flip_temp = NULL;

        return EXIT_FAILURE;
    }

    free(flip_temp);
    flip_temp = NULL;

    return EXIT_SUCCESS;
}