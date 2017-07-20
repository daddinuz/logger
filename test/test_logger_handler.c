/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   July 19, 2017 
 */

#include <errno.h>
#include "sds/sds.h"
#include "logger.h"
#include "Traits/Traits.h"

static Logger_Message_T gMessage = NULL;
static Logger_Formatter_T gFormatter = NULL;

static char *formatMessageCallback(Logger_Message_T message);
static void deleteMessageCallback(char **self);

TRAITS(CanCreateANewLoggerStreamHandler) {
    sds expected_message = formatMessageCallback(gMessage);

    Logger_Handler_T sut = Logger_Handler_newStreamHandler(gFormatter, LOGGER_LEVEL_DEBUG, stdout);
    assert_not_null(sut);

    assert_equal(LOGGER_LEVEL_DEBUG, Logger_Handler_getLevel(sut));
    Logger_Handler_setLevel(sut, LOGGER_LEVEL_INFO);
    assert_equal(LOGGER_LEVEL_INFO, Logger_Handler_getLevel(sut));

    Logger_Buffer_T result = Logger_Handler_publish(sut, gMessage);
    assert_not_null(result);

    assert_equal(sdslen(expected_message), result->size);
    assert_string_equal(expected_message, result->content);
    Logger_Handler_flush(sut);

    Logger_Formatter_deleteMessage(gFormatter, &result->content);
    assert_null(result->content);
    Logger_Buffer_delete(&result);
    assert_null(result);

    Logger_Handler_delete(&sut);
    assert_null(sut);

    sdsfree(expected_message);
}

/*
 *
 */
char *formatMessageCallback(Logger_Message_T message) {
    char time_string[32] = "";
    time_t timestamp = Logger_Message_getTimestamp(message);
    strftime(time_string, sizeof(time_string) / sizeof(time_string[0]), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
    sds result = sdscatprintf(
            sdsempty(), "%s [%s] %s %s:%zu:%s\n%s",
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

void deleteMessageCallback(char **self) {
    assert(self);
    assert(*self);
    sdsfree(*self);
    *self = NULL;
}

int main() {
    traits_run(CanCreateANewLoggerStreamHandler);
    return 0;
}

void traits_setup(void) {
    /*
     * Message
     */
    Logger_String_T logger_name = "foo";
    Logger_Level_T level = LOGGER_LEVEL_DEBUG;
    Logger_String_T file = __FILE__;
    size_t line = __LINE__;
    Logger_String_T function = __func__;
    time_t timestamp = time(NULL);
    Logger_String_T text = "Hello World!\n";

    gMessage = Logger_Message_new(logger_name, level, file, line, function, timestamp, "%s", text);
    assert_not_null(gMessage);

    assert_string_equal(logger_name, Logger_Message_getLoggerName(gMessage));
    assert_equal(level, Logger_Message_getLevel(gMessage));
    assert_string_equal(file, Logger_Message_getFile(gMessage));
    assert_equal(line, Logger_Message_getLine(gMessage));
    assert_string_equal(function, Logger_Message_getFunction(gMessage));
    assert_equal(timestamp, Logger_Message_getTimestamp(gMessage));
    assert_string_equal(text, Logger_Message_getContent(gMessage));

    /*
     * Formatter
     */
    gFormatter = Logger_Formatter_new(formatMessageCallback, deleteMessageCallback);
    assert_not_null(gFormatter);

    assert_equal(formatMessageCallback, Logger_Formatter_getFormatContentCallback(gFormatter));
    assert_equal(deleteMessageCallback, Logger_Formatter_getDeleteContentCallback(gFormatter));
}

void traits_teardown(void) {
    /*
     * Message
     */
    Logger_Message_delete(&gMessage);
    assert_null(gMessage);

    /*
     * Formatter
     */
    Logger_Formatter_delete(&gFormatter);
    assert_null(gFormatter);
}
