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
#include <stdarg.h>

#define LOGGER_VERSION "0.1.0"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Strings
 */
typedef const char *Logger_String_T;

/*
 * Buffers
 */
typedef struct Logger_Buffer_T {
    size_t size;
    char *content;
} *Logger_Buffer_T;

/**
 * Allocates and initializes a new Logger_Buffer_T.
 *
 * Checked runtime errors:
 *  - @param content must not be NULL.
 *
 * @param size The size of the buffer.
 * @param content The data held by the buffer.
 * @return A new instance of Logger_Buffer_T.
 */
extern Logger_Buffer_T Logger_Buffer_new(size_t size, char *content);

/**
 * Deletes a Logger_Buffer_T instance and frees its memory then sets self to NULL.
 * This function will not free the content of the buffer.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL and must be a valid reference to a Logger_Buffer_T instance.
 *
 * @param self The reference to the Logger_Buffer_T instance.
 */
extern void Logger_Buffer_delete(Logger_Buffer_T *self);

/*
 * Levels
 */
typedef enum Logger_Level_T {
    LOGGER_LEVEL_DEBUG = 0,
    LOGGER_LEVEL_NOTICE,
    LOGGER_LEVEL_INFO,
    LOGGER_LEVEL_WARNING,
    LOGGER_LEVEL_ERROR,
    LOGGER_LEVEL_FATAL
} Logger_Level_T;

/**
 * Returns the string version of the Logger_Level_T.
 *
 * Checked runtime errors:
 *  - @param level must be a valid Logger_Level_T.
 *
 * @param level The log level.
 * @return The string version of the Logger_Level_T.
 */
extern Logger_String_T Logger_Level_getName(Logger_Level_T level);

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
 *  - @param fmt must not be NULL.
 *  - @param args must not be NULL.
 *  - In case of OOM this function returns NULL and errno is set ENOMEM.
 *
 * @param logger_name The name of the logger that has built this message.
 * @param level The log level for this message.
 * @param file The file in which this message has been logged.
 * @param line The line in which this message has been logged.
 * @param function The function in which this message has been logged.
 * @param timestamp The timestamp in which this message has been logged.
 * @param fmt A printf-like format string.
 * @param args The arguments list for fmt.
 * @return A new instance of Logger_Message_T.
 */
extern Logger_Message_T Logger_Message_newFromArgumentsPack(
        Logger_String_T logger_name, Logger_Level_T level, Logger_String_T file, size_t line,
        Logger_String_T function, time_t timestamp, const char *fmt, va_list args
);

/**
 * Allocates and initializes a new Logger_Message_T.
 *
 * Checked runtime errors:
 *  - @param logger_name must not be NULL.
 *  - @param level must be a valid Logger_Level_T.
 *  - @param file must not be NULL.
 *  - @param function must not be NULL.
 *  - @param fmt must not be NULL.
 *  - In case of OOM this function returns NULL and errno is set ENOMEM.
 *
 * @param logger_name The name of the logger that has built this message.
 * @param level The log level for this message.
 * @param file The file in which this message has been logged.
 * @param line The line in which this message has been logged.
 * @param function The function in which this message has been logged.
 * @param timestamp The timestamp in which this message has been logged.
 * @param fmt A printf-like format string.
 * @param ... The arguments for fmt.
 * @return A new instance of Logger_Message_T.
 */
extern Logger_Message_T Logger_Message_newFromVariadicArguments(
        Logger_String_T logger_name, Logger_Level_T level, Logger_String_T file, size_t line,
        Logger_String_T function, time_t timestamp, const char *fmt, ...
);

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
extern Logger_String_T Logger_Message_getContent(Logger_Message_T self);

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
 * The functions with this signature are used to customize the way in which a message is formatted.
 * It must return an allocated string that will be freed by Logger_Formatter_deleteContentCallback_T.
 *
 * Note for implementation:
 *  - Those functions must assert that message is not NULL (do not handle the case in which message is NULL).
 *  - Those functions must return NULL and set errno to ENOMEM in case of OOM.
 */
typedef char *(*Logger_Formatter_formatContentCallback_T)(Logger_Message_T message);

/**
 * The functions with this signature are used to free the memory allocated by Logger_Formatter_formatContentCallback_T.
 *
 * Note for implementation:
 *  - Those functions must assert that content and the pointed value are not NULL.
 *  - Those functions must set the reference of content to NULL before returning.
 */
typedef void (*Logger_Formatter_deleteContentCallback_T)(char **content);

/**
 * Allocates and initializes a Logger_Formatter_T.
 *
 * Checked runtime errors:
 *  - @param formatContentCallback must not be NULL.
 *  - @param deleteContentCallback must not be NULL.
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param formatContentCallback Formatter function that will be called by Logger_Formatter_formatMessage.
 * @param deleteContentCallback Destructor function that will be called by Logger_Formatter_deleteMessage.
 * @return A new instance of Logger_Formatter_T.
 */
