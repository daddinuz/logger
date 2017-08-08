/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   July 19, 2017
 */

#ifndef LOGGER_LOGGER_INCLUDED
#define LOGGER_LOGGER_INCLUDED

#include "logger_record.h"
#include "logger_formatter.h"
#include "logger_handler.h"

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
 * Remove the handler from the logger.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL;
 *  - @param handler must not be NULL;
 *
 * @param self The Logger_T instance.
 * @param handler The handler to be removed.
 * @return The removed handler or NULL.
 */
extern Logger_Handler_T Logger_removeHandler(Logger_T self, Logger_Handler_T handler);

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

/* TODO: Error handling */
extern void _Logger_log(
        Logger_T self, Logger_Level_T level, time_t timestamp, size_t line,
        const char *file, const char *function, const char *fmt, ...
);

#define Logger_log(self, level, fmt, ...)      _Logger_log(self, level, time(NULL), __LINE__, __FILE__, __func__, fmt, __VA_ARGS__)

#define Logger_logDebug(self, fmt, ...)        Logger_log(self, LOGGER_LEVEL_DEBUG, fmt, __VA_ARGS__)
#define Logger_logNotice(self, fmt, ...)       Logger_log(self, LOGGER_LEVEL_NOTICE, fmt, __VA_ARGS__)
#define Logger_logInfo(self, fmt, ...)         Logger_log(self, LOGGER_LEVEL_INFO, fmt, __VA_ARGS__)
#define Logger_logWarning(self, fmt, ...)      Logger_log(self, LOGGER_LEVEL_WARNING, fmt, __VA_ARGS__)
#define Logger_logError(self, fmt, ...)        Logger_log(self, LOGGER_LEVEL_ERROR, fmt, __VA_ARGS__)
#define Logger_logFatal(self, fmt, ...)        Logger_log(self, LOGGER_LEVEL_FATAL, fmt, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_INCLUDED */
