/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 07, 2017 
 */

#include <stdlib.h>
#include <assert.h>
#include "logger_handler.h"

struct Logger_Handler_T {
    void *context;
    Logger_Level_T level;
    Logger_Formatter_T formatter;
    Logger_Handler_PublishCallback_T *publishCallback;
    Logger_Handler_FlushCallback_T *flushCallback;
    Logger_Handler_CloseCallback_T *closeCallback;
};

Logger_Handler_T Logger_Handler_new(
        Logger_Handler_PublishCallback_T publishCallback,
        Logger_Handler_FlushCallback_T flushCallback,
        Logger_Handler_CloseCallback_T closeCallback
) {
    assert(publishCallback);
    assert(flushCallback);
    assert(closeCallback);
    Logger_Handler_T self = malloc(sizeof(*self));
    if (self) {
        self->context = NULL;
        self->level = LOGGER_LEVEL_DEBUG;
        self->formatter = NULL;
        self->publishCallback = publishCallback;
        self->flushCallback = flushCallback;
        self->closeCallback = closeCallback;
    }
    return self;
}

void Logger_Handler_delete(Logger_Handler_T *ref) {
    assert(ref);
    assert(*ref);
    Logger_Handler_T self = *ref;
    self->flushCallback(self);
    self->closeCallback(self);
    free(self);
    *ref = NULL;
}

Logger_Err_T Logger_Handler_publish(Logger_Handler_T self, Logger_Record_T record) {
    assert(self);
    assert(record);
    return self->publishCallback(self, record);
}

void Logger_Handler_flush(Logger_Handler_T self) {
    assert(self);
    self->flushCallback(self);
}

void Logger_Handler_close(Logger_Handler_T self) {
    assert(self);
    self->closeCallback(self);
}

bool Logger_Handler_isLoggable(Logger_Handler_T self, Logger_Record_T record) {
    assert(self);
    assert(record);
    return Logger_Record_getLevel(record) >= Logger_Handler_getLevel(self);
}

void *Logger_Handler_getContext(Logger_Handler_T self) {
    assert(self);
    return self->context;
}

Logger_Level_T Logger_Handler_getLevel(Logger_Handler_T self) {
    assert(self);
    return self->level;
}

Logger_Formatter_T Logger_Handler_getFormatter(Logger_Handler_T self) {
    assert(self);
    return self->formatter;
}

void Logger_Handler_setContext(Logger_Handler_T self, void *context) {
    assert(self);
    self->context = context;
}

void Logger_Handler_setLevel(Logger_Handler_T self, Logger_Level_T level) {
    assert(self);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    self->level = level;
}

void Logger_Handler_setFormatter(Logger_Handler_T self, Logger_Formatter_T formatter) {
    assert(self);
    assert(formatter);
    self->formatter = formatter;
}
