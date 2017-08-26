/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 08, 2017 
 */

#include "Traits/Traits.h"
#include "logger_string.h"
#include "logger_record.h"

/*
 *
 */
TRAITS(CanCreateALoggerRecord) {
    // Setup resources
    Logger_String_T EXPECTED_MESSAGE[] = {
            Logger_String_new("EXPECTED_MESSAGE"),
            Logger_String_new("NEW_EXPECTED_MESSAGE")
    };
    const char *EXPECTED_LOGGER_NAME[] = {"EXPECTED_LOGGER_NAME", "NEW_EXPECTED_LOGGER_NAME"};
    const char *EXPECTED_FUNCTION[] = {"EXPECTED_FUNCTION", "NEW_EXPECTED_FUNCTION"};
    const char *EXPECTED_FILE[] = {"EXPECTED_FILE", "NEW_EXPECTED_FILE"};
    const size_t EXPECTED_LINE[] = {__LINE__, 1 + __LINE__};
    const time_t EXPECTED_TIMESTAMP[] = {time(NULL), time(NULL)};
    const Logger_Level_T EXPECTED_LEVEL[] = {LOGGER_LEVEL_DEBUG, LOGGER_LEVEL_FATAL};

    // Test constructor
    Logger_Record_T sut = Logger_Record_new(EXPECTED_LOGGER_NAME[0], EXPECTED_LEVEL[0], EXPECTED_FILE[0], EXPECTED_LINE[0], EXPECTED_FUNCTION[0], EXPECTED_TIMESTAMP[0], EXPECTED_MESSAGE[0]);
    assert_not_null(sut);

    // Test getters
    assert_string_equal(EXPECTED_MESSAGE[0], Logger_Record_getMessage(sut));
    assert_string_equal(EXPECTED_LOGGER_NAME[0], Logger_Record_getLoggerName(sut));
    assert_string_equal(EXPECTED_FUNCTION[0], Logger_Record_getFunction(sut));
    assert_string_equal(EXPECTED_FILE[0], Logger_Record_getFile(sut));
    assert_equal(EXPECTED_LINE[0], Logger_Record_getLine(sut));
    assert_equal(EXPECTED_TIMESTAMP[0], Logger_Record_getTimestamp(sut));
    assert_equal(EXPECTED_LEVEL[0], Logger_Record_getLevel(sut));

    // Test setters
    Logger_Record_setMessage(sut, EXPECTED_MESSAGE[1]);
    assert_string_equal(EXPECTED_MESSAGE[1], Logger_Record_getMessage(sut));
    Logger_Record_setLoggerName(sut, EXPECTED_LOGGER_NAME[1]);
    assert_string_equal(EXPECTED_LOGGER_NAME[1], Logger_Record_getLoggerName(sut));
    Logger_Record_setFunction(sut, EXPECTED_FUNCTION[1]);
    assert_string_equal(EXPECTED_FUNCTION[1], Logger_Record_getFunction(sut));
    Logger_Record_setFile(sut, EXPECTED_FILE[1]);
    assert_string_equal(EXPECTED_FILE[1], Logger_Record_getFile(sut));
    Logger_Record_setLine(sut, EXPECTED_LINE[1]);
    assert_equal(EXPECTED_LINE[1], Logger_Record_getLine(sut));
    Logger_Record_setTimestamp(sut, EXPECTED_TIMESTAMP[1]);
    assert_equal(EXPECTED_TIMESTAMP[1], Logger_Record_getTimestamp(sut));
    Logger_Record_setLevel(sut, EXPECTED_LEVEL[1]);
    assert_equal(EXPECTED_LEVEL[1], Logger_Record_getLevel(sut));

    // Test destructor
    Logger_Record_delete(&sut);
    assert_null(sut);

    // Cleanup resources
    Logger_String_delete(&EXPECTED_MESSAGE[0]);
    Logger_String_delete(&EXPECTED_MESSAGE[1]);
}

/*
 *
 */
int main() {
    traits_run(CanCreateALoggerRecord);
    return 0;
}

/*
 *
 */
void traits_setup(void) {
    /* Nothing to do */
}

void traits_teardown(void) {
    /* Nothing to do */
}
