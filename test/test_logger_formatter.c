/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 08, 2017 
 */

#include <errno.h>
#include "sds/sds.h"
#include "Traits/Traits.h"
#include "logger_formatter.h"

char *RECORD_MESSAGE = NULL;
const char *RECORD_LOGGER_NAME = "EXPECTED_LOGGER_NAME";
const char *RECORD_FUNCTION = "EXPECTED_FUNCTION";
const char *RECORD_FILE = "EXPECTED_FILE";
const size_t RECORD_LINE = __LINE__;
const time_t RECORD_TIMESTAMP = 0;
const Logger_Level_T RECORD_LEVEL = LOGGER_LEVEL_DEBUG;
Logger_Record_T record = NULL;

/*
 *
 */
static char *formatRecordCallback(Logger_Record_T record) {
    assert(record);
    char time_string[32] = "";
    time_t timestamp = Logger_Record_getTimestamp(record);
    strftime(time_string, sizeof(time_string) / sizeof(time_string[0]), "%Y-%m-%d %H:%M:%S UTC", gmtime(&timestamp));
    sds result = sdscatprintf(
            sdsempty(),
            "%s [%s] %s %s:%zu:%s\n%s",
            Logger_Record_getLoggerName(record),
            Logger_Level_getName(Logger_Record_getLevel(record)),
            time_string,
            Logger_Record_getFile(record),
            Logger_Record_getLine(record),
            Logger_Record_getFunction(record),
            Logger_Record_getMessage(record)
    );
    if (!result) {
        errno = ENOMEM;
    }
    return result;
}

static void deleteRecordCallback(char *formattedRecord) {
    sdsfree(formattedRecord);
}


/*
 *
 */
TRAITS(CanCreateALoggerFormatter) {
    Logger_Formatter_T sut = Logger_Formatter_new(formatRecordCallback, deleteRecordCallback);
    assert_not_null(sut);

    char *formattedRecord = Logger_Formatter_formatRecord(sut, record);
    assert_string_equal(
            "EXPECTED_LOGGER_NAME [DEBUG] 1970-01-01 00:00:00 UTC EXPECTED_FILE:18:EXPECTED_FUNCTION\nEXPECTED_MESSAGE",
            formattedRecord
    );

    Logger_Formatter_deleteFormattedRecord(sut, formattedRecord);
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
    RECORD_MESSAGE = strdup("EXPECTED_MESSAGE");
    record = Logger_Record_new(
            RECORD_MESSAGE, RECORD_LOGGER_NAME, RECORD_FUNCTION,
            RECORD_FILE, RECORD_LINE, RECORD_TIMESTAMP, RECORD_LEVEL
    );
}

void traits_teardown(void) {
    free(RECORD_MESSAGE);
    Logger_Record_delete(&record);
}
