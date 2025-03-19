#include"filters.h"
#include<stdlib.h>
#include<string.h>

static inline unsigned char clamp(int value) {
    return (value > 255) ? 255 : (value < 0) ? 0 : value;
}

unsigned char calculateGray(Pixel pixel) {
    unsigned int gray = (unsigned int)(0.299 * pixel.red + 0.587 * pixel.green + 0.114 * pixel.blue);
    return (gray > 255) ? 255 : gray;
}

void borders(BMPImage *image) {
    unsigned int width = image->infoHeader.width;
    unsigned int height = image->infoHeader.height;

    int mask[3][3] = {
        { 1, -2,  1},
        {-2,  4, -2},
        { 1, -2,  1}
    };

    // Make a copy of the original image's pixel data
    Pixel *aux = malloc(sizeof(Pixel) * image->pixelsCount);
    if (!aux) {
        fprintf(stderr, "Error: Memory allocation failed for image copy.\n");
        return;
    }
    memcpy(aux, image->pixels, sizeof(Pixel) * image->pixelsCount);

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            // Retrieve the current pixel
            unsigned int coordinate = (width * y) + x;
            Pixel currentPixel = image->pixels[coordinate];

            // apply the mask
            int intensity = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int px = x + j;
                    int py = y + i;

                    // Get the pixel value and apply the mask
                    Pixel pixel = getPixel(image, px, py, currentPixel);
                    intensity += calculateGray(pixel) * mask[i + 1][j + 1];
                }
            }

            aux[coordinate].red = clamp(intensity);
            aux[coordinate].blue = clamp(intensity);
            aux[coordinate].green = clamp(intensity);
        }
    }

    free(image->pixels);
    image->pixels = aux;
}

void focus(BMPImage *image) {
    unsigned int width = image->infoHeader.width;
    unsigned int height = image->infoHeader.height;

    int mask[3][3] = {
        {-1, -1, -1},
        {-1,  9, -1},
        {-1, -1, -1}
    };

    // Make a copy of the original image's pixel data
    Pixel *aux = malloc(sizeof(Pixel) * image->pixelsCount);
    if (!aux) {
        fprintf(stderr, "Error: Memory allocation failed for image copy.\n");
        return;
    }
    memcpy(aux, image->pixels, sizeof(Pixel) * image->pixelsCount);

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            // Retrieve the current pixel
            unsigned int coordinate = (width * y) + x;
            Pixel currentPixel = image->pixels[coordinate];

            // Initialize intensities for each channel
            int intensityRed = 0;
            int intensityGreen = 0;
            int intensityBlue = 0;

            // Apply the mask to each color channel
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int px = x + j;
                    int py = y + i;

                    // Get the pixel value
                    Pixel pixel = getPixel(image, px, py, currentPixel);

                    // Apply the mask to each channel
                    intensityRed += pixel.red * mask[i + 1][j + 1];
                    intensityGreen += pixel.green * mask[i + 1][j + 1];
                    intensityBlue += pixel.blue * mask[i + 1][j + 1];
                }
            }

            // Update the pixel in the auxiliary image
            aux[coordinate].red = clamp(intensityRed);
            aux[coordinate].green = clamp(intensityGreen);
            aux[coordinate].blue = clamp(intensityBlue);
        }
    }

    // Replace the original image's pixel data with the modified data
    free(image->pixels);
    image->pixels = aux;
}
