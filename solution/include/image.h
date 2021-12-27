#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

struct pixel {uint8_t b, g, r;};

struct image {
    uint64_t width, height;
    struct pixel* data;
};

struct pixel* find_pixel_in_image(uint32_t x, uint32_t y, struct image const* const img);

struct image create_image(uint64_t width, uint64_t height);

void destroy_image(struct image* const img);

#endif
