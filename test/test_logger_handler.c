/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 08, 2017 
 */

#include "sds/sds.h"
#include "Traits/Traits.h"
#include "logger_handler.h"
#include "logger_builtin_formatters.h"

size_t gPublishCalls = 0;
size_t gFlushCalls = 0;
size_t gCloseCalls = 0;
Logger_Handler_T sut = NULL;
Logger_Record_T gRecord = NULL;
Logger_Formatter_T gFormatter = NULL;

/*
 *
 */
static void publishCallback(Logger_Handler_T handler, Logger_Record_T record) {
    assert_not_null(handler);
    assert_not_null(record);
    assert_equal(sut, handler);
    assert_equal(gRecord, record);
    gPublishCalls++;
}

static void flushCallback(Logger_Handler_T handler) {
    assert_not_null(handler);
    assert_equal(sut, handler);
    gFlushCalls++;
}

static void closeCallback(Logger_Handler_T handler) {
    assert_not_null(handler);
    assert_equal(sut, handler);
    gCloseCalls++;
}

/*
 *
 */
TRAITS(CanCreateALoggerHandler) {
    sut = Logger_Handler_new(publishCallback, flushCallback, closeCallback);
    assert_not_null(sut);

    Logger_Handler_publish(sut, gRecord);
    assert_equal(1, gPublishCalls);

    Logger_Handler_flush(sut);
    assert_equal(1, gFlushCalls);

    Logger_Handler_close(sut);
    assert_equal(1, gCloseCalls);

    assert_equal(LOGGER_LEVEL_DEBUG, Logger_Handler_getLevel(sut));
    assert_equal(NULL, Logger_Handler_getFormatter(sut));
    assert_equal(NULL, Logger_Handler_getContext(sut));

    for (Logger_Level_T hLevel = LOGGER_LEVEL_DEBUG, rLevel = Logger_Record_getLevel(gRecord);
         hLevel < LOGGER_LEVEL_FATAL; hLevel++) {
        Logger_Handler_setLevel(sut, hLevel);
        assert_equal(hLevel, Logger_Handler_getLevel(sut));
        assert_equal(hLevel <= rLevel, Logger_Handler_isLoggable(sut, gRecord));
    }
    Logger_Handler_setFormatter(sut, gFormatter);
    assert_equal(gFormatter, Logger_Handler_getFormatter(sut));
    {
        void *const expected_context = sdsempty();
        Logger_Handler_setContext(sut, expected_context);
        assert_equal(expected_context, Logger_Handler_getContext(sut));
        sdsfree(expected_context);
    }

    Logger_Handler_delete(&sut);
    assert_null(sut);
}

/*
 *
 */
int main() {
    traits_run(CanCreateALoggerHandler);
    return 0;
}

/*
 *
 */
void traits_setup(void) {
    gPublishCalls = 0;
    gFlushCalls = 0;
    gCloseCalls = 0;
    sut = NULL;
    gRecord = Logger_Record_new(
            Logger_String_new("EXPECTED_MESSAGE"), "EXPECTED_LOGGER_NAME", "EXPECTED_FUNCTION", "EXPECTED_FILE",
            0, 0, LOGGER_LEVEL_INFO
    );
    assert_not_null(gRecord);
    assert_not_null(Logger_Record_getMessage(gRecord));
    gFormatter = Logger_Formatter_newSimpleFormatter();
    assert_not_null(gFormatter);
}

void traits_teardown(void) {
    assert_not_null(gRecord);
    assert_not_null(Logger_Record_getMessage(gRecord));
    Logger_String_T message = Logger_Record_getMessage(gRecord);
    Logger_String_delete(&message);
    assert_null(message);
    Logger_Record_delete(&gRecord);
    assert_null(gRecord);
    assert_not_null(gFormatter);
    Logger_Formatter_delete(&gFormatter);
    assert_null(gFormatter);
}
