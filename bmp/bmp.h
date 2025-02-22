#include<stdio.h>

#ifndef BMP_HEADER_H
#define BMP_HEADER_H

// Value in unsigned short representation of the 'BM' signature
#define BM 19778

/* contains information about the type, size, and layout of a
   device-independent bitmap file */
typedef struct {
    unsigned short signature;   // 2 bytes. Always "BM" expected.
    unsigned int fileSize;      // 4 bytes. Contains the filesize in BYTES.
    unsigned int reserved;      // value 0, this is reserved and not used
    unsigned int dataOffset;    // 4 bytes. Offset from beginning of file 
                                // to the beginning of the bitmap data in bytes.
} BMPFileHeader;

/* Specifies the dimensions, compression type, and color format
   for the bitmap */
typedef struct {
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitsPerPixel;        // 1: monochrome, 4: 16 colors, 8: 256 colors and so on.
    unsigned int compression;           // 0: no compression (expected)
    unsigned int imageSize;             // 0 if no compression
    unsigned int horizontalResolution;  // 0 if no compression
    unsigned int verticalResolution;    // 0 if no compression
    unsigned int colors;                // 256 if using 8bit encoding
    unsigned int importantColors;       // 0 = all
} BMPInfoHeader;

// This header is only present if bitsPerPixel is less than 8.
// Colors should be ordered by importance.
typedef struct {
    char red;       // red intensity
    char green;     // green intensity
    char blue;      // blue intensity
    char unnused;   // value 0, this is reserved and not used
} ColorInfo;

typedef struct {
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    ColorInfo     *colorTable;  // A list of data for the colors
    char          *content;     // The Image.size pixels of the image
} BMPImage;

void printBMPFileHeader(BMPFileHeader *fh);

BMPImage* openImage(FILE *fd);

#endif // BMP_HEADER_H
