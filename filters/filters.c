#include"filters.h"
#include<stdlib.h>

// Find the negative of an image
void negative(BMPImage *image) {
    for (unsigned int i = 0; i < image->pixelsCount; i++) {
        Pixel pixel = image->pixels[i];
        // Invert the pixel value (bitwise NOT)
        image->pixels[i].red = ~ pixel.red;
        image->pixels[i].blue = ~ pixel.blue;
        image->pixels[i].green = ~ pixel.green;
    }
}

/* TODO: manejar diferente diferentes bitPerPixel images */
/* When reading 2^24 colors, every 3-byte triplet in the bitmap array represents the relative intensities of blue,
  green, and red, respectively, for a pixel.*/
void grayScale(BMPImage *image) {
    for (unsigned int i = 0; i < image->pixelsCount; i++) {
        Pixel pixel = image->pixels[i];

        unsigned int auxGray = (unsigned int)(0.299 * pixel.red + 0.587 * pixel.green + 0.114 * pixel.blue);
        unsigned char gray = 0;
        if (auxGray > 255) {
            gray = 255;
        } else {
            gray = auxGray;
        }

        image->pixels[i].red = gray;
        image->pixels[i].green = gray;
        image->pixels[i].blue = gray;
    } 
}


/* TODO: manejar diferente diferentes bitPerPixel images */
void sepia(BMPImage *image) {
    for (unsigned int i = 0; i < image->pixelsCount; i++) {
        Pixel pixel = image->pixels[i];

        // Calculate the new colors and cast them to int values
        int sepiaRed = (int) ((pixel.red * 0.393) + (pixel.green * 0.769) + (pixel.blue * 0.189));
        int sepiaGreen = (int) ((pixel.red * 0.349) + (pixel.green * 0.686) + (pixel.blue * 0.168));
        int sepiaBlue = (int) ((pixel.red * 0.272) + (pixel.green * 0.534) + (pixel.blue * 0.131));

        // Clamping to 255 to avoid overflow
        image->pixels[i].blue = (sepiaBlue > 255) ? 255 : sepiaBlue;
        image->pixels[i].green = (sepiaGreen > 255) ? 255 : sepiaGreen;
        image->pixels[i].red = (sepiaRed > 255) ? 255 : sepiaRed;
   }
}

/* TODO: manejar diferente diferentes bitPerPixel images */
// Increases the red and green colors and make the blue ones less intense
void warmer(BMPImage *image) {
    for (unsigned int i = 0; i < image->pixelsCount; i++) {
        Pixel pixel = image->pixels[i];

        // Calculate the new colors and cast them to int values
        int tr = (int) (pixel.red   * 1.21);
        int tg = (int) (pixel.green * 1.08);
        int tb = (int) (pixel.blue  * 0.89);

        // Clamping to 255 to avoid overflow
        image->pixels[i].blue = tb;
        image->pixels[i].green = (tg > 255) ? 255 : tg;
        image->pixels[i].red = (tr > 255) ? 255 : tr;
   }
}

void greener(BMPImage *image) {
    for (unsigned int i = 0; i < image->pixelsCount; i++) {
        Pixel pixel = image->pixels[i];

        // Calculate the new colors and cast them to int values
        int tr = (int) (pixel.red   * 0.97);
        int tg = (int) (pixel.green * 1.12);
        int tb = (int) (pixel.blue  * 0.89);

        // Clamping to 255 to avoid overflow
        image->pixels[i].blue = tb;
        image->pixels[i].green = (tg > 255) ? 255 : tg;
        image->pixels[i].red = tr;
   }
}

// Applyes a gray scale filter and if gray > 127 then gray = 255 else gray = 0
void superSaturation(BMPImage *image){
    for (unsigned int i = 0; i < image->pixelsCount; i++) {
        Pixel pixel = image->pixels[i];

        unsigned int auxGray = (unsigned int)(0.299 * pixel.red + 0.587 * pixel.green + 0.114 * pixel.blue);
        unsigned char gray = 0;
        if (auxGray > 127) {
            gray = 255;
        } else {
            gray = 0;
        }

        image->pixels[i].red = gray;
        image->pixels[i].green = gray;
        image->pixels[i].blue = gray;
    } 
}
