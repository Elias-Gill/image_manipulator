#include<stdio.h>

#ifndef BMP_HEADER_H
#define BMP_HEADER_H

// Value in hex representation of the word 'BM' for the signature
#define BM 0x4D42

/* contains information about the type, size, and layout of a
   device-independent bitmap file */
typedef struct {
    unsigned short signature;   // 2 bytes. Always "BM" expected.
    unsigned int fileSize;      // 4 bytes. Contains the filesize in BYTES.
    unsigned int reserved;      // 4 bytes. This value is supposed to be 0 and not used.
    unsigned int dataOffset;    // 4 bytes. Offset from beginning of file 
                                // to the beginning of the bitmap data in bytes.
} BMPFileHeader;

/* Specifies the dimensions, compression type, and color format
   for the bitmap */
typedef struct {
    // Image resolution and size specification
    unsigned int size;                  // size of the info header (40 bytes)
    unsigned int width;                 // image width
    unsigned int height;                // image height
    unsigned short planes;              // expected to be 1
    unsigned short bitsPerPixel;        // 1: monochrome, 4: 16 colors, 8: 256 colors and so on.

    // All of these headers are specific for compressed images. If the image has no compression then, 
    // the compression byte and the rest of fields are set to 0.
    unsigned int compression;           // 0 if no compression (expected)
    unsigned int imageSize;             // 0 if no compression
    unsigned int horizontalResolution;  // 0 if no compression
    unsigned int verticalResolution;    // 0 if no compression

    // Color table information
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

// Functions to operate over bmp files
BMPImage* openImage(FILE *fd);

// Utility functions to print the contents of the headers
void printBMPFileHeader(BMPFileHeader *fh);
void printBMPInfoHeader(BMPInfoHeader *ih);

void printBMPImageInfo(BMPImage *img);

#endif // BMP_HEADER_H
