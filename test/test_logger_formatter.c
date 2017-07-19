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

char *format_fn(Logger_Message_T message);

TRAITS(CanCreateANewLoggerFormatter) {
    sds expected_message = format_fn(gMessage);
    Logger_Formatter_T sut = Logger_Formatter_new(format_fn);
    assert_not_null(sut);

    assert_equal(format_fn, Logger_Formatter_getCallback(sut));
    sds actual_message = Logger_Formatter_format(sut, gMessage);
    assert_string_equal(expected_message, actual_message);
    
    Logger_Formatter_delete(&sut);
    assert_null(sut);

    sdsfree(expected_message);
    sdsfree(actual_message);
}

/*
 *
 */
char *format_fn(Logger_Message_T message) {
    char time_string[32] = "";
    time_t timestamp = Logger_Message_getTimestamp(message);
    strftime(time_string, sizeof(time_string) / sizeof(time_string[0]), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
    sds result = sdscatprintf(sdsempty(), "%s [%s] %s %s:%zu:%s\n%s",
                              Logger_Message_getLoggerName(message),
                              Logger_Level_name(Logger_Message_getLevel(message)),
                              time_string,
                              Logger_Message_getFile(message),
                              Logger_Message_getLine(message),
                              Logger_Message_getFunction(message),
                              Logger_Message_getMessage(message)
    );
    if (!result) {
        errno = ENOMEM;
    }
    return result;
}

int main() {
    traits_run(CanCreateANewLoggerFormatter);
    return 0;
}

void traits_setup(void) {
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
    assert_string_equal(text, Logger_Message_getMessage(gMessage));
}

void traits_teardown(void) {
    Logger_Message_delete(&gMessage);
    assert_null(gMessage);
}
