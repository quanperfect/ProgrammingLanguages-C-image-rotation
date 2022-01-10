#include "../include/bmp.h"
#include "../include/files.h"
#include "../include/image.h"
#include "../include/rotate.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAG   "\x1B[35m"
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define RESET "\x1B[0m"

FILE* file;
enum open_status open_status;
enum close_status close_status;
enum bmp_status bmp_status;
struct image source_image = {0};
struct image result_image = {0};

void program_end_with_error() {
	printf(RED "Program has ended with error code \n " RESET);
	exit(1);
}

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

bool open_status_check(enum open_status open_status) {
    if (open_status != OPEN_SUCCESS) {
				printf(RED "%s\n" RESET, open_error_messages[open_status]);
        program_end_with_error();
    }
    return 0;
}

bool close_status_check(enum close_status close_status) {
    if (close_status != CLOSE_SUCCESS) {
				printf(RED "%s\n" RESET, close_error_messages[close_status]);
        program_end_with_error();
    }
    return 0;
}

bool bmp_status_check(enum bmp_status bmp_status) {
    if (bmp_status != BMP_SUCCESS) {
				printf(RED "%s\n" RESET, bmp_error_messages[bmp_status]);
        program_end_with_error();
    }
    return 0;
}

void pre_check(int argc, char* argv[]) {
	if (argc != 3) {
        printf(RED "Manual for using this program:\n ./image-transformer source_image.bmp transformed_image.bmp\n" RESET);
        program_end_with_error();
    }

    open_status = file_open(&file, argv[1], "rb");

    if (open_status_check(open_status)) {
        program_end_with_error();
    }

    bmp_status = from_bmp(file, &source_image);
    close_status = file_close(&file);

    if (close_status_check(close_status)){
        program_end_with_error();
    }

    if (bmp_status_check(bmp_status)) {
        destroy_image(&source_image);
        program_end_with_error();
    }
}

void get_result_in_file(char* argv[]) {
		
		open_status = file_open(&file, argv[2], "wb");
    
    if (open_status_check(open_status)) {
        destroy_image(&result_image);
        program_end_with_error();
    }

    bmp_status = to_bmp(file, result_image);

    close_status = file_close(&file);

    if (close_status_check(close_status)){
        program_end_with_error();
    }

    destroy_image(&result_image);

    if (bmp_status_check(bmp_status)) {
        program_end_with_error();
    }
}

int main(int argc, char* argv[]) {
		printf(MAG "Image rotation lab by G. Dagil\n" RESET);

		pre_check(argc, argv);
 
    result_image = rotate(source_image);

		get_result_in_file(argv);

		printf(GRN "The rotation was completed successfully." RESET "\nCheck the new created file with the name you assigned above.\n");
    return 0;
}
