#include"filters.h"
#include<stdlib.h>

// Find the negative of an image
void negative(BMPImage *image) {
    for (unsigned int i = 0; i < image->infoHeader.imageSize; i++) {
        // Invert the pixel value (bitwise NOT)
        image->content[i] = ~ image->content[i];
    }
}

/* TODO: manejar diferente diferentes bitPerPixel images */
/* When reading 2^24 colors, every 3-byte triplet in the bitmap array represents the relative intensities of blue,
  green, and red, respectively, for a pixel.*/
void grayScale(BMPImage *image) {
    for (unsigned int i = 0; i < image->infoHeader.imageSize; i++) {
        unsigned char blue = image->content[i];
        unsigned char green = image->content[i+1];
        unsigned char red = image->content[i+2];

        unsigned int auxGray = (unsigned int)(0.299 * red + 0.587 * green + 0.114 * blue);
        unsigned char gray = 0;
        if (auxGray > 255) {
            gray = 255;
        } else {
            gray = auxGray;
        }

        image->content[i] = gray;
        image->content[i+1] = gray;
        image->content[i+2] = gray;

        i += 2;
    } 
}


/* TODO: manejar diferente diferentes bitPerPixel images */
void sepia(BMPImage *image) {
    for (unsigned int i = 0; i < image->infoHeader.imageSize; i++) {
        // Get the colors but cast them as a float to enhance calculations precision
        float blue = (float) image->content[i];
        float green = (float) image->content[i + 1];
        float red = (float) image->content[i + 2];

        // Calculate the new colors and cast them to int values
        int sepiaRed = (int) ((red * 0.393) + (green * 0.769) + (blue * 0.189));
        int sepiaGreen = (int) ((red * 0.349) + (green * 0.686) + (blue * 0.168));
        int sepiaBlue = (int) ((red * 0.272) + (green * 0.534) + (blue * 0.131));

        // Clamping to 255 to avoid overflow
        image->content[i]     = (sepiaBlue > 255) ? (unsigned char)255 : (unsigned char)sepiaBlue;
        image->content[i + 1] = (sepiaGreen > 255) ? (unsigned char)255 : (unsigned char)sepiaGreen;
        image->content[i + 2] = (sepiaRed > 255) ? (unsigned char)255 : (unsigned char)sepiaRed;

        i += 2;
   }
}

/* TODO: manejar diferente diferentes bitPerPixel images */
// Increases the red and green colors and make the blue ones less intense
void warmer(BMPImage *image) {
    for (unsigned int i = 0; i < image->infoHeader.imageSize; i++) {
        unsigned char blue = image->content[i];
        unsigned char green = image->content[i + 1];
        unsigned char red = image->content[i + 2];

        int tr = (int)(red * 1.6);
        int tb = (int)(blue * 0.9);
        int tg = (int)(green * 1.1);

        // Clamping to 255 to avoid overflow
        image->content[i]     = (tb > 255) ? 255 : (unsigned char)tb;
        image->content[i + 1] = (tg > 255) ? 255 : (unsigned char)tg;
        image->content[i + 2] = (tr > 255) ? 255 : (unsigned char)tr;

        i += 2;
    }
}

// Applyes a gray scale filter and if gray > 127 then gray = 255 else gray = 0
void superSaturation(BMPImage *image){
    for (unsigned int i = 0; i < image->infoHeader.imageSize; i++) {
        unsigned char blue = image->content[i];
        unsigned char green = image->content[i+1];
        unsigned char red = image->content[i+2];

        unsigned int auxGray = (unsigned int)(0.299 * red + 0.587 * green + 0.114 * blue);
        unsigned char gray = 0;
        if (auxGray > 127) {
            gray = 255;
        } else {
            gray = 0;
        }

        image->content[i] = gray;
        image->content[i+1] = gray;
        image->content[i+2] = gray;

        i += 2;
    } 
}
