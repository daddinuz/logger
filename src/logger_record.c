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
#include "logger_record.h"

struct Logger_Record_T {
    Logger_String_T message;
    const char *loggerName;
    const char *function;
    const char *file;
    size_t line;
    time_t timestamp;
    Logger_Level_T level;
};

Logger_Record_T Logger_Record_new(
        const char *loggerName, Logger_Level_T level, const char *file, size_t line, const char *function,
        time_t timestamp, Logger_String_T message
) {
    assert(message);
    assert(loggerName);
    assert(function);
    assert(file);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    Logger_Record_T self = malloc(sizeof(*self));
    if (!self) {
        errno = ENOMEM;
        return NULL;
    }
    self->message = message;
    self->loggerName = loggerName;
    self->function = function;
    self->file = file;
    self->line = line;
    self->timestamp = timestamp;
    self->level = level;
    return self;
}

void Logger_Record_delete(Logger_Record_T *ref) {
    assert(ref);
    assert(*ref);
    Logger_Record_T self = *ref;
    free(self);
    *ref = NULL;
}

Logger_String_T Logger_Record_getMessage(Logger_Record_T self) {
    assert(self);
    return self->message;
}

const char *Logger_Record_getLoggerName(Logger_Record_T self) {
    assert(self);
    return self->loggerName;
}

const char *Logger_Record_getFunction(Logger_Record_T self) {
    assert(self);
    return self->function;
}

const char *Logger_Record_getFile(Logger_Record_T self) {
    assert(self);
    return self->file;
}

size_t Logger_Record_getLine(Logger_Record_T self) {
    assert(self);
    return self->line;
}

time_t Logger_Record_getTimestamp(Logger_Record_T self) {
    assert(self);
    return self->timestamp;
}

Logger_Level_T Logger_Record_getLevel(Logger_Record_T self) {
    assert(self);
    return self->level;
}

void Logger_Record_setMessage(Logger_Record_T self, Logger_String_T message) {
    assert(self);
    assert(message);
    self->message = message;
}

void Logger_Record_setLoggerName(Logger_Record_T self, const char *loggerName) {
    assert(self);
    assert(loggerName);
    self->loggerName = loggerName;
}

void Logger_Record_setFunction(Logger_Record_T self, const char *function) {
    assert(self);
    assert(function);
    self->function = function;
}

void Logger_Record_setFile(Logger_Record_T self, const char *file) {
    assert(self);
    assert(file);
    self->file = file;
}

void Logger_Record_setLine(Logger_Record_T self, size_t line) {
    assert(self);
    self->line = line;
}

void Logger_Record_setTimestamp(Logger_Record_T self, time_t timestamp) {
    assert(self);
    self->timestamp = timestamp;
}

void Logger_Record_setLevel(Logger_Record_T self, Logger_Level_T level) {
    assert(self);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    self->level = level;
}
