#include"bmp.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void loadMonochromeContent(FILE *fd, BMPImage *image);
void load4BitContent(FILE *fd, BMPImage *image);
void load8BitContent(FILE *fd, BMPImage *image);
void load16BitContent(FILE *fd, BMPImage *image);
void load24BitContent(FILE *fd, BMPImage *image);
void load32BitContent(FILE *fd, BMPImage *image);

void writeMonochromeContent(FILE *fd, BMPImage *image);
void write4BitContent(FILE *fd, BMPImage *image);
void write8BitContent(FILE *fd, BMPImage *image);
void write16BitContent(FILE *fd, BMPImage *image);
void write24BitContent(FILE *fd, BMPImage *image);
void write32BitContent(FILE *fd, BMPImage *image);

int loadImage(char *inputFile, BMPImage *image) {
    if (image == NULL) {
        fprintf(stderr, "Bad use of loadImage(). Passing a NULL pointer. \nYou have to preallocate a BMPImage struct.");
        return -1;
    }

    FILE *fd = fopen(inputFile, "rb");
    if (fd == NULL ) {
        fprintf(stderr, "Cannot open input file: %s", inputFile);
        return -1;
    }

    // Read File Header
    BMPFileHeader *fileHeader = &image->fileHeader;

    fread(&fileHeader->signature, sizeof(fileHeader->signature), 1, fd);
    if (BM != fileHeader->signature) {
        fprintf(stderr, "Bad .bmp file signature.");
        return -1;
    }
    fread(&fileHeader->fileSize, sizeof(fileHeader->fileSize), 1, fd);
    fread(&fileHeader->reserved, sizeof(fileHeader->reserved), 1, fd);
    fread(&fileHeader->dataOffset, sizeof(fileHeader->dataOffset), 1, fd);

    // Read Info Header
    BMPInfoHeader *infoHeader = &image->infoHeader;

    fread(&infoHeader->headerSize, sizeof(infoHeader->headerSize), 1, fd);
    fread(&infoHeader->width, sizeof(infoHeader->width), 1, fd);
    fread(&infoHeader->height, sizeof(infoHeader->height), 1, fd);
    fread(&infoHeader->planes, sizeof(infoHeader->planes), 1, fd);
    fread(&infoHeader->bitsPerPixel, sizeof(infoHeader->bitsPerPixel), 1, fd);
    fread(&infoHeader->compression, sizeof(infoHeader->compression), 1, fd);
    fread(&infoHeader->imageSize, sizeof(infoHeader->imageSize), 1, fd);
    fread(&infoHeader->horizontalResolution, sizeof(infoHeader->horizontalResolution), 1, fd);
    fread(&infoHeader->verticalResolution, sizeof(infoHeader->verticalResolution), 1, fd);
    fread(&infoHeader->usedColors, sizeof(infoHeader->usedColors), 1, fd);
    fread(&infoHeader->importantColors, sizeof(infoHeader->importantColors), 1, fd);

    // Read colors table
    if (infoHeader->bitsPerPixel < 8) {
        unsigned int n = 1 << infoHeader->bitsPerPixel; // exponentiation
        image->colorTable = malloc(sizeof(ColorInfo) * n);

        for (unsigned int i = 0; i < n; i++) {
            ColorInfo *aux = malloc(sizeof(ColorInfo));
            fread(&aux->red, sizeof(aux->red), 1, fd);
            fread(&aux->blue, sizeof(aux->green), 1, fd);
            fread(&aux->green, sizeof(aux->blue), 1, fd);
            fread(&aux->unnused, sizeof(aux->unnused), 1, fd);
            image->colorTable[i] = aux;
        }
    } else {
        image->colorTable = NULL;
    }

    // Read bitmap content
    unsigned int n = infoHeader->width * infoHeader->height;
    image->pixels = malloc(sizeof(Pixel) * n);
    image->pixelsCount = n;

    switch(infoHeader->bitsPerPixel) {
        case 1:
            /*err = loadMonochromeContent(fd, image, content)*/
            break;
        case 4:
            break;
        case 8:
            /*load8BitContent(fd, image);*/
            break;
        case 16:
            load16BitContent(fd, image);
            break;
        case 24:
            load24BitContent(fd, image);
            break;
        case 32:
            load32BitContent(fd, image);
            break;
    }

    // free resources
    fclose(fd);

    return 0;
}

