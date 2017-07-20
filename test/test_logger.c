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

static Logger_Formatter_T gFormatter = NULL;
static Logger_Handler_T gStdoutHandler = NULL;
static Logger_Handler_T gStderrHandler = NULL;

static char *formatMessageCallback(Logger_Message_T message);
static void deleteMessageCallback(char **self);

TRAITS(CanCreateANewLogger) {
    char expected_name[] = "foo";
    Logger_Level_T expected_level = LOGGER_LEVEL_FATAL;

    Logger_T sut = Logger_new(expected_name, expected_level);
    assert_not_null(sut);

    assert_string_equal(expected_name, Logger_getName(sut));
    assert_equal(expected_level, Logger_getLevel(sut));

    expected_name[0] = 'F';
    assert_string_equal(expected_name, Logger_setName(sut, expected_name));
    assert_string_equal(expected_name, Logger_getName(sut));

    expected_level = LOGGER_LEVEL_DEBUG;
    Logger_setLevel(sut, LOGGER_LEVEL_DEBUG);
    assert_equal(expected_level, Logger_getLevel(sut));

    assert_equal(gStdoutHandler, Logger_addHandler(sut, gStdoutHandler));
    assert_equal(gStderrHandler, Logger_addHandler(sut, gStderrHandler));

    Logger_removeHandler(sut, gStdoutHandler);
    assert_not_null(sut);
    assert_not_null(gStdoutHandler);
    assert_not_null(gStderrHandler);

    Logger_removeHandler(sut, gStderrHandler);
    assert_not_null(sut);
    assert_not_null(gStdoutHandler);
    assert_not_null(gStderrHandler);

    /* double remove should not cause errors */
    Logger_removeHandler(sut, gStdoutHandler);
    assert_not_null(sut);
    assert_not_null(gStdoutHandler);
    assert_not_null(gStderrHandler);

    Logger_removeHandler(sut, gStderrHandler);
    assert_not_null(sut);
    assert_not_null(gStdoutHandler);
    assert_not_null(gStderrHandler);

    /* re-add handlers, Logger_delete should free them */
    assert_equal(gStdoutHandler, Logger_addHandler(sut, gStdoutHandler));
    assert_equal(gStderrHandler, Logger_addHandler(sut, gStderrHandler));

    Logger_logDebug(sut, "Hello %s!\n", "World");
    Logger_logNotice(sut, "Hello %s!\n", "World");
    Logger_logInfo(sut, "Hello %s!\n", "World");
    Logger_logWarning(sut, "Hello %s!\n", "World");
    Logger_logError(sut, "Hello %s!\n", "World");
    Logger_logFatal(sut, "Hello %s!\n", "World");

    Logger_delete(&sut);
    assert_null(sut);
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
    traits_run(CanCreateANewLogger);
    return 0;
}

void traits_setup(void) {
    /*
     * Formatter
     */
    gFormatter = Logger_Formatter_new(formatMessageCallback, deleteMessageCallback);
    assert_not_null(gFormatter);

    assert_equal(formatMessageCallback, Logger_Formatter_getFormatContentCallback(gFormatter));
    assert_equal(deleteMessageCallback, Logger_Formatter_getDeleteContentCallback(gFormatter));

    /*
     * Handler
     */
    gStdoutHandler = Logger_Handler_newStreamHandler(gFormatter, LOGGER_LEVEL_DEBUG, stdout);
    assert_not_null(gStdoutHandler);
    assert_equal(LOGGER_LEVEL_DEBUG, Logger_Handler_getLevel(gStdoutHandler));

    gStderrHandler = Logger_Handler_newStreamHandler(gFormatter, LOGGER_LEVEL_ERROR, stderr);
    assert_not_null(gStderrHandler);
    assert_equal(LOGGER_LEVEL_ERROR, Logger_Handler_getLevel(gStderrHandler));
}

void traits_teardown(void) {
    /*
     * Formatter
     */
    Logger_Formatter_delete(&gFormatter);
    assert_null(gFormatter);

    /*
     * Handler
     */
    Logger_Handler_delete(&gStdoutHandler);
    assert_null(gStdoutHandler);

    Logger_Handler_delete(&gStderrHandler);
    assert_null(gStderrHandler);
}
