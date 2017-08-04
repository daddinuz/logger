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
#include "logger_formatters.h"

/*
 * Logger Formatters Callbacks
 */
static char *Logger_Formatter_simpleFormatterFormatMessageCallback(Logger_Message_T message) {
    assert(message);
    char time_string[32] = "";
    time_t timestamp = Logger_Message_getTimestamp(message);
    strftime(time_string, sizeof(time_string) / sizeof(time_string[0]), "%Y-%m-%d %H:%M:%S UTC", gmtime(&timestamp));
    sds result = sdscatprintf(
            sdsempty(),
            "%s [%s] %s %s:%zu:%s\n%s",
            Logger_Message_getLoggerName(message),
            Logger_Level_getName(Logger_Message_getLevel(message)),
            time_string,
            Logger_Message_getFile(message),
            Logger_Message_getLine(message),
            Logger_Message_getFunction(message),
            Logger_Message_getContent(message)
    );
    if (!result) {
        errno = ENOMEM;
    }
    return result;
}

static void Logger_Formatter_simpleFormatterDeleteMessageCallback(char **content) {
    assert(content);
    assert(*content);
    sdsfree(*content);
    *content = NULL;
}

/*
 * Logger Formatters
 */
Logger_Formatter_T Logger_Formatter_newSimpleFormatter(void) {
    return Logger_Formatter_new(
            Logger_Formatter_simpleFormatterFormatMessageCallback,
            Logger_Formatter_simpleFormatterDeleteMessageCallback
    );
}
