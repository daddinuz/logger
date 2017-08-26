/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 07, 2017 
 */

#ifndef LOGGER_LOGGER_RECORD_INCLUDED
#define LOGGER_LOGGER_RECORD_INCLUDED

#include <time.h>
#include <stddef.h>
#include <stdarg.h>
#include "logger_level.h"
#include "logger_string.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Logger_Record_T structs are used to pass logging requests between
 * the logging framework and individual log Handlers.
 * When a Logger_Record_T is passed into the logging framework it logically belongs
 * to the framework and should no longer be used or updated by the client application.
 */
typedef struct Logger_Record_T *Logger_Record_T;

/**
 * Construct a Logger_Record_T.
 *
 * Checked runtime errors:
 *  - @param message must not be NULL.
 *  - @param loggerName must not be NULL.
 *  - @param function must not be NULL.
 *  - @param file must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *
 * @param loggerName The source logger's name.
 * @param level The logging message level.
 * @param file The name of the file in which the logging request was issued.
 * @param line The line of the file in which the logging request was issued.
 * @param function The name of the function in which the logging request was issued.
 * @param timestamp The timestamp in which the logging request was issued (in milliseconds since 1970).
 * @param message The raw log message, before localization or formatting.
 * @return A new Logger_Record_T instance.
 */
extern Logger_Record_T Logger_Record_new(
        const char *loggerName, Logger_Level_T level, const char *file, size_t line, const char *function,
        time_t timestamp, Logger_String_T message
);

/**
 * Destruct a Logger_Record_T.
 *
 * Checked runtime errors:
 *  - @param ref must be a valid reference to a Logger_Record_T instance.
 *
 * @param ref The reference to the Logger_Record_T instance.
 */
extern void Logger_Record_delete(Logger_Record_T *ref);

/**
 * Get the raw log message, before localization or formatting.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Record_T instance.
 * @return The message associated to the Logger_Record_T instance.
 */
extern Logger_String_T Logger_Record_getMessage(Logger_Record_T self);

/**
 * Get the source logger's name.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Record_T instance.
 * @return The name of the logger that issued the logging request.
 */
extern const char *Logger_Record_getLoggerName(Logger_Record_T self);

/**
 * Get the name of the function in which the logging request was issued.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Record_T instance.
 * @return The name of the function that issued the logging request.
 */
extern const char *Logger_Record_getFunction(Logger_Record_T self);

/**
 * Get the name of the file in which the logging request was issued.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Record_T instance.
 * @return The name of the file that issued the logging request.
 */
extern const char *Logger_Record_getFile(Logger_Record_T self);

/**
 * Get the line of the file in which the logging request was issued.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Record_T instance.
 * @return The line of the file in which the logging request was issued.
 */
extern size_t Logger_Record_getLine(Logger_Record_T self);

/**
 * The timestamp in which the logging request was issued (in milliseconds since 1970).
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Record_T instance.
 * @return The timestamp of the logging request.
 */
extern time_t Logger_Record_getTimestamp(Logger_Record_T self);

/**
 * Get the logging message level.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Record_T instance.
 * @return The logging message level.
 */
extern Logger_Level_T Logger_Record_getLevel(Logger_Record_T self);

/**
 * Set the raw log message, before localization or formatting.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param message must not be NULL.
 *
 * @param self The Logger_Record_T instance.
 * @param message The raw message to be set.
 */
extern void Logger_Record_setMessage(Logger_Record_T self, Logger_String_T message);

/**
 * Set the logger name that issued the request.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param loggerName must not be NULL.
 *
 * @param self The Logger_Record_T instance.
 * @param loggerName The source logger's name.
 */
extern void Logger_Record_setLoggerName(Logger_Record_T self, const char *loggerName);

/**
 * Set the function name in which the logging request was issued.
 * 
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param function must not be NULL.
 *
 * @param self The Logger_Record_T instance.
 * @param function The function name in which the logging request was issued.
 */
extern void Logger_Record_setFunction(Logger_Record_T self, const char *function);

/**
 * Set the file name in which the logging request was issued.
 * 
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param file must not be NULL.
 *
 * @param self The Logger_Record_T instance.
 * @param file The file name in which the logging request was issued.
 */
extern void Logger_Record_setFile(Logger_Record_T self, const char *file);

/**
 * Set the line of the file in which the logging request was issued.
 * 
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Record_T instance.
 * @param line The line of the file in which the logging request was issued.
 */
extern void Logger_Record_setLine(Logger_Record_T self, size_t line);

/**
 * Set the timestamp in which the logging request was issued.
 * 
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Record_T instance.
 * @param timestamp The timestamp in which the logging request was issued.
 */
extern void Logger_Record_setTimestamp(Logger_Record_T self, time_t timestamp);

/**
 * Set the logging level in which the logging request was issued.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *
 * @param self The Logger_Record_T instance.
 * @param level The logging level in which the logging request was issued.
 */
extern void Logger_Record_setLevel(Logger_Record_T self, Logger_Level_T level);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_RECORD_INCLUDED */
