#include<stdio.h>

long getFileSize(FILE *fd) {
    fseek(fd, 0, SEEK_END);
    long fileSize = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    return fileSize;
}
