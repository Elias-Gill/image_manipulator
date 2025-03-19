#include"../bmp/bmp.h"

#ifndef FILTERS_H
#define FILTERS_H

// Pixel per pixel filters
void negative(BMPImage *image);
void grayScale(BMPImage *image);
void sepia(BMPImage *image);
void warmer(BMPImage *image);
void superSaturation(BMPImage *image);
void greener(BMPImage *image);

// Mask filters
void borders(BMPImage *image);
void focus(BMPImage *image);

#endif
