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
    fread(&fileHeader->dataOffset, sizeof(fileHeader->dataOffset), 1, fd);

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

    printf("\n\033[35mHeader field \t\t\tField size\033[0m");
    printf("\nSignature: %s \t\t\tsize: %lu bytes", signature, sizeof(fh->signature));
    printf("\nFile size: %s \t\tsize: %lu bytes", fileSize, sizeof(fh->fileSize));
    printf("\nData offset: %i \t\t\tsize: %lu bytes", fh->dataOffset, sizeof(fh->dataOffset));
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