extern Logger_Formatter_T Logger_Formatter_new(
        Logger_Formatter_formatContentCallback_T formatContentCallback,
        Logger_Formatter_deleteContentCallback_T deleteContentCallback
);

/**
 * Returns the formatter callback.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self A Logger_Formatter_T instance.
 * @return The formatter callback.
 */
extern Logger_Formatter_formatContentCallback_T Logger_Formatter_getFormatContentCallback(Logger_Formatter_T self);

/**
 * Returns the destructor callback.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self A Logger_Formatter_T instance.
 * @return The destructor callback.
 */
extern Logger_Formatter_deleteContentCallback_T Logger_Formatter_getDeleteContentCallback(Logger_Formatter_T self);

/**
 * Format the message as specified by callback.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param message must not be NULL.
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param self A Logger_Formatter_T instance.
 * @param message A Logger_Message_T instance.
 * @return The formatted message.
 */
extern char *Logger_Formatter_formatMessage(Logger_Formatter_T self, Logger_Message_T message);

/**
 * Deletes the formatted message and frees its memory then sets message to NULL.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param content must be a valid reference to a Logger_Message_T instance.
 *
 * @param self A Logger_Formatter_T instance.
 * @param content The reference of the message to be deleted.
 */
extern void Logger_Formatter_deleteMessage(Logger_Formatter_T self, char **content);

/**
 * Deletes a Logger_Formatter_T instance and frees its memory then sets self to NULL.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL and must be a valid reference to a Logger_Formatter_T instance.
 *
 * @param self The reference to the Logger_Formatter_T instance.
 */
extern void Logger_Formatter_delete(Logger_Formatter_T *self);

/*
 * Handlers
 */
typedef struct Logger_Handler_T *Logger_Handler_T;

/**
 * The functions with this signature are used to customize the way in which the formatted message is logged.
 *
 * Note for implementation:
 *  - Those functions must assert that handler is not NULL.
 *  - Those functions must assert that file is not NULL.
 *  - Those functions must assert that content is not NULL.
 */
typedef Logger_Buffer_T (*Logger_Handler_publishCallback_T)(
        Logger_Handler_T handler, FILE *file, void *context, char *content
);

/**
 * The functions with this signature are used to free the context related to the handler.
 *
 * Note for implementation:
 *  - Those functions must assert that context and the pointed value are not NULL.
 *  - Those functions must set the reference of context to NULL before returning.
 */
typedef void (*Logger_Handler_deleteContextCallback_T)(void **context);

/**
 * Allocates and initializes a Logger_Handler_T.
 *
 * Checked runtime errors:
 *  - @param formatter must not be NULL.
 *  - @param level must be a valid Logger_Level_T.
 *  - @param file must not be NULL.
 *  - @param publishCallback must not be NULL.
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param file The stream in which this handler will write.
 * @param level The level for this handler.
 * @param formatter The formatter for this handler.
 * @param publishCallback The callback to be applied for publishing the messages.
 * @param context The callback used to allocate a new context.
 * @param deleteContextCallback The callback used to free a context.
 * @return A new instance of Logger_Handler_T.
 */
extern Logger_Handler_T Logger_Handler_new(
        FILE *file, void *context, Logger_Level_T level, Logger_Formatter_T formatter,
        Logger_Handler_publishCallback_T publishCallback, Logger_Handler_deleteContextCallback_T deleteContextCallback
);

/**
 * Flushes the file associated to the handler.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self A Logger_Handler_T instance.
 */
extern void Logger_Handler_flush(Logger_Handler_T self);

/**
 * Publishes the Logger_Message_T.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param message must not be NULL.
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *  - In case of I/O error this function will set errno to EIO and Logger_Buffer_T
 *    size will be set to the bytes of data actually written.
 *
 * @param self A Logger_Handler_T instance.
 * @param message A Logger_Message_T instance.
 * @return The data that has been published
 */
extern Logger_Buffer_T Logger_Handler_publish(Logger_Handler_T self, Logger_Message_T message);

/**
 * Sets the Logger_Level_T for the current handler.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param level must be a valid Logger_Level_T.
 *
 * @param self A Logger_Handler_T instance.
 * @param level The desired Logger_Level_T.
 */
extern void Logger_Handler_setLevel(Logger_Handler_T self, Logger_Level_T level);

/**
 * Gets the Logger_Level_T for the current handler.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self A Logger_Handler_T instance.
 * @return The Logger_Level_T for the current handler.
 */
extern Logger_Level_T Logger_Handler_getLevel(Logger_Handler_T self);

/**
 * Sets the number of bytes that has been written by the handler.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self A Logger_Handler_T instance.
 */
extern void Logger_Handler_setBytesWritten(Logger_Handler_T self, size_t bytes_written);

