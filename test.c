#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "bmp/bmp.h"

#define OUT_TEST_FILE "out/image_test.bmp"
#define BMP_VARIANTS_FOLDER "test_images/"

// List of BMP files to test
static char *BMP_FILES[] = {
    BMP_VARIANTS_FOLDER "16bpp.bmp",
    BMP_VARIANTS_FOLDER "24bpp.bmp",
    BMP_VARIANTS_FOLDER "32bpp.bmp"
};

#define NUM_BMP_FILES (sizeof(BMP_FILES) / sizeof(BMP_FILES[0]))

// ANSI color codes
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"

// Compare two files byte by byte
int compareFiles(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "rb");
    FILE *f2 = fopen(file2, "rb");

    if (!f1 || !f2) {
        fprintf(stderr, "%sError: Failed to open files for comparison.%s\n", COLOR_RED, COLOR_RESET);
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return 1;
    }

    int result = 0;
    unsigned char buf1[4096], buf2[4096];

    size_t size1, size2;
    do {
        size1 = fread(buf1, 1, sizeof(buf1), f1);
        size2 = fread(buf2, 1, sizeof(buf2), f2);

        if (size1 != size2 || memcmp(buf1, buf2, size1) != 0) {
            result = 1;
            break;
        }
    } while (size1 > 0 && size2 > 0);

    fclose(f1);
    fclose(f2);

    return result;
}

int testImageParsing() {
    int all_tests_passed = 1; // Assume all tests will pass initially
    for (size_t i = 0; i < NUM_BMP_FILES; i++) {
        char *input_file = BMP_FILES[i];

        BMPImage *image = malloc(sizeof(BMPImage));
        if (image == NULL) {
            fprintf(stderr, "%sError: Memory allocation failed for file: %s%s\n", COLOR_RED, input_file, COLOR_RESET);
            all_tests_passed = 0;
            continue;
        }

        int err = loadImage(input_file, image);
        if (err != 0) {
            fprintf(stderr, "%sError: Failed to load image: %s%s\n", COLOR_RED, input_file, COLOR_RESET);
            all_tests_passed = 0;
            continue;
        }

        err = saveImage(OUT_TEST_FILE, image);
        if (err != 0) {
            fprintf(stderr, "%sError: Failed to save image: %s%s\n", COLOR_RED, input_file, COLOR_RESET);
            freeImage(image);
            all_tests_passed = 0;
            continue;
        }

        if (compareFiles(input_file, OUT_TEST_FILE) != 0) {
            fprintf(stderr, "%sError: File comparison failed for: %s%s\n", COLOR_RED, input_file, COLOR_RESET);
            freeImage(image);
            all_tests_passed = 0;
            continue;
        }

        freeImage(image);
    }

    return all_tests_passed;
}

// Helper function for comparing pixels
int comparePixels(Pixel a, Pixel b) {
    return a.red == b.red && a.green == b.green && a.blue == b.blue;
}

int testGetPixel() {
    int all_tests_passed = 1; // Assume all tests will pass initially

    // Test image setup
    Pixel pixels[6] = {
        {0, 255, 0}, {255, 0, 0},
        {0, 0, 255}, {255, 255, 255},
        {0, 0, 255}, {255, 255, 255}
    };
    Pixel emptyPixel = {.red = 0, .green = 0, .blue = 0};

    BMPImage image = {
        .infoHeader = {40, .width = 2, .height = 3, 1, 24, 0, 0, 0, 0, 0, 0},
        .pixels = pixels,
        .pixelsCount = 6
    };

    void checkPixel(BMPImage *image, int x, int y, Pixel expected) {
        Pixel actual = getPixel(image, x, y, emptyPixel);
        if (!comparePixels(actual, expected)) {
            fprintf(stderr, "Error: Pixel mismatch at (%d, %d): expected (%d, %d, %d), got (%d, %d, %d)\n",
                    x, y,
                    expected.red, expected.green, expected.blue,
                    actual.red, actual.green, actual.blue);
            all_tests_passed = 0;
        }
    }

    // Tests within bounds
    checkPixel(&image, 0, 0, pixels[0]);
    checkPixel(&image, 1, 0, pixels[1]);
    checkPixel(&image, 0, 2, pixels[4]);
    checkPixel(&image, 0, 1, pixels[2]);

    // Tests out of bounds (should return emptyPixel)
    checkPixel(&image, 3, 0, emptyPixel); // x out of bounds
    checkPixel(&image, 0, 3, emptyPixel); // y out of bounds
    checkPixel(&image, -1, 0, emptyPixel); // negative x
    checkPixel(&image, 0, -1, emptyPixel); // negative y
    checkPixel(&image, 2, 2, emptyPixel);  // both out of bounds

    return all_tests_passed;
}


int main() {
    int all_tests_passed = 1; // Assume all tests will pass initially

    // test functions
    all_tests_passed = testImageParsing() & 1;
    all_tests_passed = testGetPixel() & 1;

    if (all_tests_passed) {
        printf("%sAll tests passed!%s\n", COLOR_GREEN, COLOR_RESET);
        return 0;
    } else {
        fprintf(stderr, "%sSome tests failed!%s\n", COLOR_RED, COLOR_RESET);
        return 1;
    }
}
