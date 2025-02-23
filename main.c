/*
The structure of a .bmp is like:

    File Header 0x00 - 0x0D:
        contains information about the type, size, and layout of a device-independent bitmap file

    InfoHeader 0x0E - 0x35: 
        Specifies the dimensions, compression type, and color format for the bitmap

    ColorTable 0x36 - variable: 
        Contains as many elements as there are colors in the bitmap, but is not present for bitmaps with 24 color bits
        because each pixel is represented by 24-bit red-green-blue (RGB) values in the actual bitmap data area an array of
        bytes that defines the bitmap bits. 

    Pixel Data variable:
        These are the actual image data, represented by consecutive rows, or "scan lines," of the bitmap. Each scan line
        consists of consecutive bytes representing the pixels in the scan line, in left-to-right order. The system maps
        pixels beginning with the bottom scan line of the rectangular region and ending with the top scan line.
*/

#include"bmp/bmp.h"
#include"filters/filters.h"
#include<stdio.h>
#include<stdlib.h>

// TODO: recibir como parametro
char inputFile[65] = "/home/elias/Proyectos/c_projects/image_manipulator/dead_star.bmp";
char outputFile[65] = "/tmp/result.bmp";

int main() {
    int err; 
    printf("\nExtracting file data\nFile: %s", inputFile);

    BMPImage *image = malloc(sizeof(BMPImage));
    err = loadImage(inputFile, image);
    if (err != 0) {
        return err;
    }

    printBMPImageInfo(image);

    grayScale(image);

    // Store result file
    err = saveImage(outputFile, image);
    if (err != 0) {
        return err;
    }

    // free resources
    freeImage(image);

    return 0;
}
