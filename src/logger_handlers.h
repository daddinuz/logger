/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 04, 2017 
 */

#ifndef LOGGER_LOGGER_HANDLERS_INCLUDED
#define LOGGER_LOGGER_HANDLERS_INCLUDED

#include "logger.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates and initializes a Logger_Handler_T.
 *
 * Checked runtime errors:
 *  - @param stream must not be NULL.
 *  - @param level must be a valid Logger_Level_T.
 *  - @param formatter must not be NULL.
 *  - In case of file handling errors this function will return NULL, errno will be set to any value returned by `fopen`
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param stream The stream in which this handler will write.
 * @param level The level for this handler.
 * @param formatter The formatter for this handler.
 * @return A new instance of Logger_Handler_T.
 */
extern Logger_Handler_T Logger_Handler_newStreamHandler(
        FILE *stream, Logger_Level_T level, Logger_Formatter_T formatter
);

/**
 * Allocates and initializes a Logger_Handler_T.
 *
 * Checked runtime errors:
 *  - @param filePath must not be NULL.
 *  - @param level must be a valid Logger_Level_T.
 *  - @param formatter must not be NULL.
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param filePath The path to the file in which the handler will write.
 * @param level The level for this handler.
 * @param formatter The formatter for this handler.
 * @return A new instance of Logger_Handler_T.
 */
extern Logger_Handler_T Logger_Handler_newFileHandler(
        Logger_String_T filePath, Logger_Level_T level, Logger_Formatter_T formatter
);

/**
 * Allocates and initializes a Logger_Handler_T.
 *
 * Checked runtime errors:
 *  - @param filePath must not be NULL.
 *  - @param level must be a valid Logger_Level_T.
 *  - @param formatter must not be NULL.
 *  - In case of file handling errors this function will return NULL, errno will be set to any value returned by `fopen`
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param filePath The path to the file in which the handler will write.
 * @param level The level for this handler.
 * @param formatter The formatter for this handler.
 * @param bytesBeforeRotation The number of bytes to be written before rotating.
 * @return A new instance of Logger_Handler_T.
 */
extern Logger_Handler_T Logger_Handler_newRotatingHandler(
        Logger_String_T filePath, Logger_Level_T level, Logger_Formatter_T formatter, size_t bytesBeforeRotation
);

// TODO
extern Logger_Handler_T Logger_Handler_newBufferHandler(
        Logger_String_T filePath, Logger_Level_T level, Logger_Formatter_T formatter, size_t bytes
);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_HANDLERS_INCLUDED */
