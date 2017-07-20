/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   July 19, 2017 
 */

#include <time.h>
#include "logger.h"
#include "Traits/Traits.h"


TRAITS(CanCreateANewLoggerMessage) {
    Logger_String_T logger_name = "foo";
    Logger_Level_T level = LOGGER_LEVEL_DEBUG;
    Logger_String_T file = __FILE__;
    size_t line = __LINE__;
    Logger_String_T function = __func__;
    time_t timestamp = time(NULL);
    Logger_String_T content = "Hello World!\n";

    Logger_Message_T sut = Logger_Message_new(logger_name, level, file, line, function, timestamp, "%s", content);
    assert_not_null(sut);

    assert_string_equal(logger_name, Logger_Message_getLoggerName(sut));
    assert_equal(level, Logger_Message_getLevel(sut));
    assert_string_equal(file, Logger_Message_getFile(sut));
    assert_equal(line, Logger_Message_getLine(sut));
    assert_string_equal(function, Logger_Message_getFunction(sut));
    assert_equal(timestamp, Logger_Message_getTimestamp(sut));
    assert_string_equal(content, Logger_Message_getContent(sut));

    Logger_Message_delete(&sut);
    assert_null(sut);
}

/*
 *
 */
int main() {
    traits_run(CanCreateANewLoggerMessage);
    return 0;
}

void traits_setup(void) {
    /* Nothing to do */
}

void traits_teardown(void) {
    /* Nothing to do */
}