/**
 * Gets the number of bytes that has been written by the handler.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self A Logger_Handler_T instance.
 * @return The number of bytes written by the handlers.
 */
extern size_t Logger_Handler_getBytesWritten(Logger_Handler_T self);

/**
 * Deletes a Logger_Handler_T instance and frees its memory then sets self to NULL.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL and must be a valid reference to a Logger_Handler_T instance.
 *
 * @param self The reference to the Logger_Handler_T instance.
 */
extern void Logger_Handler_delete(Logger_Handler_T *self);

/*
 * Loggers
 */
typedef struct Logger_T *Logger_T;

/**
 * Allocates and initializes a new Logger_T instance.
 *
 * Checked runtime errors:
 *  - @param name must not be NULL.
 *  - @param level must be a valid Logger_Level_T.
 *  - In case of OOM this function will return NULL and errno is set to ENOMEM.
 *
 * @param name The logger name.
 * @param level The logger level.
 * @return A new Logger_T instance.
 */
extern Logger_T Logger_new(Logger_String_T name, Logger_Level_T level);

/**
 * Sets name for the specific logger.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param name must not be NULL.
 *  - In case of OOM this function will return NULL and errno is set to ENOMEM.
 *
 * @param self The Logger_T instance.
 * @param name The new name for the logger.
 * @return The new logger name.
 */
extern Logger_String_T Logger_setName(Logger_T self, Logger_String_T name);

/**
 * Gets the name of the specific logger.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_T instance.
 * @return The logger name.
 */
extern Logger_String_T Logger_getName(Logger_T self);

/**
 * Sets level for the specific logger.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param level must be a valid Logger_Level_T.
 *
 * @param self The Logger_T instance.
 * @param level The level to set.
 */
extern void Logger_setLevel(Logger_T self, Logger_Level_T level);

/**
 * Gets the level for the specific logger.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_T instance.
 * @return The logging level for the logger.
 */
extern Logger_Level_T Logger_getLevel(Logger_T self);

/**
 * Adds a new handler to the specific logger.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param handler must not be NULL.
 *  - In case of OOM this function will return NULL and errno is set to ENOMEM.
 *
 * @param self The Logger_T instance.
 * @param handler The Logger_Handler_T instance to be added.
 * @return The added handler; *
 */
extern Logger_Handler_T Logger_addHandler(Logger_T self, Logger_Handler_T handler);

/**
 * Removes a handler from the logger.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL;
 *  - @param handler must not be NULL;
 *
 * @param self The Logger_T instance.
 * @param handler The handler to be removed.
 */
extern void Logger_removeHandler(Logger_T self, Logger_Handler_T handler);

/**
 * Logs the message.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param level must be a valid Logger_Level_T.
 *  - @param file must not be NULL.
 *  - @param function must not be NULL.
 *  - @param fmt must not be NULL.
 *
 * @param self The Logger_T instance.
 * @param level The log level.
 * @param file The file in which this function has been called.
 * @param line The line of the file in which this function has been called.
 * @param function The outer function in which this function has been called.
 * @param timestamp The timestamp in which this function has been called.
 * @param fmt A printf-like format string.
 * @param ... The fmt arguments.
 */
/* TODO: Error handling */
extern void _Logger_log(
        Logger_T self, Logger_Level_T level, Logger_String_T file, size_t line, Logger_String_T function,
        time_t timestamp, const char *fmt, ...
);

#define Logger_log(self, level, fmt, ...)      _Logger_log(self, level, __FILE__, __LINE__, __func__, time(NULL), fmt, __VA_ARGS__)

#define Logger_logDebug(self, fmt, ...)        Logger_log(self, LOGGER_LEVEL_DEBUG, fmt, __VA_ARGS__)
#define Logger_logNotice(self, fmt, ...)       Logger_log(self, LOGGER_LEVEL_NOTICE, fmt, __VA_ARGS__)
#define Logger_logInfo(self, fmt, ...)         Logger_log(self, LOGGER_LEVEL_INFO, fmt, __VA_ARGS__)
#define Logger_logWarning(self, fmt, ...)      Logger_log(self, LOGGER_LEVEL_WARNING, fmt, __VA_ARGS__)
#define Logger_logError(self, fmt, ...)        Logger_log(self, LOGGER_LEVEL_ERROR, fmt, __VA_ARGS__)
#define Logger_logFatal(self, fmt, ...)        Logger_log(self, LOGGER_LEVEL_FATAL, fmt, __VA_ARGS__)

/**
 * Deletes a Logger_T instance and frees its memory then sets self to NULL.
 *
 * Checked runtime errors:
 *  - @param self must be a valid reference to a Logger_T instance.
 *
 * @param self The reference to the Logger_T instance.
 */
extern void Logger_delete(Logger_T *self);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_INCLUDED */
