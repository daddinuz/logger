/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 26, 2017 
 */

#ifndef LOGGER_LOGGER_ERR_INCLUDED
#define LOGGER_LOGGER_ERR_INCLUDED

#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum Logger_Err_T {
    LOGGER_ERR_OK,
    LOGGER_ERR_IO,
    LOGGER_ERR_NO_SPACE,
    LOGGER_ERR_NO_ENTITY,
    LOGGER_ERR_NO_PERMISSION,
    LOGGER_ERR_IS_NOT_A_FILE,
    LOGGER_ERR_FILE_TOO_LARGE,
    LOGGER_ERR_FILENAME_TOO_LONG,
    LOGGER_ERR_TOO_MANY_OPEN_FILE,
    LOGGER_ERR_OUT_OF_MEMORY,
    LOGGER_ERR_UNKNOWN,
} Logger_Err_T;

/**
 * Get the string representation of a Logger_Err_T.
 *
 * Checked runtime errors:
 *  - @param err must be in range LOGGER_ERR_OK - LOGGER_ERR_OUT_OF_MEMORY.
 *
 * @param err The error value.
 * @return The string representation of the Logger_Err_T.
 */
extern const char *Logger_Err_gerString(Logger_Err_T err);

/**
 * Get the Logger_Err_T from errno code.
 *
 * @param e The errno code.
 * @return The Logger_Err_T.
 */
extern Logger_Err_T Logger_Err_fromErrno(int e);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_ERR_INCLUDED */
