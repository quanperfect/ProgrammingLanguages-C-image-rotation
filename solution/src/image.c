#include "../include/image.h"
#include <malloc.h>

struct pixel* find_pixel_in_image(uint32_t x, uint32_t y, struct image const* const img) {
    return img->data + x + y * img->width;
}

struct image create_image(uint64_t width, uint64_t height) {
    return (struct image) {width, height, malloc(sizeof(struct pixel) * width * height)};
}

void destroy_image(struct image* const img) {
    if (img->data != NULL) free(img->data);
}
