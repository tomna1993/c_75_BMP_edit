#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CHARS 20

#define EMODE_FLIP_H "flip_horizontally"
#define EMODE_FLIP_V "flip_vertically"
#define EMODE_TURN_R "turn_right"
#define EMODE_TURN_L "turn_left"

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

    return EXIT_SUCCESS;   
}