/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 07, 2017 
 */

#ifndef LOGGER_LOGGER_FORMATTER_INCLUDED
#define LOGGER_LOGGER_FORMATTER_INCLUDED

#include "logger_record.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A Logger_Formatter_T provides support for formatting Logger_Record_T.
 * Typically each Logger_Handler_T will have a Formatter associated with it.
 * The Formatter takes a Logger_Record_T and converts it to a string.
 */
typedef struct Logger_Formatter_T *Logger_Formatter_T;

/**
 * The functions with this signature are used to customize the way in which a Logger_Record_T is formatted.
 * It must return an allocated string that will be freed by Logger_Formatter_deleteContentCallback_T.
 *
 * Note for implementation:
 *  - Those functions must assert that record is not NULL.
 *  - Those functions must return NULL and set errno to ENOMEM in case of OOM.
 */
typedef char *(*Logger_Formatter_formatRecordCallback_T)(Logger_Record_T record);

/**
 * The functions with this signature are used to free the memory allocated by Logger_Formatter_formatRecordCallback_T.
 *
 * Note for implementation:
 *  - Those functions must consider the case in which formattedRecord is NULL.
 */
typedef void (*Logger_Formatter_deleteFormattedRecordCallback_T)(char *formattedRecord);

/**
 * Construct a Logger_Formatter_T.
 *
 * Checked runtime errors:
 *  - @param formatRecordCallback must not be NULL.
 *  - @param deleteFormattedRecordCallback must not be NULL.
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param formatRecordCallback Formatter function that will be called to format a Logger_Record_T.
 * @param deleteFormattedRecordCallback Destructor function that will be called by Logger_Formatter_deleteMessage.
 * @return A new instance of Logger_Formatter_T.
 */
extern Logger_Formatter_T Logger_Formatter_new(
        Logger_Formatter_formatRecordCallback_T formatRecordCallback,
        Logger_Formatter_deleteFormattedRecordCallback_T deleteFormattedRecordCallback
);

/**
 * Destruct a Logger_Formatter_T.
 *
 * Checked runtime errors:
 *  - @param ref must be a valid reference to a Logger_Formatter_T instance.
 *
 * @param ref The reference to the Logger_Formatter_T instance.
 */
extern void Logger_Formatter_delete(Logger_Formatter_T *ref);

/**
 * Format the record as specified by the associated formatter callback.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *  - @param record must not be NULL.
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @param self The Logger_Formatter_T instance.
 * @param record A Logger_Record_T instance.
 * @return The formatted message.
 */
extern char *Logger_Formatter_formatRecord(Logger_Formatter_T self, Logger_Record_T record);

/**
 * Destruct the formatted record.
 *
 * Checked runtime errors:
 *  - @param self must not be NULL.
 *
 * @param self The Logger_Formatter_T instance.
 * @param formattedRecord The formatted record to be deleted.
 */
extern void Logger_Formatter_deleteFormattedRecord(Logger_Formatter_T self, char *formattedRecord);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_FORMATTER_INCLUDED */
