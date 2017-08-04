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
 * Buffers
 */
Logger_Buffer_T Logger_Buffer_new(size_t size, char *content) {
    assert(content);
    Logger_Buffer_T self = malloc(sizeof(*self));
    if (!self) {
        errno = ENOMEM;
        return NULL;
    }
    self->size = size;
    self->content = content;
    return self;
}

void Logger_Buffer_delete(Logger_Buffer_T *self) {
    assert(self);
    assert(*self);
    free(*self);
    *self = NULL;
}

/*
 * Levels
 */
static Logger_String_T LOGGER_LEVEL_DEBUG_STR = "DEBUG";
static Logger_String_T LOGGER_LEVEL_NOTICE_STR = "NOTICE";
static Logger_String_T LOGGER_LEVEL_INFO_STR = "INFO";
static Logger_String_T LOGGER_LEVEL_WARNING_STR = "WARNING";
static Logger_String_T LOGGER_LEVEL_ERROR_STR = "ERROR";
static Logger_String_T LOGGER_LEVEL_FATAL_STR = "FATAL";

Logger_String_T Logger_Level_getName(Logger_Level_T level) {
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    switch (level) {
        case LOGGER_LEVEL_DEBUG:
            return LOGGER_LEVEL_DEBUG_STR;
        case LOGGER_LEVEL_NOTICE:
            return LOGGER_LEVEL_NOTICE_STR;
        case LOGGER_LEVEL_INFO:
            return LOGGER_LEVEL_INFO_STR;
        case LOGGER_LEVEL_WARNING:
            return LOGGER_LEVEL_WARNING_STR;
        case LOGGER_LEVEL_ERROR:
            return LOGGER_LEVEL_ERROR_STR;
        case LOGGER_LEVEL_FATAL:
            return LOGGER_LEVEL_FATAL_STR;
        default:
            abort();
    };
}

/*
 * Messages
 */
struct Logger_Message_T {
    Logger_String_T loggerName;
    Logger_Level_T level;
    Logger_String_T file;
    size_t line;
    Logger_String_T function;
    time_t timestamp;
    sds content;
};

