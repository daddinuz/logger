/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   July 19, 2017
 */

#ifndef LOGGER_LOGGER_INCLUDED
#define LOGGER_LOGGER_INCLUDED

#define LOGGER_VERSION "0.2.0"

#include "logger_err.h"
#include "logger_level.h"
#include "logger_string.h"
#include "logger_record.h"
#include "logger_handler.h"
#include "logger_formatter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Logger_T *Logger_T;

/**
 * Construct a Logger_T.
 *
 * Checked runtime errors:
 *  - @param name must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *  - In case of OOM this function will return NULL.
 *
 * @param name The logger name.
 * @param level The logger level.
 * @return A new Logger_T instance.
 */
extern Logger_T Logger_new(const char *name, Logger_Level_T level);

/**
 * Destruct a Logger_T.
 *
 * Checked runtime errors:
 *  - @param ref must be a valid reference to a Logger_T instance.
 *
 * @param ref The reference to the Logger_T instance.
 */
extern void Logger_delete(Logger_T *ref);

/**
 * Destruct a Logger_T and recursively the handler associated and the relative formatters.
 *
 * Checked runtime errors:
 *  - @param ref must be a valid reference to a Logger_T instance.
 *
 * @param ref The reference to the Logger_T instance.
 */
extern void Logger_deepDelete(Logger_T *ref);

/**
 * Get the name of the specific logger.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_T instance.
 * @return The logger name.
 */
extern const char *Logger_getName(Logger_T self);

/**
 * Get the level for the specific logger.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_T instance.
 * @return The logging level for the logger.
 */
extern Logger_Level_T Logger_getLevel(Logger_T self);

/**
 * Remove a specific handler from the logger.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param handler must not be NULL.
 *
 * @param self The Logger_T instance.
 * @param handler The handler to be removed.
 * @return The removed handler or NULL.
 */
extern Logger_Handler_T Logger_removeHandler(Logger_T self, Logger_Handler_T handler);

/**
 * Remove the last inserted handler from the logger.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_T instance.
 * @return The last handler or NULL if no handler left.
 */
extern Logger_Handler_T Logger_popHandler(Logger_T self);

/**
 * Set the name for the specific logger.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param name must not be NULL.
 *
 * @param self The Logger_T instance.
 * @param name The name for the logger.
 */
extern void Logger_setName(Logger_T self, const char *name);

/**
 * Set the level for the specific logger.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *
 * @param self The Logger_T instance.
 * @param level The level to set.
 */
extern void Logger_setLevel(Logger_T self, Logger_Level_T level);

/**
 * Add a new handler to the specific logger.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param handler must not be NULL.
 *  - In case of OOM this function will return NULL.
 *
 * @param self The Logger_T instance.
 * @param handler The Logger_Handler_T instance to be added.
 * @return The added handler or NULL.
 */
extern Logger_Handler_T Logger_addHandler(Logger_T self, Logger_Handler_T handler);

/**
 * Check if a record with the given level would actually be logged by this logger.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *
 * @param self The Logger_T instance.
 * @return true if the given record would actually be logged by this logger.
 */
extern bool Logger_isLoggable(Logger_T self, Logger_Level_T level);

/**
 * Log a Logger_Record_T.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param record must not be NULL.
 *
 * @param self The Logger_T instance.
 * @param record The Logger_Record_T instance to be logged.
 * @return The `LOGGER_ERR_OK` or the error code.
 */
extern Logger_Err_T Logger_logRecord(Logger_T self, Logger_Record_T record);

/**
 * Construct and log a Logger_Record_T.
 * This function should never be used directly, use the macros instead.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *  - @param file must not be NULL.
 *  - @param function must not be NULL.
 *  - @param fmt must not be NULL.
 *
 * @param self The Logger_T instance.
 * @param level The logging message level.
 * @param file The name of the file in which the logging request was issued.
 * @param line The line of the file in which the logging request was issued.
 * @param function The name of the function in which the logging request was issued.
 * @param timestamp The timestamp in which the logging request was issued (in milliseconds since 1970).
 * @param fmt The printf-like fmt string.
 * @param ...
 * @return The `LOGGER_ERR_OK` or the error code.
 */
extern Logger_Err_T _Logger_log(
        Logger_T self, Logger_Level_T level, const char *file, size_t line, const char *function, time_t timestamp,
        const char *fmt, ...
);

#define _LOGGER_TRACE                         __FILE__, __LINE__, __func__, time(NULL)
#define Logger_log(xSelf, xLevel, xFmt, ...)  _Logger_log(xSelf, xLevel, _LOGGER_TRACE, xFmt, __VA_ARGS__)
#define Logger_logDebug(xSelf, xFmt, ...)     _Logger_log(xSelf, LOGGER_LEVEL_DEBUG, _LOGGER_TRACE, xFmt, __VA_ARGS__)
#define Logger_logNotice(xSelf, xFmt, ...)    _Logger_log(xSelf, LOGGER_LEVEL_NOTICE, _LOGGER_TRACE, xFmt, __VA_ARGS__)
#define Logger_logInfo(xSelf, xFmt, ...)      _Logger_log(xSelf, LOGGER_LEVEL_INFO, _LOGGER_TRACE, xFmt, __VA_ARGS__)
#define Logger_logWarning(xSelf, xFmt, ...)   _Logger_log(xSelf, LOGGER_LEVEL_WARNING, _LOGGER_TRACE, xFmt, __VA_ARGS__)
#define Logger_logError(xSelf, xFmt, ...)     _Logger_log(xSelf, LOGGER_LEVEL_ERROR, _LOGGER_TRACE, xFmt, __VA_ARGS__)
#define Logger_logFatal(xSelf, xFmt, ...)     _Logger_log(xSelf, LOGGER_LEVEL_FATAL, _LOGGER_TRACE, xFmt, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_INCLUDED */
