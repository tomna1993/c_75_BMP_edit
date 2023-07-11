# c_75_BMP_edit

## DESCRIPTION

Open a BMP file then edit and save a new file. Use CL arguments to ask the user what he wants to edit on the image, f.e. flip horizontally, flip vertically, rotate left, rotate right.

[Informations about BMP file structure](https://en.wikipedia.org/wiki/BMP_file_format)

### IMPLEMENTATION DETAILS

- read file name (or path) from command line argument

- make sure the file is a BMP image

- create a structure to save all the BMP header and DIB infromation

- read in all the BMP header information and print them on screen

- read pixel data into an allocated memory field

- get edit mode information and create turn, flip vertically and flip horizontally functions to edit the pixel data

- save the edited image as a new image
