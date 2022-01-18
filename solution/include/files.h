#ifndef FILES_H
#define FILES_H

#include <stdio.h>

enum open_status {
	OPEN_FILE_NOT_EXIST,
	OPEN_FILE_PERMISSION_DENIED,
	OPEN_UNKNOWN_ERROR,
	OPEN_SUCCESS
};

enum close_status {
	CLOSE_FILE_NOT_EXIST,
	CLOSE_UNKNOWN_ERROR,
	CLOSE_SUCCESS
};


enum open_status file_open(FILE** file, const char* name, const char* mode);

enum close_status file_close(FILE** file);


#endif

