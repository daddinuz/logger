/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 26, 2017 
 */

#include <assert.h>
#include <stdbool.h>
#include "logger_err.h"

static const char *LOGGER_ERR_OK_STR = "";
static const char *LOGGER_ERR_IO_STR = "I/O error.";
static const char *LOGGER_ERR_NO_SPACE_STR = "No space left on device.";
static const char *LOGGER_ERR_NO_SUCH_FILE_STR = "No such file or directory.";
static const char *LOGGER_ERR_NO_PERMISSION_STR = "Operation not permitted.";
static const char *LOGGER_ERR_IS_NOT_A_FILE_STR = "Is not a file.";
static const char *LOGGER_ERR_FILE_TOO_LARGE_STR = "File too large.";
static const char *LOGGER_ERR_FILENAME_TOO_LONG_STR = "Filename too long.";
static const char *LOGGER_ERR_TOO_MANY_OPEN_FILE_STR = "Too many files open in system.";
static const char *LOGGER_ERR_OUT_OF_MEMORY_STR = "Out of memory";
static const char *LOGGER_ERR_UNKNOWN_STR = "Unknown error.";

const char *Logger_Err_gerString(Logger_Err_T err) {
    switch (err) {
        case LOGGER_ERR_OK:
            return LOGGER_ERR_OK_STR;
        case LOGGER_ERR_IO:
            return LOGGER_ERR_IO_STR;
        case LOGGER_ERR_NO_SPACE:
            return LOGGER_ERR_NO_SPACE_STR;
        case LOGGER_ERR_NO_ENTITY:
            return LOGGER_ERR_NO_SUCH_FILE_STR;
        case LOGGER_ERR_NO_PERMISSION:
            return LOGGER_ERR_NO_PERMISSION_STR;
        case LOGGER_ERR_IS_NOT_A_FILE:
            return LOGGER_ERR_IS_NOT_A_FILE_STR;
        case LOGGER_ERR_FILE_TOO_LARGE:
            return LOGGER_ERR_FILE_TOO_LARGE_STR;
        case LOGGER_ERR_FILENAME_TOO_LONG:
            return LOGGER_ERR_FILENAME_TOO_LONG_STR;
        case LOGGER_ERR_TOO_MANY_OPEN_FILE:
            return LOGGER_ERR_TOO_MANY_OPEN_FILE_STR;
        case LOGGER_ERR_OUT_OF_MEMORY:
            return LOGGER_ERR_OUT_OF_MEMORY_STR;
        case LOGGER_ERR_UNKNOWN:
            return LOGGER_ERR_UNKNOWN_STR;
        default:
            assert(false);
    }
}

Logger_Err_T Logger_Err_fromErrno(int e) {
    switch (e) {
        case 0:
            return LOGGER_ERR_OK;
        case EIO:
            return LOGGER_ERR_IO;
        case ENOSPC:
            return LOGGER_ERR_NO_SPACE;
        case ENOENT:
            return LOGGER_ERR_NO_ENTITY;
        case EACCES:
            return LOGGER_ERR_NO_PERMISSION;
        case EISDIR:
            return LOGGER_ERR_IS_NOT_A_FILE;
        case EFBIG:
            return LOGGER_ERR_FILE_TOO_LARGE;
        case ENAMETOOLONG:
            return LOGGER_ERR_FILENAME_TOO_LONG;
        case ENFILE:
            return LOGGER_ERR_TOO_MANY_OPEN_FILE;
        case ENOMEM:
            return LOGGER_ERR_OUT_OF_MEMORY;
        default:
            return LOGGER_ERR_UNKNOWN;
    }
}
