#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CHARS 20

#define EMODE_FLIP_H "flip_horizontally"
#define EMODE_FLIP_V "flip_vertically"
#define EMODE_TURN_R "turn_right"
#define EMODE_TURN_L "turn_left"

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