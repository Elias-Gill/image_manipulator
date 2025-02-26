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

int main() {
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

    if (all_tests_passed) {
        printf("%sAll tests passed!%s\n", COLOR_GREEN, COLOR_RESET);
        return 0;
    } else {
        fprintf(stderr, "%sSome tests failed!%s\n", COLOR_RED, COLOR_RESET);
        return 1;
    }
}
