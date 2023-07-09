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
    if (argc <= 1)
    {
        printf ("Usage: <program> <edit_mode>\n");
        return EXIT_FAILURE;   
    }

    char edit_mode[MAX_CHARS];

    strcpy_s (edit_mode, MAX_CHARS, argv[1]);


    return EXIT_SUCCESS;   
}