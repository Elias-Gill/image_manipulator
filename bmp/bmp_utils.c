#include"bmp.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int loadImage(char *inputFile, BMPImage *image) {
    if (image == NULL) {
        printf("Bad use of loadImage(). Passing a NULL pointer. \nYou have to preallocate a BMPImage struct.");
        return -1;
    }

    FILE *fd = fopen(inputFile, "rb");
    if (fd == NULL ) {
        printf("Cannot open input file");
        return -1;
    }

    // Read File Header
    BMPFileHeader *fileHeader = &image->fileHeader;

    fread(&fileHeader->signature, sizeof(fileHeader->signature), 1, fd);
    if (BM != fileHeader->signature) {
        printf("Bad .bmp file signature.");
        return -1;
    }
    fread(&fileHeader->fileSize, sizeof(fileHeader->fileSize), 1, fd);
    fread(&fileHeader->reserved, sizeof(fileHeader->reserved), 1, fd);
    fread(&fileHeader->dataOffset, sizeof(fileHeader->dataOffset), 1, fd);

    // Read Info Header
    BMPInfoHeader *infoHeader = &image->infoHeader;

    fread(&infoHeader->size, sizeof(infoHeader->size), 1, fd);
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
    char *bitmap = malloc(sizeof(char) * infoHeader->imageSize);
    for (unsigned int i = 0; i < infoHeader->imageSize; i++){
        fread(&bitmap[i], sizeof(char), 1, fd);
    }
    image->content = bitmap;

    // free resources
    fclose(fd);

    return 0;
}

// Write a BMP image into a file
int saveImage(char *outputFile, BMPImage *image) {
    printf("\nSaving File");
    FILE *fd = fopen(outputFile, "wb");
    if (fd == NULL) {
        perror("Cannot open the result file");
        return -1;
    }

    // Writing BMPFileHeader fields separately
    fwrite(&image->fileHeader.signature, sizeof(image->fileHeader.signature), 1, fd);
    fwrite(&image->fileHeader.fileSize, sizeof(image->fileHeader.fileSize), 1, fd);
    fwrite(&image->fileHeader.reserved, sizeof(image->fileHeader.reserved), 1, fd);
    fwrite(&image->fileHeader.dataOffset, sizeof(image->fileHeader.dataOffset), 1, fd);

    // Writing BMPInfoHeader fields separately
    fwrite(&image->infoHeader.size, sizeof(image->infoHeader.size), 1, fd);
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

    // Write the content
    if (image->content) {
        fwrite(image->content, image->infoHeader.imageSize, 1, fd);
    }

    fclose(fd);
    printf("\nFile saved succesfully: %s\n", outputFile);

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
    if (image->content != NULL) {
        free(image->content);  // Free the image pixel data
    }

    // Free the BMP headers (fileHeader and infoHeader) is not necesary 
    // because they are not dynamically allocated
    free(image);
}

// Convert bytes to KB, MB or GB
void formatBytes(unsigned int bytes, char *output) {
    const char *units[] = {"Bytes", "KB", "MB", "GB"};
    int unitIndex = 0;
    double convertedSize = (double)bytes;

    while (convertedSize >= 1024 && unitIndex < 3) {
        convertedSize /= 1024;
        unitIndex++;
    }

    snprintf(output, 50, "%.2f %s", convertedSize, units[unitIndex]);
}

void printBMPFileHeader(BMPFileHeader *fh){
    // Extract bytes from signature (little-endian)
    char signature[3];                                  // Buffer to store the signature as a string
    signature[0] = (char)(fh->signature & 0xFF);        // Lest significative byte ('B')
    signature[1] = (char)((fh->signature >> 8) & 0xFF); // Most significative byte ('M')
    signature[2] = '\0';                                // Null terminator
                                                    
    char fileSize[32];
    formatBytes(fh->fileSize, fileSize);

    printf("\n\n\033[32mFile header\033[0m");
    printf("\n\033[35mHeader field \t\t\tField size\033[0m");
    printf("\nSignature: %s \t\t\tsize: %lu bytes", signature, sizeof(fh->signature));
    printf("\nFile size: %s \t\tsize: %lu bytes", fileSize, sizeof(fh->fileSize));
    printf("\nReserved \t\t\tsize: %lu bytes", sizeof(fh->reserved));
    printf("\nData offset: %i \t\tsize: %lu bytes", fh->dataOffset, sizeof(fh->dataOffset));
}

void printBMPInfoHeader(BMPInfoHeader *ih) {
    printf("\n\n\033[32mImage info header\033[0m");
    printf("\n\033[35mHeader field \t\t\tField size\033[0m");
    printf("\nSize: %u \t\t\tsize: %lu bytes", ih->size, sizeof(ih->size));
    printf("\nWidth: %u \t\t\tsize: %lu bytes", ih->width, sizeof(ih->width));
    printf("\nHeight: %u \t\t\tsize: %lu bytes", ih->height, sizeof(ih->height));
    printf("\nPlanes: %u \t\t\tsize: %lu bytes", ih->planes, sizeof(ih->planes));
    printf("\nBits per pixel: %u \t\tsize: %lu bytes", ih->bitsPerPixel, sizeof(ih->bitsPerPixel));
    printf("\nCompression: %u \t\t\tsize: %lu bytes", ih->compression, sizeof(ih->compression));
    printf("\nImage size: %i bytes \tsize: %lu bytes", ih->imageSize, sizeof(ih->imageSize));
    printf("\nX-resolution: %u \t\tsize: %lu bytes", ih->horizontalResolution, sizeof(ih->horizontalResolution));
    printf("\nY-resolution: %u \t\tsize: %lu bytes", ih->verticalResolution, sizeof(ih->verticalResolution));
    printf("\nUsed colors: %u \t\t\tsize: %lu bytes", ih->usedColors, sizeof(ih->usedColors));
    printf("\nImportant colors: %u \t\tsize: %lu bytes", ih->importantColors, sizeof(ih->importantColors));
}

void printColorTable(ColorInfo **colorTable, unsigned short size) {
    printf("\n\n\033[32mColors table\033[0m");
    if (size == 0 || colorTable == NULL) {
        printf("\nEmpty color table\n");
        return;
    }

    for (unsigned short i = 0; i < size; i++) {
        if (colorTable[i] == NULL) {
            printf("\nError: colorTable[%d] is NULL\n", i);
            continue;  // Skip this entry
        }
        ColorInfo *entry = colorTable[i];

        printf("\nRed: %i", entry->red);
        printf("\nGreen: %i", entry->green);
        printf("\nBlue: %i", entry->blue);
        printf("\nReserved %lu bytes", sizeof(entry->unnused));
        printf("\n-------------");
    }
    printf("\n");
}

void printBMPImageInfo(BMPImage *img){
    printBMPFileHeader(&img->fileHeader);
    printBMPInfoHeader(&img->infoHeader);
    printColorTable(img->colorTable, img->infoHeader.bitsPerPixel);
}