Logger_Message_T Logger_Message_newFromArgumentsPack(
        Logger_String_T logger_name, Logger_Level_T level, Logger_String_T file, size_t line,
        Logger_String_T function, time_t timestamp, const char *fmt, va_list args
) {
    assert(logger_name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(file);
    assert(function);
    assert(fmt);
    assert(args);
    Logger_Message_T self = malloc(sizeof(*self));
    if (!self) {
        errno = ENOMEM;
        return NULL;
    }
    sds dummy = sdsempty();
    if (!dummy) {
        free(self);
        errno = ENOMEM;
        return NULL;
    }
    self->content = sdscatvprintf(dummy, fmt, args);
    if (!self->content) {
        sdsfree(dummy);
        free(self);
        errno = ENOMEM;
        return NULL;
    }
    self->loggerName = logger_name;
    self->level = level;
    self->file = file;
    self->line = line;
    self->function = function;
    self->timestamp = timestamp;
    return self;
}

Logger_Message_T Logger_Message_newFromVariadicArguments(
        Logger_String_T logger_name, Logger_Level_T level, Logger_String_T file, size_t line,
        Logger_String_T function, time_t timestamp, const char *fmt, ...
) {
    assert(logger_name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(file);
    assert(function);
    assert(fmt);
    va_list args;
    va_start(args, fmt);
    Logger_Message_T self = Logger_Message_newFromArgumentsPack(logger_name, level, file, line, function, timestamp, fmt, args);
    va_end(args);
    if (!self) {
        errno = ENOMEM;
        return NULL;
    }
    return self;
}

Logger_String_T Logger_Message_getLoggerName(Logger_Message_T self) {
    assert(self);
    return self->loggerName;
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

Logger_String_T Logger_Message_getContent(Logger_Message_T self) {
    assert(self);
    return self->content;
}

void Logger_Message_delete(Logger_Message_T *self) {
    assert(self);
    assert(*self);
    sdsfree((*self)->content);
    free(*self);
    *self = NULL;
}

/*
 * Formatters
 */
struct Logger_Formatter_T {
    Logger_Formatter_formatContentCallback_T formatMessageCallback;
    Logger_Formatter_deleteContentCallback_T deleteMessageCallback;
};

Logger_Formatter_T Logger_Formatter_new(
        Logger_Formatter_formatContentCallback_T formatContentCallback,
        Logger_Formatter_deleteContentCallback_T deleteContentCallback
) {
    assert(formatContentCallback);
    assert(deleteContentCallback);
    Logger_Formatter_T self = malloc(sizeof(*self));
    if (!self) {
        errno = ENOMEM;
        return NULL;
    }
    self->formatMessageCallback = formatContentCallback;
    self->deleteMessageCallback = deleteContentCallback;
    return self;
}

Logger_Formatter_formatContentCallback_T Logger_Formatter_getFormatContentCallback(Logger_Formatter_T self) {
    assert(self);
    return self->formatMessageCallback;
}

Logger_Formatter_deleteContentCallback_T Logger_Formatter_getDeleteContentCallback(Logger_Formatter_T self) {
    assert(self);
    return self->deleteMessageCallback;
}

char *Logger_Formatter_formatMessage(Logger_Formatter_T self, Logger_Message_T message) {
    assert(self);
    assert(message);
    return self->formatMessageCallback(message);
}

void Logger_Formatter_deleteMessage(Logger_Formatter_T self, char **content) {
    assert(self);
    assert(content);
    assert(*content);
    self->deleteMessageCallback(content);
    *content = NULL;
}

void Logger_Formatter_delete(Logger_Formatter_T *self) {
    assert(self);
    assert(*self);
    free(*self);
    *self = NULL;
}

/*
 * Handlers
 */
struct Logger_Handler_T {
    FILE *file;
    void *context;
    Logger_Formatter_T formatter;
    Logger_Handler_publishCallback_T publishCallback;
    Logger_Handler_deleteContextCallback_T deleteContextCallback;
    Logger_Level_T level;
    size_t bytesWritten;
};

Logger_Handler_T Logger_Handler_new(
        FILE *file, void *context, Logger_Level_T level, Logger_Formatter_T formatter,
        Logger_Handler_publishCallback_T publishCallback, Logger_Handler_deleteContextCallback_T deleteContextCallback
) {
    assert(formatter);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(file);
    assert(publishCallback);
    Logger_Handler_T self = malloc(sizeof(*self));
    if (!self) {
        errno = ENOMEM;
        return NULL;
    }
    self->file = file;
    self->level = level;
    self->context = context;
    self->bytesWritten = 0;
    self->formatter = formatter;
    self->publishCallback = publishCallback;
    self->deleteContextCallback = deleteContextCallback;
    return self;
}

void Logger_Handler_flush(Logger_Handler_T self) {
    assert(self);
    fflush(self->file);
}

Logger_Buffer_T Logger_Handler_publish(Logger_Handler_T self, Logger_Message_T message) {
    assert(self);
    assert(message);
    char *content = Logger_Formatter_formatMessage(self->formatter, message);
    if (!content) {
        errno = ENOMEM;
        return NULL;
    }
    return self->publishCallback(self, self->file, self->context, content);
}

void Logger_Handler_setLevel(Logger_Handler_T self, Logger_Level_T level) {
    assert(self);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    self->level = level;
}

Logger_Level_T Logger_Handler_getLevel(Logger_Handler_T self) {
    assert(self);
    return self->level;
}

void Logger_Handler_setBytesWritten(Logger_Handler_T self, size_t bytes_written) {
    assert(self);
    self->bytesWritten = bytes_written;
}

size_t Logger_Handler_getBytesWritten(Logger_Handler_T self) {
    assert(self);
    return self->bytesWritten;
}

void Logger_Handler_delete(Logger_Handler_T *self) {
    assert(self);
    assert(*self);
    if ((*self)->context && (*self)->deleteContextCallback) {
        (*self)->deleteContextCallback(&(*self)->context);
    }
    free(*self);
    *self = NULL;
}

/*
 * Loggers
 */
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

static void Logger_Handler_List_delete(Logger_Handler_List_T *self) {
    assert(self);
    assert(*self);
    free(*self);
    *self = NULL;
}

struct Logger_T {
    sds name;
    Logger_Level_T level;
    Logger_Handler_List_T handlers;
};

Logger_T Logger_new(Logger_String_T name, Logger_Level_T level) {
    assert(name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    Logger_T self = malloc(sizeof(*self));
    if (!self) {
        errno = ENOMEM;
        return NULL;
    }
    self->name = sdsnew(name);
    if (!self->name) {
        free(self);
        errno = ENOMEM;
        return NULL;
    }
    self->level = level;
    self->handlers = NULL;
    return self;
}

Logger_String_T Logger_setName(Logger_T self, Logger_String_T name) {
    assert(self);
    assert(name);
    sds new_name = sdsnew(name);
    if (!new_name) {
        errno = ENOMEM;
        return NULL;
    }
    sdsfree(self->name);
    self->name = new_name;
    return self->name;
}

Logger_String_T Logger_getName(Logger_T self) {
    assert(self);
    return self->name;
}

void Logger_setLevel(Logger_T self, Logger_Level_T level) {
    assert(self);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    self->level = level;
}

Logger_Level_T Logger_getLevel(Logger_T self) {
    assert(self);
    return self->level;
}

Logger_Handler_T Logger_addHandler(Logger_T self, Logger_Handler_T handler) {
    assert(self);
    assert(handler);
    Logger_Handler_List_T head = Logger_Handler_List_new(handler, self->handlers);
    if (!head) {
        errno = ENOMEM;
        return NULL;
    }
    self->handlers = head;
    return head->handler;
}

void Logger_removeHandler(Logger_T self, Logger_Handler_T handler) {
    assert(self);
    assert(handler);
    Logger_Handler_List_T prev = NULL, base = self->handlers;
    while (base) {
        if (base->handler == handler) {
            if (prev) {
                prev->next = base->next;
            } else {
                self->handlers = base->next;
            }
            Logger_Handler_List_delete(&base);
            break;
        }
        prev = base;
        base = base->next;
    }
}

void _Logger_log(
        Logger_T self, Logger_Level_T level, Logger_String_T file, size_t line, Logger_String_T function,
        time_t timestamp, const char *fmt, ...
) {
    assert(self);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(file);
    assert(function);
    assert(fmt);
    if (level >= self->level) {
        va_list args;
        va_start(args, fmt);
        Logger_Message_T message = Logger_Message_newFromArgumentsPack(self->name, level, file, line, function, timestamp, fmt, args);
        Logger_Buffer_T buffer = NULL;
        Logger_Handler_T handler = NULL;
        Logger_Handler_List_T base = NULL;
        for (base = self->handlers; base; base = base->next) {
            handler = base->handler;
            if (level >= handler->level) {
                buffer = Logger_Handler_publish(handler, message);
                Logger_Formatter_deleteMessage(handler->formatter, &buffer->content);
                Logger_Buffer_delete(&buffer);
            }
        }
        Logger_Message_delete(&message);
        va_end(args);
    }
}

void Logger_delete(Logger_T *self) {
    assert(self);
    assert(*self);
    sdsfree((*self)->name);
    Logger_Handler_List_T current, next;
    for (current = (*self)->handlers; current; current = next) {
        next = current->next;
        Logger_Handler_List_delete(&current);
    }
    free(*self);
    *self = NULL;
}
