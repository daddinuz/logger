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

typedef struct Logger_Result_T {
    Logger_Err_T err;
    Logger_T logger;
} Logger_Result_T;

/**
 * Construct a Logger_T using ConsoleHandler as handler and SimpleFormatter as formatter.
 * The stream used for ConsoleHandler will be set to stdout.
 *
 * Checked runtime errors:
 *  - @param name must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *  - In case of errors this function will set Logger_Result_T.err to the error value.
 *
 * @param name The logger name.
 * @param level The logger and handler level.
 * @return A Logger_Result_T wrapper. If no err occurred logger will be the new instance of a Logger_T.
 */
extern Logger_Result_T Logger_newStdoutLogger(const char *name, Logger_Level_T level);

/**
 * Construct a Logger_T using ConsoleHandler as handler and SimpleFormatter as formatter.
 * The stream used for ConsoleHandler will be set to stderr.
 *
 * Checked runtime errors:
 *  - @param name must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *  - In case of errors this function will set Logger_Result_T.err to the error value.
 *
 * @param name The logger name.
 * @param level The logger and handler level.
 * @return A Logger_Result_T wrapper. If no err occurred logger will be the new instance of a Logger_T.
 */
extern Logger_Result_T Logger_newStderrLogger(const char *name, Logger_Level_T level);

/**
 * Construct a Logger_T using FileHandler as handler and SimpleFormatter as formatter.
 *
 * Checked runtime errors:
 *  - @param name must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *  - @param filePath must not be NULL.
 *  - In case of errors this function will set Logger_Result_T.err to the error value.
 *
 * @param name The logger name.
 * @param level The logger and handler level.
 * @param filePath The path to the file in which the handler will write.
 * @return A Logger_Result_T wrapper. If no err occurred logger will be the new instance of a Logger_T.
 */
extern Logger_Result_T Logger_newFileLogger(const char *name, Logger_Level_T level, const char *filePath);

/**
 * Construct a Logger_T using RotatingFileHandler as handler and SimpleFormatter as formatter.
 *
 * Checked runtime errors:
 *  - @param name must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *  - @param filePath must not be NULL.
 *  - In case of errors this function will set Logger_Result_T.err to the error value.
 *
 * @param name The logger name.
 * @param level The logger and handler level.
 * @param filePath The path to the file in which the handler will write.
 * @param bytesBeforeRotation The number of bytes to be written before rotating.
 * @return A Logger_Result_T wrapper. If no err occurred logger will be the new instance of a Logger_T.
 */
extern Logger_Result_T
Logger_newRotatingFileLogger(const char *name, Logger_Level_T level, const char *filePath, size_t bytesBeforeRotation);

/**
 * Construct a Logger_T using MemoryFileHandler as handler and SimpleFormatter as formatter.
 *
 * Checked runtime errors:
 *  - @param name must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *  - @param filePath must not be NULL.
 *  - In case of errors this function will set Logger_Result_T.err to the error value.
 *
 * @param name The logger name.
 * @param level The logger and handler level.
 * @param filePath The path to the file in which the handler will write.
 * @param bytesBeforeWrite The number of bytes before performing a write.
 * @return A Logger_Result_T wrapper. If no err occurred logger will be the new instance of a Logger_T.
 */
extern Logger_Result_T
Logger_newMemoryFileLogger(const char *name, Logger_Level_T level, const char *filePath, size_t bytesBeforeWrite);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_BUILTIN_LOGGERS_INCLUDED */
