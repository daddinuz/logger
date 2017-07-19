/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   July 19, 2017 
 */

#ifndef LOGGER_LOGGER_INCLUDED
#define LOGGER_LOGGER_INCLUDED

#include <time.h>
#include <stdio.h>
#include <stddef.h>

#define LOGGER_VERSION "0.1.0"

#ifdef __cplusplus
extern "C" {
#endif

typedef const char *Logger_String_T;

typedef enum Logger_Mode_T {
    LOGGER_MODE_UNDEFINED = 0,
    LOGGER_MODE_TRUNCATE,
    LOGGER_MODE_APPEND,
} Logger_Mode_T;

typedef enum Logger_Level_T {
    LOGGER_LEVEL_DEBUG = 0,
    LOGGER_LEVEL_NOTICE,
    LOGGER_LEVEL_INFO,
    LOGGER_LEVEL_WARNING,
    LOGGER_LEVEL_ERROR,
    LOGGER_LEVEL_FATAL
} Logger_Level_T;

/*
 * Messages
 */
typedef struct Logger_Message_T *Logger_Message_T;

/**
 * Allocates and initializes a new Logger_Message_T.
 *
 * Checked runtime errors:
 *  - @param logger_name must not be NULL.
 *  - @param level must be a valid Logger_Level_T.
 *  - @param file must not be NULL.
 *  - @param function must not be NULL.
 *  - In case of OOM this function returns NULL and errno is set ENOMEM.
 *
 * Un-checked runtime errors:
 *  - @param ... first argument must be a const char *
 *
 * @param logger_name The name of the logger that has built this message.
 * @param level The log level for this message.
 * @param file The file in which this message has been logged.
 * @param line The line in which this message has been logged.
 * @param function The function in which this message has been logged.
 * @param timestamp The timestamp in which this message has been logged.
 * @param ... Vararg printf-like formatting
 * @return A new instance of Logger_Message_T.
 */
extern Logger_Message_T Logger_Message_new(Logger_String_T logger_name, Logger_Level_T level, Logger_String_T file, size_t line, Logger_String_T function, time_t timestamp, const char *fmt, ...);

/**
 * Returns the name of the logger that has built this message.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self A Logger_Message_T instance.
 * @return The name of the logger that has built this message.
 */
extern Logger_String_T Logger_Message_getLoggerName(Logger_Message_T self);

/**
 * Returns the level of the logger that has built this message.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self A Logger_Message_T instance.
 * @return The level of the logger that has built this message.
 */
extern Logger_Level_T Logger_Message_getLevel(Logger_Message_T self);

/**
 * Returns the file in which this message has been logged.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self A Logger_Message_T instance.
 * @return The file in which this message has been logged.
 */
extern Logger_String_T Logger_Message_getFile(Logger_Message_T self);

/**
 * Returns the line in which this message has been logged.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self A Logger_Message_T instance.
 * @return The line in which this message has been logged.
 */
extern size_t Logger_Message_getLine(Logger_Message_T self);

/**
 * Returns the function in which this message has been logged.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self A Logger_Message_T instance.
 * @return The function in which this message has been logged.
 */
extern Logger_String_T Logger_Message_getFunction(Logger_Message_T self);

/**
 * Returns the timestamp in which this message has been logged.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self A Logger_Message_T instance.
 * @return The timestamp in which this message has been logged.
 */
extern time_t Logger_Message_getTimestamp(Logger_Message_T self);

/**
 * Returns the message in which this message has been logged.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self A Logger_Message_T instance.
 * @return The message in which this message has been logged.
 */
extern Logger_String_T Logger_Message_getMessage(Logger_Message_T self);

/**
 * Deletes a Logger_Message_T instance and frees its memory then sets self to NULL.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL and must be a valid reference to a Logger_Message_T instance.
 *
 * @param self The reference to the Logger_Message_T instance.
 */
extern void Logger_Message_delete(Logger_Message_T *self);

/*
 * Formatters
 */
typedef struct Logger_Formatter_T *Logger_Formatter_T;

/**
 * Allocate and initialize a Logger_Formatter_T.
 *
 * Checked runtime errors:
 *  - format must no be NULL.
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param format Formatting string
 * @return A new instance of Logger_Formatter_T
 */
extern Logger_Formatter_T Logger_Formatter_new(Logger_String_T format);

/**
 *  the message string from a log record.
 *
 * @param message
 * @return
 */
extern Logger_String_T Logger_Formatter_format(Logger_String_T message);
extern void Logger_Formatter_delete(Logger_Formatter_T *self);

/*
 * Handlers
 */
typedef struct Logger_Handler_T *Logger_Handler_T;

extern Logger_Handler_T Logger_Handler_newStreamHandler(Logger_Formatter_T formatter, Logger_Level_T level, FILE *stream);
extern Logger_Handler_T Logger_Handler_newFileHandler(Logger_Formatter_T formatter, Logger_Level_T level, Logger_String_T file_path, Logger_Mode_T mode);
extern Logger_Handler_T Logger_Handler_newRotatingHandler(Logger_Formatter_T formatter, Logger_Level_T level, Logger_String_T file_path, Logger_Mode_T mode, size_t bytes);
extern Logger_Handler_T Logger_Handler_newBufferedHandler(Logger_Formatter_T formatter, Logger_Level_T level, Logger_String_T file_path, Logger_Mode_T mode, size_t bytes);

extern void Logger_Handler_flush(Logger_Handler_T self);

extern void Logger_Handler_setLevel(Logger_Handler_T self, Logger_Level_T level);
extern Logger_Level_T Logger_Handler_getLevel(Logger_Handler_T self);

extern void Logger_Handler_delete(Logger_Handler_T *self);

/*
 * Loggers
 */
typedef struct Logger_T *Logger_T;

extern Logger_T Logger_new(Logger_String_T name, Logger_Level_T level);

extern void Logger_setLevel(Logger_T self, Logger_Level_T level);
extern Logger_Level_T Logger_getLevel(Logger_T self);

extern void Logger_addHandler(Logger_T self, Logger_Handler_T handler);
extern void Logger_removeHandler(Logger_T self, Logger_Handler_T handler);

extern void Logger_logDebug(Logger_T self, Logger_String_T message, ...);
extern void Logger_logNotice(Logger_T self, Logger_String_T message, ...);
extern void Logger_logInfo(Logger_T self, Logger_String_T message, ...);
extern void Logger_logWarning(Logger_T self, Logger_String_T message, ...);
extern void Logger_logError(Logger_T self, Logger_String_T message, ...);
extern void Logger_logFatal(Logger_T self, Logger_String_T message, ...);

extern Logger_T Logger_delete(Logger_T *self);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_INCLUDED */
