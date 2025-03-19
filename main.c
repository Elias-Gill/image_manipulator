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
#include<string.h>
#include<stdlib.h>

int main(int argc, char **argv) {
    char defaultOutput[60] = "/tmp/result.bmp";
    if (argc < 3) {
        printf("Usage: \n\tbmp_program filter inputFile [outputFile]\n");
        printf("Available filters:%s%s%s%s%s%s", "\n- sepia", "\n- grayScale", "\n- negative", "\n- warmer",
                "\n- superSaturation", "\n- greener");

        // If help flag was submited
        if (argc == 2 && strcmp(argv[1], "--help") == 0) {
            return 0;
        }

        return -1;
    }

    char *filter = argv[1];
    char *inputFile = argv[2];
    char *outputFile;
    if (argc > 3) {
        outputFile = argv[3];
    } else {
        outputFile = defaultOutput;
    }

    // Select operation (avoid loading the file if the parameters are malformed)
    void (*operation)(BMPImage *) = NULL;
    if(strcmp(filter, "grayScale") == 0) {
        operation = &grayScale;
    }
    if(strcmp(filter, "negative") == 0) {
        operation = &negative;
    }
    if(strcmp(filter, "sepia") == 0) {
        operation = &sepia;
    }
    if(strcmp(filter, "warmer") == 0) {
        operation = &warmer;
    }
    if(strcmp(filter, "greener") == 0) {
        operation = &greener;
    }
    if(strcmp(filter, "superSaturation") == 0) {
        operation = &superSaturation;
    }
    if(strcmp(filter, "borders") == 0) {
        operation = &borders;
    }
    if(strcmp(filter, "focus") == 0) {
        operation = &focus;
    }
    if(operation == NULL) {
        fprintf(stderr, "Invalid filter selection: %s", filter);
        return -1;
    }

    // Load file data
    int err; 
    printf("\nExtracting file data\nFile: %s", inputFile);

    BMPImage *image = malloc(sizeof(BMPImage));
    err = loadImage(inputFile, image);
    if (err != 0) {
        return err;
    }
    printBMPImageInfo(image);

    // Apply filter 
    operation(image);

    // Store result file
    printf("Saving image to: %s", outputFile);
    err = saveImage(outputFile, image);
    if (err != 0) {
        return err;
    }
    printf("File saved succesfully!");

    // free resources
    freeImage(image);

    return 0;
}
