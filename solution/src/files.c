#include "../include/files.h"
#include <errno.h>

enum open_status file_open(FILE** file, const char* name, const char* mode) {
    *file = fopen(name, mode);
		
    if (*file) {
        return OPEN_SUCCESS;
    } 
		else if (errno == ENOENT){
        return OPEN_FILE_NOT_EXIST;
    } 
		else if (errno == EACCES) {
        return OPEN_FILE_PERMISSION_DENIED;
    }

    return OPEN_UNKNOWN_ERROR;
}

enum close_status file_close(FILE** file) {
    if (*file) {
        if (fclose(*file) == 0) {
            return CLOSE_SUCCESS;
        } 
				else {
            return CLOSE_UNKNOWN_ERROR;
        }
    }

    return CLOSE_FILE_NOT_EXIST;
}


