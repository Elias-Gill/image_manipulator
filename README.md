# BMP Image Reader and Manipulator

## Overview

This project is designed for learning purposes and focuses on image
manipulation techniques. It is written in **C** and provides a basic framework
for reading and manipulating BMP image files. The project allows for the
application of various filters to BMP images, enabling users to explore image
processing concepts such as color adjustments and transformations.

## Features

- **BMP File Reading**: The program can read BMP files, extract and print information, 
  and load their pixel data.
- **Image Manipulation**: You can apply multiple filters to the image, such as adjusting colors, 
  brightness, and contrast.
- **Learning Tool**: The project was created to help understand image processing techniques 
  and the BMP file format, with a focus on low-level C programming.

## Learning Goals

- **Understanding BMP File Structure**: Gain insights into how BMP images are
  stored, including headers and pixel data.
- **Memory Management**: Learn how - to manipulate and manage image data in memory, which 
  is crucial for working with large datasets. 
- **Image Filters**: Learn how to apply different types - of filters and transformations to 
  an image, such as color corrections, blurring, etc.
- **C Programming**: Improve my skills in C programming.

## Requirements

- **C Compiler**: Any C99 compatible compiler (e.g., `gcc`).
- **Operating System**: Linux or macOS with `glibc` (C standar library) installed.

## How to Build

Clone and run with make:

```bash
make clean build
./out/bmp_program
```

Alternativelly:

```bash
make run
```
