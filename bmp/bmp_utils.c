#include"bmp.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void formatBytes(unsigned int bytes, char *output);

BMPImage* openImage(FILE *fd) {
    BMPImage *image = malloc(sizeof(BMPImage));

    // Read File Header
    BMPFileHeader *fileHeader = &image->fileHeader;

    fread(&fileHeader->signature, sizeof(fileHeader->signature), 1, fd);
    if (BM != fileHeader->signature) {
        free(image);
        return NULL;
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
    fread(&infoHeader->colors, sizeof(infoHeader->colors), 1, fd);
    fread(&infoHeader->importantColors, sizeof(infoHeader->importantColors), 1, fd);

    // TODO: read info header, colors list and data

    return image;
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
    printf("\nColors: %u \t\t\tsize: %lu bytes", ih->colors, sizeof(ih->colors));
    printf("\nImportant colors: %u \t\tsize: %lu bytes", ih->importantColors, sizeof(ih->importantColors));
}

void printBMPImageInfo(BMPImage *img){
    printBMPFileHeader(&img->fileHeader);
    printBMPInfoHeader(&img->infoHeader);
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