// Write a BMP image into a file
int saveImage(char *outputFile, BMPImage *image) {
    FILE *fd = fopen(outputFile, "wb");
    if (fd == NULL) {
        fprintf(stderr, "Cannot write over the output file %s\n", outputFile);
        return -1;
    }

    // Writing BMPFileHeader fields separately
    fwrite(&image->fileHeader.signature, sizeof(image->fileHeader.signature), 1, fd);
    fwrite(&image->fileHeader.fileSize, sizeof(image->fileHeader.fileSize), 1, fd);
    fwrite(&image->fileHeader.reserved, sizeof(image->fileHeader.reserved), 1, fd);
    fwrite(&image->fileHeader.dataOffset, sizeof(image->fileHeader.dataOffset), 1, fd);

    // Writing BMPInfoHeader fields separately
    fwrite(&image->infoHeader.headerSize, sizeof(image->infoHeader.headerSize), 1, fd);
    fwrite(&image->infoHeader.width, sizeof(image->infoHeader.width), 1, fd);
    fwrite(&image->infoHeader.height, sizeof(image->infoHeader.height), 1, fd);
    fwrite(&image->infoHeader.planes, sizeof(image->infoHeader.planes), 1, fd);
    fwrite(&image->infoHeader.bitsPerPixel, sizeof(image->infoHeader.bitsPerPixel), 1, fd);
    fwrite(&image->infoHeader.compression, sizeof(image->infoHeader.compression), 1, fd);
    fwrite(&image->infoHeader.imageSize, sizeof(image->infoHeader.imageSize), 1, fd);
    fwrite(&image->infoHeader.horizontalResolution, sizeof(image->infoHeader.horizontalResolution), 1, fd);
    fwrite(&image->infoHeader.verticalResolution, sizeof(image->infoHeader.verticalResolution), 1, fd);
    fwrite(&image->infoHeader.usedColors, sizeof(image->infoHeader.usedColors), 1, fd);
    fwrite(&image->infoHeader.importantColors, sizeof(image->infoHeader.importantColors), 1, fd);

    // Write the colors table
    if (image->infoHeader.bitsPerPixel <= 8 && image->colorTable != NULL) {
        unsigned int numColors = 1 << image->infoHeader.bitsPerPixel; // 2^bitsPerPixel
        for (unsigned int i = 0; i < numColors; i++) {
            fwrite(image->colorTable[i], sizeof(ColorInfo), 1, fd);
        }
    }

    // TODO: Write the content
    if (image->pixels != NULL) {
        switch(image->infoHeader.bitsPerPixel) {
            case 1:
                /*err = writeMonochromeContent(fd, image, content)*/
                break;
            case 4:
                break;
            case 8:
                /*write8BitContent(fd, image);*/
                break;
            case 16:
                write16BitContent(fd, image);
                break;
            case 24:
                write24BitContent(fd, image);
                break;
            case 32:
                write32BitContent(fd, image);
                break;
        }
    }

    fclose(fd);

    return 0;
}

void freeImage(BMPImage *image) {
    if (image == NULL) {
        return;
    }

    // Free the colorTable
    if (image->colorTable != NULL) {
        unsigned int numColors = 1 << image->infoHeader.bitsPerPixel; // 2^bitsPerPixel
        for (unsigned short i = 0; i < numColors; i++) {
            free(image->colorTable[i]);
        }
        free(image->colorTable);  // Free the colorTable pointer itself
    }

    // Free the image content
    if (image->pixels != NULL) {
        free(image->pixels);
    }

    // Free the BMP headers (fileHeader and infoHeader) is not necesary 
    // because they are not dynamically allocated
    free(image);
}

// FIX: not implemented yet
void loadMonochromeContent(FILE *fd, BMPImage *image){
    /*return 1;*/
}

// FIX: not implemented yet
void load4BitContent(FILE *fd, BMPImage *image){
    /*return 1;*/
}

// FIX: not implemented yet
void load8BitContent(FILE *fd, BMPImage *image) {
    /*return 1;*/
}

/*The value for blue is in the least significant 5 bits, 
  followed by 5 bits for green and 
  5 bits for red, respectively. The most significant bit is not used.*/
