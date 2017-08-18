/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   July 19, 2017
 */

#ifndef LOGGER_LOGGER_INCLUDED
#define LOGGER_LOGGER_INCLUDED

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
 *  - In case of OOM this function will return NULL and errno is set to ENOMEM.
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
 *  - In case of OOM this function will return NULL and errno is set to ENOMEM.
 *
 * @param self The Logger_T instance.
 * @param name The name for the logger.
 * @return The logger name.
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
 *  - In case of OOM this function will set errno to ENOMEM.
 *
 * @param self The Logger_T instance.
 * @param handler The Logger_Handler_T instance to be added.
 */
extern void Logger_addHandler(Logger_T self, Logger_Handler_T handler);

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
 */
// TODO: understand how to propagate errors
extern void Logger_log(Logger_T self, Logger_Record_T record);

// TODO: error handling
#define _Logger_build(__self__, __level__, __fmt__, ...)                                                                                \
    do {                                                                                                                                \
        Logger_T self = __self__;                                                                                                       \
        Logger_String_T message = Logger_String_from(__fmt__, __VA_ARGS__);                                                             \
        Logger_Record_T record = Logger_Record_new(message, Logger_getName(self), __func__, __FILE__, __LINE__, time(NULL), __level__); \
        Logger_log(self, record);                                                                                                       \
        Logger_Record_delete(&record);                                                                                                  \
        Logger_String_delete(&message);                                                                                                 \
    } while (false)

#define Logger_logDebug(__self__, __fmt__, ...)     _Logger_build(__self__, LOGGER_LEVEL_DEBUG, __fmt__, __VA_ARGS__)
#define Logger_logNotice(__self__, __fmt__, ...)    _Logger_build(__self__, LOGGER_LEVEL_NOTICE, __fmt__, __VA_ARGS__)
#define Logger_logInfo(__self__, __fmt__, ...)      _Logger_build(__self__, LOGGER_LEVEL_INFO, __fmt__, __VA_ARGS__)
#define Logger_logWarning(__self__, __fmt__, ...)   _Logger_build(__self__, LOGGER_LEVEL_WARNING, __fmt__, __VA_ARGS__)
#define Logger_logError(__self__, __fmt__, ...)     _Logger_build(__self__, LOGGER_LEVEL_ERROR, __fmt__, __VA_ARGS__)
#define Logger_logFatal(__self__, __fmt__, ...)     _Logger_build(__self__, LOGGER_LEVEL_FATAL, __fmt__, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_INCLUDED */
