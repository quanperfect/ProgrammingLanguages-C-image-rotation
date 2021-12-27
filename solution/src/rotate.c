#include "../include/rotate.h"

struct image rotate(struct image const image) {
    struct image new_image = create_image(image.height, image.width);
    for (uint32_t y = 0; y < image.height; y++) {
        for (uint32_t x = 0; x < image.width; x++) {
            *find_pixel_in_image(image.height - 1 - y, x, &new_image) = *find_pixel_in_image(x, y, &image);
		}
	}
    return new_image;
}
