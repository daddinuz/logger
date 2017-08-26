/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 07, 2017 
 */

#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include "logger_formatter.h"

struct Logger_Formatter_T {
    Logger_Formatter_formatRecordCallback_T *formatRecordCallback;
    Logger_Formatter_deleteFormattedRecordCallback_T *deleteFormattedRecordCallback;
};

Logger_Formatter_T Logger_Formatter_new(
        Logger_Formatter_formatRecordCallback_T formatRecordCallback,
        Logger_Formatter_deleteFormattedRecordCallback_T deleteFormattedRecordCallback
) {
    assert(formatRecordCallback);
    assert(deleteFormattedRecordCallback);
    Logger_Formatter_T self = malloc(sizeof(*self));
    if (!self) {
        errno = ENOMEM;
        return NULL;
    }
    self->formatRecordCallback = formatRecordCallback;
    self->deleteFormattedRecordCallback = deleteFormattedRecordCallback;
    return self;
}

void Logger_Formatter_delete(Logger_Formatter_T *ref) {
    assert(ref);
    assert(*ref);
    Logger_Formatter_T self = *ref;
    free(self);
    *ref = NULL;
}

char *Logger_Formatter_formatRecord(Logger_Formatter_T self, Logger_Record_T record) {
    assert(self);
    assert(record);
    return self->formatRecordCallback(record);
}

void Logger_Formatter_deleteFormattedRecord(Logger_Formatter_T self, char *formattedRecord) {
    assert(self);
    self->deleteFormattedRecordCallback(formattedRecord);
}
