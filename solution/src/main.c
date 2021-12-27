#include "../include/bmp.h"
#include "../include/files.h"
#include "../include/image.h"
#include "../include/rotate.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static const char* const bmp_error_messages[] = {
        "ERROR: THE REQUIRED FILE FORMAT IS BMP\n",
        "ERROR: UNKNOWN ERROR ¯\\_(ツ)_/¯\n"
};

static const char* const open_error_messages[] = {
        "ERROR: FILE DOES NOT EXIST\n",
        "ERROR: PERMISSION DENIED\n",
        "ERROR: UNKNOWN ERROR ¯\\_(ツ)_/¯\n"
};

static const char* const close_error_messages[] = {
        "ERROR: NO SUCH FILE\n",
        "ERROR: UNKNOWN ERROR ¯\\_(ツ)_/¯\n"
};

bool open_status_check (enum open_status open_status) {
    if (open_status != OPEN_SUCCESS) {
        fputs(open_error_messages[open_status], stderr);
        return 1;
    }
    return 0;
}

bool close_status_check (enum close_status close_status) {
    if (close_status != CLOSE_SUCCESS) {
        fputs(close_error_messages[close_status], stderr);
        return 1;
    }
    return 0;
}

bool bmp_status_check (enum bmp_status bmp_status) {
    if (bmp_status != BMP_SUCCESS) {
        fputs(bmp_error_messages[bmp_status], stderr);
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Manual for using this program:\n ./image-transformer \"source image\" \"transformed image\"\n");
        return 1;
    }

    FILE* file;
    enum open_status open_status = file_open(&file, argv[1], "rb");

    if (open_status_check(open_status)) {
        return 1;
    }

    struct image source = {0};
    enum bmp_status bmp_status = from_bmp(file, &source);
    enum close_status close_status = file_close(&file);

    if (close_status_check(close_status)){
        return 1;
    }

    if (bmp_status_check(bmp_status)) {
        destroy_image(&source);
        return 1;
    }

    struct image result = rotate(source);
    destroy_image(&source);

    open_status = file_open(&file, argv[2], "wb");
    
    if (open_status_check(open_status)) {
        destroy_image(&result);
        return 1;
    }

    bmp_status = to_bmp(file, result);

    close_status = file_close(&file);

    if (close_status_check(close_status)){
        return 1;
    }

    destroy_image(&result);

    if (bmp_status_check(bmp_status)) {
        return 1;
    }

    fputs("The rotation was completed successfully.\nCheck the new created file with the name you assigned above.\n", stderr);
    return 0;
}
