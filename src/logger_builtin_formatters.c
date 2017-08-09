/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 04, 2017 
 */

#include <errno.h>
#include <assert.h>
#include "sds/sds.h"
#include "logger_builtin_formatters.h"

/*
 * Logger Formatters Callbacks
 */
static char *formatRecordCallback(Logger_Record_T record) {
    assert(record);
    char time_string[32] = "";
    time_t timestamp = Logger_Record_getTimestamp(record);
    strftime(time_string, sizeof(time_string) / sizeof(time_string[0]), "%Y-%m-%d %H:%M:%S UTC", gmtime(&timestamp));
    sds result = sdscatprintf(
            sdsempty(),
            "%s [%s] %s %s:%zu:%s\n%s\n",
            Logger_Record_getLoggerName(record),
            Logger_Level_getName(Logger_Record_getLevel(record)),
            time_string,
            Logger_Record_getFile(record),
            Logger_Record_getLine(record),
            Logger_Record_getFunction(record),
            Logger_Record_getMessage(record)
    );
    if (!result) {
        errno = ENOMEM;
    }
    return result;
}

static void deleteFormattedRecordCallback(char *formattedRecord) {
    sdsfree(formattedRecord);
}

/*
 * Logger Formatters
 */
Logger_Formatter_T Logger_Formatter_newSimpleFormatter(void) {
    return Logger_Formatter_new(formatRecordCallback, deleteFormattedRecordCallback);
}
