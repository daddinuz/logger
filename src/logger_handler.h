/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 07, 2017 
 */

#ifndef LOGGER_LOGGER_HANDLER_INCLUDED
#define LOGGER_LOGGER_HANDLER_INCLUDED

#include <stdbool.h>
#include "logger_err.h"
#include "logger_record.h"
#include "logger_formatter.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A Logger_Handler_T takes Logger_Record_T from a Logger_T and exports them.
 * It might for example, write them to a console or write them to a file.
 */
typedef struct Logger_Handler_T *Logger_Handler_T;

/**
 * The functions with this signature are used to publish the formatted record.
 *
 * Note for implementation:
 *  - Those functions must assert that handler is not NULL.
 *  - Those functions must assert that record is not NULL.
 *  - In case of OOM those functions will set errno to ENOMEM.
 *  - In case of I/O error those functions will set errno to EIO
 */
typedef Logger_Err_T Logger_Handler_PublishCallback_T(Logger_Handler_T handler, Logger_Record_T record);

/**
 * The functions with this signature are used to flush the buffer associated to the handler.
 *
 * Note for implementation:
 *  - Those functions must assert that handler is not NULL.
 */
typedef void Logger_Handler_FlushCallback_T(Logger_Handler_T handler);

/**
 * The functions with this signature are used to close the buffer associated to the handler
 * and destruct the context associated to the handler.
 *
 * Note for implementation:
 *  - Those functions must assert that handler is not NULL.
 */
typedef void Logger_Handler_CloseCallback_T(Logger_Handler_T handler);

/**
 * Construct a Logger_Handler_T.
 *
 * Checked runtime errors:
 *  - @param publishCallback must not be NULL.
 *  - @param flushCallback must not be NULL.
 *  - @param closeCallback must not be NULL.  
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param publishCallback The callback used to publish the formatter record.
 * @param flushCallback The callback used to flush the buffer.
 * @param closeCallback The callback used to destruct the buffer.
 * @return A new instance of Logger_Handler_T.
 */
extern Logger_Handler_T Logger_Handler_new(
        Logger_Handler_PublishCallback_T publishCallback,
        Logger_Handler_FlushCallback_T flushCallback,
        Logger_Handler_CloseCallback_T closeCallback
);

/**
 * Destruct a Logger_Handler_T.
 *
 * Checked runtime errors:
 *  - @param ref must be a valid reference to a Logger_Handler_T instance.
 *
 * @param ref The reference to the Logger_Handler_T instance.
 */
extern void Logger_Handler_delete(Logger_Handler_T *ref);

/**
 * Publish the formatted record.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param record must not be NULL.
 *  - In case of OOM this function will set errno to ENOMEM.
 *  - In case of I/O error this function will set errno to EIO.
 *
 * @param self The Logger_Handler_T instance.
 * @param record A Logger_Record_T instance.
 */
extern Logger_Err_T Logger_Handler_publish(Logger_Handler_T self, Logger_Record_T record);

/**
 * Flush the buffer associated to the handler.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Handler_T instance.
 */
extern void Logger_Handler_flush(Logger_Handler_T self);

/**
 * Close the buffer associated to the handler.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Handler_T instance.
 */
extern void Logger_Handler_close(Logger_Handler_T self);

/**
 * Check if this handler would actually log a given Logger_Record_T.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param record must not be NULL.
 *
 * @param self The Logger_Handler_T instance.
 * @param record The logging record.
 * @return True if the record will be logged else false.
 */
extern bool Logger_Handler_isLoggable(Logger_Handler_T self, Logger_Record_T record);

/**
 * Get the context associated to the handler.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Handler_T instance.
 * @return The context associated to the handler
 */
extern void *Logger_Handler_getContext(Logger_Handler_T self);

/**
 * Get the Logger_Level_T for the current handler.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Handler_T instance.
 * @return The Logger_Level_T for the current handler.
 */
extern Logger_Level_T Logger_Handler_getLevel(Logger_Handler_T self);

/**
 * Get the formatter associated to the handler.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Handler_T instance.
 * @return The formatter associated to the handler.
 */
extern Logger_Formatter_T Logger_Handler_getFormatter(Logger_Handler_T self);

/**
 * Set the context associated to the handler.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Handler_T instance.
 * @param context The new context for the handler.
 * @return The context associated to the handler
 */
extern void Logger_Handler_setContext(Logger_Handler_T self, void *context);

/**
 * Set the Logger_Level_T for the current handler.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *
 * @param self The Logger_Handler_T instance.
 * @param level The desired Logger_Level_T.
 */
extern void Logger_Handler_setLevel(Logger_Handler_T self, Logger_Level_T level);

/**
 * Set the formatter for the current handler.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param formatter must not be NULL.
 *
 * @param self The Logger_Handler_T instance.
 * @param formatter The formatter to be set.
 */
extern void Logger_Handler_setFormatter(Logger_Handler_T self, Logger_Formatter_T formatter);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_HANDLER_INCLUDED */
