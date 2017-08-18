/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 15, 2017 
 */

#ifndef LOGGER_LOGGER_BUILTIN_LOGGERS_INCLUDED
#define LOGGER_LOGGER_BUILTIN_LOGGERS_INCLUDED

#include "logger.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Construct a Logger_T with a console handler using simple formatter.
 *
 * Checked runtime errors:
 *  - @param name must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *  - @param stream must be one of LOGGER_CONSOLE_STREAM_STDERR or LOGGER_CONSOLE_STREAM_STDOUT.
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param name The logger name.
 * @param level The logger and handler level.
 * @param stream The stream in which this handler will write.
 * @return A new Logger_T instance.
 */
extern Logger_T Logger_newConsoleLogger(const char *name, Logger_Level_T level, Logger_ConsoleStream_T stream);

/**
 * Construct a Logger_T with a file handler using simple formatter.
 *
 * Checked runtime errors:
 *  - @param name must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *  - @param filePath must not be NULL.
 *  - In case of IO errors this function will return NULL and errno will be set to a proper value.
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param name The logger name.
 * @param level The logger and handler level.
 * @param filePath The path to the file in which the handler will write.
 * @return A new Logger_T instance.
 */
extern Logger_T Logger_newFileLogger(const char *name, Logger_Level_T level, const char *filePath);

// TODO
extern Logger_T Logger_newRotatingFileLogger(const char *name, Logger_Level_T level);

// TODO
extern Logger_T Logger_newMemoryFileLogger(const char *name, Logger_Level_T level);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_BUILTIN_LOGGERS_INCLUDED */
