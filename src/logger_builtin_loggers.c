/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 15, 2017 
 */

#include <assert.h>
#include "logger_builtin_formatters.h"
#include "logger_builtin_handlers.h"
#include "logger_builtin_loggers.h"

static Logger_T newConsoleLogger(const char *name, Logger_Level_T level, Logger_ConsoleStream_T stream) {
    assert(name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(LOGGER_CONSOLE_STREAM_STDERR == stream || LOGGER_CONSOLE_STREAM_STDOUT == stream);

    Logger_Formatter_T formatter = NULL;
    Logger_Handler_T handler = NULL;
    Logger_T self = NULL;

    for (;;) {
        formatter = Logger_Formatter_newSimpleFormatter();
        if (!formatter) {
            break;
        }

        handler = Logger_Handler_newConsoleHandler(stream, level, formatter);
        if (!handler) {
            break;
        }

        self = Logger_new(name, level);
        if (!self) {
            break;
        }

        Logger_addHandler(self, handler);
        return self;
    }

    if (formatter) {
        Logger_Formatter_delete(&formatter);
    }
    if (handler) {
        Logger_Handler_delete(&handler);
    }
    return NULL;
}

Logger_T Logger_newStdoutLogger(const char *name, Logger_Level_T level) {
    assert(name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    return newConsoleLogger(name, level, LOGGER_CONSOLE_STREAM_STDOUT);
}

Logger_T Logger_newStderrLogger(const char *name, Logger_Level_T level) {
    assert(name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    return newConsoleLogger(name, level, LOGGER_CONSOLE_STREAM_STDERR);
}

Logger_T Logger_newFileLogger(const char *name, Logger_Level_T level, const char *filePath) {
    assert(name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(filePath);

    Logger_Formatter_T formatter = NULL;
    Logger_Handler_T handler = NULL;
    Logger_T self = NULL;

    for (;;) {
        formatter = Logger_Formatter_newSimpleFormatter();
        if (!formatter) {
            break;
        }

        handler = Logger_Handler_newFileHandler(filePath, level, formatter);
        if (!handler) {
            break;
        }

        self = Logger_new(name, level);
        if (!self) {
            break;
        }

        Logger_addHandler(self, handler);
        return self;
    }

    if (formatter) {
        Logger_Formatter_delete(&formatter);
    }
    if (handler) {
        Logger_Handler_delete(&handler);
    }
    return NULL;
}

Logger_T
Logger_newRotatingFileLogger(const char *name, Logger_Level_T level, const char *filePath, size_t bytesBeforeRotation) {
    assert(name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(filePath);

    Logger_Formatter_T formatter = NULL;
    Logger_Handler_T handler = NULL;
    Logger_T self = NULL;

    for (;;) {
        formatter = Logger_Formatter_newSimpleFormatter();
        if (!formatter) {
            break;
        }

        handler = Logger_Handler_newRotatingFileHandler(filePath, level, formatter, bytesBeforeRotation);
        if (!handler) {
            break;
        }

        self = Logger_new(name, level);
        if (!self) {
            break;
        }

        Logger_addHandler(self, handler);
        return self;
    }

    if (formatter) {
        Logger_Formatter_delete(&formatter);
    }
    if (handler) {
        Logger_Handler_delete(&handler);
    }
    return NULL;
}

Logger_T
Logger_newMemoryFileLogger(const char *name, Logger_Level_T level, const char *filePath, size_t bytesBeforeWrite) {
    assert(name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(filePath);

    Logger_Formatter_T formatter = NULL;
    Logger_Handler_T handler = NULL;
    Logger_T self = NULL;

    for (;;) {
        formatter = Logger_Formatter_newSimpleFormatter();
        if (!formatter) {
            break;
        }

        handler = Logger_Handler_newMemoryFileHandler(filePath, level, formatter, bytesBeforeWrite);
        if (!handler) {
            break;
        }

        self = Logger_new(name, level);
        if (!self) {
            break;
        }

        Logger_addHandler(self, handler);
        return self;
    }

    if (formatter) {
        Logger_Formatter_delete(&formatter);
    }
    if (handler) {
        Logger_Handler_delete(&handler);
    }
    return NULL;
}
