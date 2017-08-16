/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 08, 2017
 */

#include "Traits/Traits.h"
#include "logger_builtin_formatters.h"
#include "logger_builtin_handlers.h"
#include "logger.h"

Logger_Formatter_T gFormatter = NULL;
Logger_Handler_T gHandler1 = NULL, gHandler2 = NULL;
Logger_T sut;

/*
 *
 */
TRAITS(CanCreateALogger) {
    const char *EXPECTED_NAME = "EXPECTED_NAME";
    Logger_Level_T EXPECTED_LEVEL = LOGGER_LEVEL_DEBUG;

    sut = Logger_new(EXPECTED_NAME, EXPECTED_LEVEL);
    assert_not_null(sut);

    assert_string_equal(EXPECTED_NAME, Logger_getName(sut));
    assert_equal(EXPECTED_LEVEL, Logger_getLevel(sut));

    EXPECTED_NAME = "NEW_EXPECTED_NAME";
    Logger_setName(sut, EXPECTED_NAME);
    assert_string_equal(EXPECTED_NAME, Logger_getName(sut));

    EXPECTED_LEVEL = LOGGER_LEVEL_FATAL;
    Logger_setLevel(sut, EXPECTED_LEVEL);
    assert_equal(EXPECTED_LEVEL, Logger_getLevel(sut));

    Logger_delete(&sut);
    assert_null(sut);
}

TRAITS(CanManageHandlers) {
    sut = Logger_new("Logger Name", LOGGER_LEVEL_DEBUG);
    assert_not_null(sut);

    assert_null(Logger_popHandler(sut));

    Logger_addHandler(sut, gHandler1);
    assert_equal(gHandler1, Logger_popHandler(sut));
    assert_not_null(gHandler1);
    assert_null(Logger_popHandler(sut));

    Logger_addHandler(sut, gHandler1);
    Logger_addHandler(sut, gHandler2);
    assert_equal(gHandler2, Logger_popHandler(sut));
    assert_not_null(gHandler2);
    assert_equal(gHandler1, Logger_popHandler(sut));
    assert_not_null(gHandler1);
    assert_null(Logger_popHandler(sut));

    Logger_addHandler(sut, gHandler1);
    assert_equal(gHandler1, Logger_removeHandler(sut, gHandler1));
    assert_not_null(gHandler1);
    assert_null(Logger_removeHandler(sut, gHandler1));

    Logger_addHandler(sut, gHandler1);
    Logger_addHandler(sut, gHandler2);
    assert_equal(gHandler1, Logger_removeHandler(sut, gHandler1));
    assert_not_null(gHandler1);
    assert_equal(gHandler2, Logger_removeHandler(sut, gHandler2));
    assert_not_null(gHandler2);
    assert_null(Logger_removeHandler(sut, gHandler1));
    assert_null(Logger_removeHandler(sut, gHandler2));

    Logger_delete(&sut);
    assert_null(sut);
}

/*
 *
 */
int main() {
    traits_run(CanCreateALogger);
    traits_run(CanManageHandlers);
    return 0;
}

/*
 *
 */
void traits_setup(void) {
    assert_null(gFormatter);
    assert_null(gHandler1);
    assert_null(gHandler2);

    gFormatter = Logger_Formatter_newSimpleFormatter();
    assert_not_null(gFormatter);

    gHandler1 = Logger_Handler_newConsoleHandler(LOGGER_CONSOLE_STREAM_STDOUT, LOGGER_LEVEL_DEBUG, gFormatter);
    assert_not_null(gHandler1);

    gHandler2 = Logger_Handler_newConsoleHandler(LOGGER_CONSOLE_STREAM_STDERR, LOGGER_LEVEL_ERROR, gFormatter);
    assert_not_null(gHandler2);
}

void traits_teardown(void) {
    assert_not_null(gHandler2);
    assert_not_null(gHandler1);
    assert_not_null(gFormatter);

    Logger_Handler_delete(&gHandler2);
    assert_null(gHandler2);

    Logger_Handler_delete(&gHandler1);
    assert_null(gHandler1);

    Logger_Formatter_delete(&gFormatter);
    assert_null(gFormatter);
}
