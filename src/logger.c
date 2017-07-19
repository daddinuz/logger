/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   July 19, 2017 
 */

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "sds/sds.h"
#include "logger.h"

/*
 * Messages
 */
struct Logger_Message_T {
    Logger_String_T logger_name;
    Logger_Level_T level;
    Logger_String_T file;
    size_t line;
    Logger_String_T function;
    time_t timestamp;
    sds message;
};

Logger_Message_T Logger_Message_new(Logger_String_T logger_name, Logger_Level_T level, Logger_String_T file, size_t line, Logger_String_T function, time_t timestamp, const char *fmt, ...) {
    assert(logger_name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(file);
    assert(function);
    Logger_Message_T self = malloc(sizeof(*self));
    if (!self) {
        errno = ENOMEM;
        return NULL;
    }
    va_list args;
    va_start(args, fmt);
    self->message = sdscatvprintf(sdsempty(), fmt, args);  // TODO this need to be freed
    va_end(args);
    if (!self->message) {
        free(self);
        errno = ENOMEM;
        return NULL;
    }
    self->logger_name = logger_name;
    self->level = level;
    self->file = file;
    self->line = line;
    self->function = function;
    self->timestamp = timestamp;
    return self;
}

Logger_String_T Logger_Message_getLoggerName(Logger_Message_T self) {
    assert(self);
    return self->logger_name;
}

Logger_Level_T Logger_Message_getLevel(Logger_Message_T self) {
    assert(self);
    return self->level;
}

Logger_String_T Logger_Message_getFile(Logger_Message_T self) {
    assert(self);
    return self->file;
}

size_t Logger_Message_getLine(Logger_Message_T self) {
    assert(self);
    return self->line;
}

Logger_String_T Logger_Message_getFunction(Logger_Message_T self) {
    assert(self);
    return self->function;
}

time_t Logger_Message_getTimestamp(Logger_Message_T self) {
    assert(self);
    return self->timestamp;
}

Logger_String_T Logger_Message_getMessage(Logger_Message_T self) {
    assert(self);
    return self->message;
}

void Logger_Message_delete(Logger_Message_T *self) {
    assert(self);
    assert(*self);
    sdsfree((*self)->message);
    free(*self);
    *self = NULL;
}
