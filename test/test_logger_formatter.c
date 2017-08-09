/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 08, 2017 
 */

#include "sds/sds.h"
#include "Traits/Traits.h"
#include "logger_formatter.h"

size_t gFormatRecordCalls = 0;
size_t gDeleteFormattedRecordCalls = 0;
char *G_EXPECTED_FORMATTED_RECORD = NULL;
Logger_Record_T gRecord = NULL;

/*
 *
 */
static char *formatRecordCallback(Logger_Record_T record) {
    assert(record);
    assert_equal(gRecord, record);
    G_EXPECTED_FORMATTED_RECORD = sdsnew("EXPECTED_FORMATTED_RECORD");
    assert_not_null(G_EXPECTED_FORMATTED_RECORD);
    gFormatRecordCalls++;
    return G_EXPECTED_FORMATTED_RECORD;
}

static void deleteRecordCallback(char *formattedRecord) {
    assert(formattedRecord);
    assert_equal(G_EXPECTED_FORMATTED_RECORD, formattedRecord);
    sdsfree(formattedRecord);
    gDeleteFormattedRecordCalls++;
}

/*
 *
 */
TRAITS(CanCreateALoggerFormatter) {
    Logger_Formatter_T sut = Logger_Formatter_new(formatRecordCallback, deleteRecordCallback);
    assert_not_null(sut);

    char *formattedRecord = Logger_Formatter_formatRecord(sut, gRecord);
    assert_equal(1, gFormatRecordCalls);

    Logger_Formatter_deleteFormattedRecord(sut, formattedRecord);
    assert_equal(1, gDeleteFormattedRecordCalls);

    Logger_Formatter_delete(&sut);
    assert_null(sut);
}

/*
 *
 */
int main() {
    traits_run(CanCreateALoggerFormatter);
    return 0;
}

/*
 *
 */
void traits_setup(void) {
    gFormatRecordCalls = 0;
    gDeleteFormattedRecordCalls = 0;
    gRecord = Logger_Record_new(
            Logger_String_new("EXPECTED_MESSAGE"), "EXPECTED_LOGGER_NAME", "EXPECTED_FUNCTION", "EXPECTED_FILE",
            0, 0, LOGGER_LEVEL_NOTICE
    );
    assert_not_null(gRecord);
}

void traits_teardown(void) {
    assert_not_null(gRecord);
    assert_not_null(Logger_Record_getMessage(gRecord));
    Logger_String_T message = Logger_Record_getMessage(gRecord);
    Logger_String_delete(&message);
    assert_null(message);
    Logger_Record_delete(&gRecord);
    assert_null(gRecord);
}
