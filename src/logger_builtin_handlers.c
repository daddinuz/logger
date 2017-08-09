/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 04, 2017
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "sds/sds.h"
#include "logger_builtin_handlers.h"

static void invalidate_ptr(void **ref, void (*destructor)()) {
    assert(ref);
    assert(*ref);
    assert(destructor);
    destructor(*ref);
    *ref = NULL;
}

/*
 * Console Handler
 */
static void consoleHandlerPublishCallback(Logger_Handler_T handler, Logger_Record_T record) {
    assert(handler);
    assert(record);
    FILE *file = Logger_Handler_getContext(handler);
    Logger_Formatter_T formatter = Logger_Handler_getFormatter(handler);
    char *log = Logger_Formatter_formatRecord(formatter, record);
    if (!log) {
        // TODO: handle OOM Errors
        return;
    }
    const int bytesWritten = fprintf(file, "%s", log);
    if (bytesWritten <= 0) {
        // TODO: handle IO Errors
    }
    Logger_Formatter_deleteFormattedRecord(formatter, log);
}

static void consoleHandlerFlushCallback(Logger_Handler_T handler) {
    assert(handler);
    fflush(Logger_Handler_getContext(handler));
}

static void consoleHandlerCloseCallback(Logger_Handler_T handler) {
    assert(handler);
    (void) handler;
}

Logger_Handler_T Logger_Handler_newConsoleHandler(
        Logger_ConsoleStream_T stream, Logger_Level_T level, Logger_Formatter_T formatter
) {
    assert(LOGGER_CONSOLE_STREAM_STDERR == stream || LOGGER_CONSOLE_STREAM_STDOUT == stream);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(formatter);
    Logger_Handler_T self = Logger_Handler_new(consoleHandlerPublishCallback, consoleHandlerFlushCallback, consoleHandlerCloseCallback);
    if (!self) {
        return NULL;
    }
    Logger_Handler_setContext(self, LOGGER_CONSOLE_STREAM_STDOUT == stream ? stdout : stderr);
    Logger_Handler_setLevel(self, level);
    Logger_Handler_setFormatter(self, formatter);
    return self;
}

/*
 * File Handler
 */
static void fileHandlerPublishCallback(Logger_Handler_T handler, Logger_Record_T record) {
    assert(handler);
    assert(record);
    FILE *file = Logger_Handler_getContext(handler);
    Logger_Formatter_T formatter = Logger_Handler_getFormatter(handler);
    char *log = Logger_Formatter_formatRecord(formatter, record);
    if (!log) {
        // TODO: handle OOM Errors
        return;
    }
    const int bytesWritten = fprintf(file, "%s", log);
    if (bytesWritten <= 0) {
        // TODO: handle IO Errors
    }
    Logger_Formatter_deleteFormattedRecord(formatter, log);
}

static void fileHandlerFlushCallback(Logger_Handler_T handler) {
    assert(handler);
    fflush(Logger_Handler_getContext(handler));
}

static void fileHandlerCloseCallback(Logger_Handler_T handler) {
    assert(handler);
    fclose(Logger_Handler_getContext(handler));
}

Logger_Handler_T Logger_Handler_newFileHandler(
        const char *filePath, Logger_Level_T level, Logger_Formatter_T formatter
) {
    assert(filePath);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(formatter);
    FILE *file = fopen(filePath, "w");
    if (!file) {
        return NULL;
    }
    Logger_Handler_T self = Logger_Handler_new(fileHandlerPublishCallback, fileHandlerFlushCallback, fileHandlerCloseCallback);
    if (!self) {
        fclose(file);
        return NULL;
    }
    Logger_Handler_setContext(self, file);
    Logger_Handler_setLevel(self, level);
    Logger_Handler_setFormatter(self, formatter);
    return self;
}

/*
 * Rotating File Handler
 */
typedef struct rotatingHandlerContext {
    size_t BYTES_BEFORE_ROTATION;
    size_t bytesWritten;
    size_t rotationCounter;
    const char *filePath;
    FILE *file;
} *rotatingHandlerContext;

static void rotatingFileHandlerPublishCallback(Logger_Handler_T handler, Logger_Record_T record) {
    assert(handler);
    assert(record);
    char *log = NULL;
    sds realFilePath = NULL;
    rotatingHandlerContext context = Logger_Handler_getContext(handler);
    Logger_Formatter_T formatter = Logger_Handler_getFormatter(handler);

    do {
        log = Logger_Formatter_formatRecord(formatter, record);
        if (!log) {
            break;
        }
        if (context->bytesWritten >= context->BYTES_BEFORE_ROTATION) {
            context->rotationCounter++;
            realFilePath = sdscatprintf(sdsempty(), "%s.%zu", context->filePath, context->rotationCounter);
            if (!realFilePath) {
                // TODO: handle OOM Errors
                break;
            }
            freopen(realFilePath, "w", context->file);
            invalidate_ptr((void **) &realFilePath, sdsfree);
            if (!context->file) {
                // TODO: handle IO Errors
                break;
            }
            context->bytesWritten = 0;
        }
        const int bytesWritten = fprintf(context->file, "%s", log);
        if (bytesWritten <= 0) {
            // TODO: handle IO Errors
            break;
        } else {
            context->bytesWritten += bytesWritten;
            break;
        }
    } while (false);

    Logger_Formatter_deleteFormattedRecord(formatter, log);
    sdsfree(realFilePath);
}

static void rotatingFileHandlerFlushCallback(Logger_Handler_T handler) {
    assert(handler);
    rotatingHandlerContext context = Logger_Handler_getContext(handler);
    fflush(context->file);
}

static void rotatingFileHandlerCloseCallback(Logger_Handler_T handler) {
    assert(handler);
    rotatingHandlerContext context = Logger_Handler_getContext(handler);
    fclose(context->file);
}

Logger_Handler_T Logger_Handler_newRotatingHandler(
        const char *filePath, Logger_Level_T level, Logger_Formatter_T formatter, size_t bytesBeforeRotation
) {
    assert(filePath);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(formatter);
    const size_t ROTATION_COUNTER = 0;
    sds realFilePath = NULL;
    FILE *file = NULL;
    rotatingHandlerContext context = NULL;
    Logger_Handler_T self = NULL;

    for (;;) {
        realFilePath = sdscatprintf(sdsempty(), "%s.%zu", filePath, ROTATION_COUNTER);
        if (!realFilePath) {
            break;
        }
        file = fopen(realFilePath, "w");
        invalidate_ptr((void **) &realFilePath, sdsfree);
        if (!file) {
            break;
        }
        context = malloc(sizeof(*context));
        if (!context) {
            break;
        }
        context->file = file;
        context->filePath = filePath;
        context->bytesWritten = 0;
        context->rotationCounter = ROTATION_COUNTER;
        context->BYTES_BEFORE_ROTATION = bytesBeforeRotation;
        self = Logger_Handler_new(rotatingFileHandlerPublishCallback, rotatingFileHandlerFlushCallback, rotatingFileHandlerCloseCallback);
        if (!self) {
            break;
        }
        Logger_Handler_setLevel(self, level);
        Logger_Handler_setContext(self, context);
        Logger_Handler_setFormatter(self, formatter);
        return self;
    };

    // TODO: handle OOM Errors
    sdsfree(realFilePath);
    if (file) {
        fclose(file);
    }
    free(context);
    Logger_Handler_delete(&self);
    return NULL;
}
