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
#include "logger.h"

typedef struct Logger_Handler_List_T {
    Logger_Handler_T handler;
    struct Logger_Handler_List_T *next;
} *Logger_Handler_List_T;

static Logger_Handler_List_T Logger_Handler_List_new(Logger_Handler_T handler, Logger_Handler_List_T next) {
    assert(handler);
    Logger_Handler_List_T self = malloc(sizeof(*self));
    if (!self) {
        errno = ENOMEM;
        return NULL;
    }
    self->handler = handler;
    self->next = next;
    return self;
}

static void Logger_Handler_List_delete(Logger_Handler_List_T *ref) {
    assert(ref);
    assert(*ref);
    Logger_Handler_List_T self = *ref;
    free(self);
    *ref = NULL;
}

struct Logger_T {
    const char *name;
    Logger_Level_T level;
    Logger_Handler_List_T handlers;
};

Logger_T Logger_new(const char *name, Logger_Level_T level) {
    assert(name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    Logger_T self = malloc(sizeof(*self));
    if (!self) {
        errno = ENOMEM;
        return NULL;
    }
    self->name = name;
    self->level = level;
    self->handlers = NULL;
    return self;
}

void Logger_delete(Logger_T *ref) {
    assert(ref);
    assert(*ref);
    Logger_T self = *ref;
    Logger_Handler_List_T current, next;
    for (current = self->handlers; current; current = next) {
        next = current->next;
        Logger_Handler_List_delete(&current);
    }
    free(self);
    *ref = NULL;
}

const char *Logger_getName(Logger_T self) {
    assert(self);
    return self->name;
}

Logger_Level_T Logger_getLevel(Logger_T self) {
    assert(self);
    return self->level;
}

Logger_Handler_T Logger_removeHandler(Logger_T self, Logger_Handler_T handler) {
    assert(self);
    assert(handler);
    Logger_Handler_T outHandler = NULL;
    Logger_Handler_List_T prev = NULL, base = self->handlers;
    while (base) {
        if (base->handler == handler) {
            if (prev) {
                prev->next = base->next;
            } else {
                self->handlers = base->next;
            }
            outHandler = base->handler;
            Logger_Handler_List_delete(&base);
            break;
        }
        prev = base;
        base = base->next;
    }
    return outHandler;
}


void Logger_setName(Logger_T self, const char *name) {
    assert(self);
    assert(name);
    self->name = name;
}

void Logger_setLevel(Logger_T self, Logger_Level_T level) {
    assert(self);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    self->level = level;
}

void Logger_addHandler(Logger_T self, Logger_Handler_T handler) {
    assert(self);
    assert(handler);
    Logger_Handler_List_T head = Logger_Handler_List_new(handler, self->handlers);
    if (!head) {
        errno = ENOMEM;
        return;
    }
    self->handlers = head;
}

void _Logger_log(
        Logger_T self, Logger_Level_T level, time_t timestamp, size_t line,
        const char *file, const char *function, const char *fmt, ...
) {
    assert(self);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(file);
    assert(function);
    assert(fmt);
    if (level >= Logger_getLevel(self)) {
        va_list args;
        va_start(args, fmt);
        Logger_Handler_T handler = NULL;
        Logger_Handler_List_T base = NULL;
        // TODO make message
        Logger_Record_T record = Logger_Record_new(NULL, self->name, function, file, line, timestamp, level);
        for (base = self->handlers; base; base = base->next) {
            handler = base->handler;
            if (Logger_Handler_isLoggable(handler, record)) {
                Logger_Handler_publish(handler, record);
            }
        }
        Logger_Record_delete(&record);
        va_end(args);
    }
}
