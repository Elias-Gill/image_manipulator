#include<bmp.h>

Pixel emptyPixel = {.red = 0, .green = 0, .blue = 0};

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
    printf("\nSize: %u \t\t\tsize: %lu bytes", ih->headerSize, sizeof(ih->headerSize));
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

// When a requested pixel is out of bounds, then a empty full black pixel is given as a default value
Pixel getPixel(BMPImage *image, int x, int y, Pixel fallback) {
    if (x < 0 || y < 0 || x >= (int)image->infoHeader.width || y >= (int)image->infoHeader.height) {
        return fallback;
    }

    unsigned int coordinate = (image->infoHeader.width * y) + x;
    if (coordinate >= image->pixelsCount) {
        return fallback;
    }

    return image->pixels[coordinate];
}
