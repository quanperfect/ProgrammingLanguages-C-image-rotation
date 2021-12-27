#include "../include/bmp.h"
#include <stdbool.h>
#include <stdint.h>

#pragma pack(push, 1)
struct bmp_header {
    uint16_t bfSignature;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)

static uint8_t create_padding(uint32_t width) {
    return (width % 4 == 0) ? 0 : (4 - (width * sizeof(struct pixel)) % 4);
}

static bool check_signature(const struct bmp_header* header) {
    unsigned char b_byte, m_byte;
    b_byte = header->bfSignature & 0xFF;
    m_byte = (header->bfSignature >> 8) & 0xFF;
    if (b_byte == 'B' && m_byte == 'M') {
        return true;
    }
    return false;
}

enum bmp_status from_bmp(FILE* file, struct image* const img) {
    struct bmp_header header;
    size_t result = fread(&header, sizeof(struct bmp_header), 1, file);
    if (result != 1) return BMP_ERROR;

    if (!check_signature(&header)) return BMP_INVALID_HEADER;

    *img = create_image(header.biWidth, header.biHeight);
    for (uint32_t y = 0; y < img->height; y++) {
        size_t result = fread(find_pixel_in_image(0, y, img), sizeof(struct pixel), img->width, file);
        
        if (result != img->width) return BMP_ERROR;

        result = fseek(file, create_padding(img->width), SEEK_CUR);
        if (result != 0) return BMP_ERROR;
    }

    return BMP_SUCCESS;
}

struct bmp_header create_header(uint32_t width, uint32_t height) {
    const uint32_t HEADER_SIZE = 40;
    const uint32_t OFF_BITS_HEADER_SIZE = 14 + HEADER_SIZE;
    const uint32_t IMAGE_SIZE = (sizeof(struct pixel) * width + create_padding(width)) * height;
    const uint32_t FILE_SIZE = OFF_BITS_HEADER_SIZE + IMAGE_SIZE;
    return (struct bmp_header) {
            .bfSignature = ('M' << 8) + 'B',
            .bfileSize = FILE_SIZE,
            .bfReserved = 0,
            .bOffBits = OFF_BITS_HEADER_SIZE,
            .biSize = HEADER_SIZE,
            .biWidth = width,
            .biHeight = height,
            .biPlanes = 1,
            .biBitCount = 24,
            .biCompression = 0,
            .biSizeImage = IMAGE_SIZE,
            .biXPelsPerMeter = 0,
            .biYPelsPerMeter = 0,
            .biClrUsed = 0,
            .biClrImportant = 0
    };
}

enum bmp_status to_bmp(FILE* file, struct image const img) {
    struct bmp_header header = create_header(img.width, img.height);
    size_t result = fwrite(&header, sizeof(struct bmp_header), 1, file);
    
    if (result != 1) return BMP_ERROR;
    
    for (uint32_t y = 0; y < img.height; y++) {
        size_t result = fwrite(find_pixel_in_image(0, y, &img), sizeof(struct pixel), img.width, file);
        if (result != img.width) return BMP_ERROR;

        for (int i = 0; i < create_padding(img.width); i++) {
            result = putc(0, file);
            if (result == EOF) return BMP_ERROR;
        }
    }

    return BMP_SUCCESS;
}
