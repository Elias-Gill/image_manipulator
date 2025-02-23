#include"filters.h"
#include<stdlib.h>

// Find the negative of an image
void negative(BMPImage *image) {
    for (unsigned int i = 0; i < image->infoHeader.imageSize; i++) {
        // Invert the pixel value (bitwise NOT)
        image->content[i] = ~ image->content[i];
    }
}

/*When reading 2^24 colors, every 3-byte triplet in the bitmap array represents the relative intensities of blue,
  green, and red, respectively, for a pixel.*/
void grayScale(BMPImage *image) {
    for (unsigned int i = 0; i < image->infoHeader.imageSize; i++) {
        unsigned char blue = image->content[i];
        unsigned char green = image->content[i+1];
        unsigned char red = image->content[i+2];

        unsigned char gray = (unsigned char)(0.299 * red + 0.587 * green + 0.114 * blue);
        image->content[i] = gray;
        image->content[i+1] = gray;
        image->content[i+2] = gray;

        i = i+3; // saltar el alpha channel
   }
}
