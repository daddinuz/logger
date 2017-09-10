/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 08, 2017 
 */

#include "traits/traits.h"
#include "traits-unit/traits-unit.h"
#include "sds/sds.h"
#include "logger_formatter.h"

/*
 * Define globals
 */
size_t gFormatRecordCalls = 0;
size_t gDeleteFormattedRecordCalls = 0;
char *G_EXPECTED_FORMATTED_RECORD = NULL;
Logger_Record_T gRecord = NULL;

/*
 * Declare callbacks
 */
static char *formatRecordCallback(Logger_Record_T record);
static void deleteRecordCallback(char *formattedRecord);

/*
 * Declare setups
 */
SetupDeclare(SetupLoggerFormatter);

/*
 * Declare teardowns
 */
TeardownDeclare(TeardownLoggerFormatter);

/*
 * Declare fixtures
 */
FixtureDeclare(FixtureLoggerFormatter);

/*
 * Declare features
 */
FeatureDeclare(NewAndDelete);
FeatureDeclare(FormatRecordAndDelete);

/*
 * Describe the test case
 */
Describe("LoggerFormatter",
         Trait(
                 "Basic",
                 Run(FormatRecordAndDelete, FixtureLoggerFormatter)
         )
)

/*
 * Define callbacks
 */
char *formatRecordCallback(Logger_Record_T record) {
    assert_not_null(record);
    assert_equal(gRecord, record);
    G_EXPECTED_FORMATTED_RECORD = sdsnew("EXPECTED_FORMATTED_RECORD");
    assert_not_null(G_EXPECTED_FORMATTED_RECORD);
    gFormatRecordCalls++;
    return G_EXPECTED_FORMATTED_RECORD;
}

void deleteRecordCallback(char *formattedRecord) {
    assert_not_null(formattedRecord);
    assert_equal(G_EXPECTED_FORMATTED_RECORD, formattedRecord);
    sdsfree(formattedRecord);
    gDeleteFormattedRecordCalls++;
}

/*
 * Define setups
 */
SetupDefine(SetupLoggerFormatter) {
    gFormatRecordCalls = 0;
    gDeleteFormattedRecordCalls = 0;
    gRecord = Logger_Record_new("EXPECTED_LOGGER_NAME", LOGGER_LEVEL_NOTICE, "EXPECTED_FILE", 0, "EXPECTED_FUNCTION", 0, Logger_String_new("EXPECTED_MESSAGE"));
    assert_not_null(gRecord);
    Logger_Formatter_T sut = Logger_Formatter_new(formatRecordCallback, deleteRecordCallback);
    assert_not_null(sut);
    return sut;
}

/*
 * Define teardowns
 */
TeardownDefine(TeardownLoggerFormatter) {
    assert_not_null(traits_context);
    assert_not_null(gRecord);
    assert_not_null(Logger_Record_getMessage(gRecord));
    Logger_Formatter_T sut = traits_context;
    Logger_String_T message = Logger_Record_getMessage(gRecord);
    Logger_String_delete(&message);
    assert_null(message);
    Logger_Record_delete(&gRecord);
    assert_null(gRecord);
    Logger_Formatter_delete(&sut);
    assert_null(sut);
}

/*
 * Define features
 */
FixtureDefine(FixtureLoggerFormatter, SetupLoggerFormatter, TeardownLoggerFormatter);

/*
 * Define features
 */
FeatureDefine(NewAndDelete) {
    (void) traits_context;
    /* Nothing to do */
}

FeatureDefine(FormatRecordAndDelete) {
    Logger_Formatter_T sut = traits_context;

    char *formattedRecord = Logger_Formatter_formatRecord(sut, gRecord);
    assert_equal(1, gFormatRecordCalls);

    Logger_Formatter_deleteFormattedRecord(sut, formattedRecord);
    assert_equal(1, gDeleteFormattedRecordCalls);
}
