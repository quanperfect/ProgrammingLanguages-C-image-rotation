#ifndef BMP_H
#define BMP_H

#include "image.h"
#include <stdio.h>

enum bmp_status {
    BMP_INVALID_HEADER = 0,
    BMP_ERROR,
    BMP_SUCCESS
};

enum bmp_status to_bmp(FILE* file, struct image const img);

enum bmp_status from_bmp(FILE* file, struct image* const img);

#endif
