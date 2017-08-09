/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 04, 2017 
 */

#ifndef LOGGER_LOGGER_BUILTIN_HANDLERS_INCLUDED
#define LOGGER_LOGGER_BUILTIN_HANDLERS_INCLUDED

#include "logger_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum Logger_ConsoleStream_T {
    LOGGER_CONSOLE_STREAM_STDERR,
    LOGGER_CONSOLE_STREAM_STDOUT,
} Logger_ConsoleStream_T;

/**
 * Construct a Logger_Handler_T.
 *
 * Checked runtime errors:
 *  - @param stream must be one of LOGGER_CONSOLE_STREAM_STDERR or LOGGER_CONSOLE_STREAM_STDOUT.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *  - @param formatter must not be NULL.
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param stream The stream in which this handler will write.
 * @param level The level for this handler.
 * @param formatter The formatter for this handler.
 * @return A new instance of Logger_Handler_T.
 */
extern Logger_Handler_T Logger_Handler_newConsoleHandler(
        Logger_ConsoleStream_T stream, Logger_Level_T level, Logger_Formatter_T formatter
);

/**
 * Construct a Logger_Handler_T.
 *
 * Checked runtime errors:
 *  - @param filePath must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *  - @param formatter must not be NULL.
 *  - In case of IO errors this function will return NULL and errno will be set to a proper value.
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param filePath The path to the file in which the handler will write.
 * @param level The level for this handler.
 * @param formatter The formatter for this handler.
 * @return A new instance of Logger_Handler_T.
 */
extern Logger_Handler_T Logger_Handler_newFileHandler(
        const char *filePath, Logger_Level_T level, Logger_Formatter_T formatter
);

/**
 * Construct a Logger_Handler_T.
 *
 * Checked runtime errors:
 *  - @param filePath must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *  - @param formatter must not be NULL.
 *  - In case of IO errors this function will return NULL and errno will be set to a proper value.
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param filePath The path to the file in which the handler will write.
 * @param level The level for this handler.
 * @param formatter The formatter for this handler.
 * @param bytesBeforeRotation The number of bytes to be written before rotating.
 * @return A new instance of Logger_Handler_T.
 */
extern Logger_Handler_T Logger_Handler_newRotatingFileHandler(
        const char *filePath, Logger_Level_T level, Logger_Formatter_T formatter, size_t bytesBeforeRotation
);

/**
 * Construct a Logger_Handler_T.
 *
 * Checked runtime errors:
 *  - @param filePath must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *  - @param formatter must not be NULL.
 *  - In case of IO errors this function will return NULL and errno will be set to a proper value.
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param filePath The path to the file in which the handler will write.
 * @param level The level for this handler.
 * @param formatter The formatter for this handler.
 * @param bytesBeforeWrite The number of bytes before performing a write.
 * @return A new instance of Logger_Handler_T.
 */
extern Logger_Handler_T Logger_Handler_newMemoryFileHandler(
        const char *filePath, Logger_Level_T level, Logger_Formatter_T formatter, size_t bytesBeforeWrite
);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_BUILTIN_HANDLERS_INCLUDED */