void load16BitContent(FILE *fd, BMPImage *image) {
    for (unsigned int i = 0; i < image->pixelsCount; i++) {
        unsigned short colors;
        fread(&colors, sizeof(colors), 1, fd);

        // Parse colors using bitwise operations
        unsigned char red = (colors >> 10) & 0x1F;    // Extract red (bits 10-14)
        unsigned char green = (colors >> 5) & 0x1F;  // Extract green (bits 5-9)
        unsigned char blue = colors & 0x1F;          // Extract blue (bits 0-4)

        // Scale 5-bit values to 8-bit (0-255 range)
        red = (red << 3) | (red >> 2);     // Scale 5-bit to 8-bit
        green = (green << 3) | (green >> 2); // Scale 5-bit to 8-bit
        blue = (blue << 3) | (blue >> 2);   // Scale 5-bit to 8-bit

        Pixel pixel = {
            .red = red, 
            .green = green, 
            .blue = blue
        };

        image->pixels[i] = pixel;
    }
}

void load24BitContent(FILE *fd, BMPImage *image){
    for (unsigned int i = 0; i < image->pixelsCount; i++) {
        unsigned char red, green, blue;

        // NOTE: kep in this specific order
        fread(&blue, sizeof(unsigned char), 1, fd);
        fread(&green, sizeof(unsigned char), 1, fd);
        fread(&red, sizeof(unsigned char), 1, fd);

        Pixel pixel = {
            .red = red, 
            .green = green, 
            .blue = blue
        };

        image->pixels[i] = pixel;
    }
}

// NOTE: When using 32 bits per pixel, the 4 byte is the alpha channel. For this project im not counting (yet)
// the alpha channel, so I'm ignoring it's content.
void load32BitContent(FILE *fd, BMPImage *image){
    for (unsigned int i = 0; i < image->pixelsCount; i++) {
        unsigned char red, green, blue, unnused;

        // NOTE: kep in this specific order
        fread(&blue, sizeof(unsigned char), 1, fd);
        fread(&green, sizeof(unsigned char), 1, fd);
        fread(&red, sizeof(unsigned char), 1, fd);
        fread(&unnused, sizeof(unsigned char), 1, fd); // skip over the alpha channel

        Pixel pixel = {
            .red = red, 
            .green = green, 
            .blue = blue
        };

        image->pixels[i] = pixel;
    }
}

// FIX: not implemented yet
void writeMonochromeContent(FILE *fd, BMPImage *image){
    /*return 1;*/
}

// FIX: not implemented yet
void write4BitContent(FILE *fd, BMPImage *image){
    /*return 1;*/
}

// FIX: not implemented yet
void write8BitContent(FILE *fd, BMPImage *image) {
    /*return 1;*/
}

void write16BitContent(FILE *fd, BMPImage *image) {
    for (unsigned int i = 0; i < image->pixelsCount; i++) {
        Pixel pixel = image->pixels[i];

        // Scale 8-bit values to 5-bit
        unsigned short red = (pixel.red >> 3) & 0x1F;   // 5 bits for red
        unsigned short green = (pixel.green >> 3) & 0x1F; // 5 bits for green
        unsigned short blue = (pixel.blue >> 3) & 0x1F;  // 5 bits for blue

        // Pack the values into a 16-bit unsigned short
        unsigned short colors = (red << 10) | (green << 5) | blue;

        // Write the 16-bit color value to the file
        fwrite(&colors, sizeof(colors), 1, fd);
    }
}

void write24BitContent(FILE *fd, BMPImage *image) {
    for (unsigned int i = 0; i < image->pixelsCount; i++) {
        Pixel pixel = image->pixels[i];

        // Nota: keep this specific order
        fwrite(&pixel.blue, sizeof(unsigned char), 1, fd);
        fwrite(&pixel.green, sizeof(unsigned char), 1, fd);
        fwrite(&pixel.red, sizeof(unsigned char), 1, fd);
    }
}

void write32BitContent(FILE *fd, BMPImage *image) {
    unsigned char alpha = 0xFF; // default value for alpha channel (ignored)

    for (unsigned int i = 0; i < image->pixelsCount; i++) {
        Pixel pixel = image->pixels[i];

        // Nota: keep this specific order
        fwrite(&pixel.blue, sizeof(unsigned char), 1, fd);
        fwrite(&pixel.green, sizeof(unsigned char), 1, fd);
        fwrite(&pixel.red, sizeof(unsigned char), 1, fd);
        fwrite(&alpha, sizeof(unsigned char), 1, fd);
    }
}
