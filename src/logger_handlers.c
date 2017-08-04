/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 04, 2017 
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "sds/sds.h"
#include "logger_handlers.h"

/*
 * Logger Handlers Callbacks
 */
static Logger_Buffer_T
Logger_Handler_basicPublishCallback(Logger_Handler_T handler, FILE *file, void *context, char *content) {
    (void) context;
    assert(handler);
    assert(file);
    assert(!context);
    assert(content);
    long bytes_written = fprintf(file, "%s", content);
    if (bytes_written < 0) {
        errno = EIO;
        bytes_written = 0;
    }
    if (strcmp("\0", (content + bytes_written)) != 0) {
        errno = EIO;
    }
    Logger_Handler_setBytesWritten(handler, Logger_Handler_getBytesWritten(handler) + bytes_written);
    return Logger_Buffer_new((size_t) bytes_written, content);
}

struct Logger_Handler_rotatingHandlerContext {
    FILE *file;
    Logger_String_T filePath;
    size_t BYTES_BEFORE_ROTATION;
    size_t rotationCounter;
    size_t bytesWritten;
};

static void Logger_Handler_rotatingDeleteContextCallback(void **context) {
    assert(context);
    assert(*context);
    struct Logger_Handler_rotatingHandlerContext *x = *context;
    if (x->file) {
        fclose(x->file);
    }
    free(*context);
    *context = NULL;
}

static Logger_Buffer_T
Logger_Handler_rotatingPublishCallback(Logger_Handler_T handler, FILE *file, void *context, char *content) {
    assert(handler);
    assert(file);
    assert(context);
    assert(content);
    struct Logger_Handler_rotatingHandlerContext *x = context;
    assert(x->file == file);
    if (x->bytesWritten >= x->BYTES_BEFORE_ROTATION) {
        x->rotationCounter++;
        sds realFilePath = sdscatprintf(sdsempty(), "%s.%zu", x->filePath, x->rotationCounter);
        if (!realFilePath) {
            errno = ENOMEM;
            return NULL;
        }
        x->file = freopen(realFilePath, "w", x->file);
        sdsfree(realFilePath);
        if (!x->file) {
            return NULL;
        }
        x->bytesWritten = 0;
    }
    Logger_Buffer_T buffer = Logger_Handler_basicPublishCallback(handler, file, NULL, content);
    if (!buffer) {
        errno = ENOMEM;
        return NULL;
    }
    x->bytesWritten += buffer->size;
    return buffer;
}

/*
 * Logger Handlers
 */
Logger_Handler_T Logger_Handler_newStreamHandler(
        FILE *stream, Logger_Level_T level, Logger_Formatter_T formatter
) {
    assert(stream);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(formatter);
    return Logger_Handler_new(stream, NULL, level, formatter, Logger_Handler_basicPublishCallback, NULL);
}

Logger_Handler_T Logger_Handler_newFileHandler(
        Logger_String_T filePath, Logger_Level_T level, Logger_Formatter_T formatter
) {
    // TODO make context for this function (file)
    assert(filePath);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(formatter);
    FILE *file = fopen(filePath, "w");
    return file ? Logger_Handler_new(file, NULL, level, formatter, Logger_Handler_basicPublishCallback, NULL) : NULL;
}

Logger_Handler_T Logger_Handler_newRotatingHandler(
        Logger_String_T filePath, Logger_Level_T level, Logger_Formatter_T formatter, size_t bytesBeforeRotation
) {
    assert(filePath);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(formatter);
    const size_t rotationCounter = 0;
    sds realFilePath = sdscatprintf(sdsempty(), "%s.%zu", filePath, rotationCounter);
    if (!realFilePath) {
        errno = ENOMEM;
        return NULL;
    }
    FILE *file = fopen(realFilePath, "w");
    sdsfree(realFilePath);
    if (!file) {
        return NULL;
    }
    struct Logger_Handler_rotatingHandlerContext *context = malloc(sizeof(*context));
    if (!context) {
        errno = ENOMEM;
        return NULL;
    }
    context->file = file;
    context->filePath = filePath;
    context->bytesWritten = 0;
    context->rotationCounter = rotationCounter;
    context->BYTES_BEFORE_ROTATION = bytesBeforeRotation;
    return Logger_Handler_new(
            file, context, level, formatter,
            Logger_Handler_rotatingPublishCallback, Logger_Handler_rotatingDeleteContextCallback
    );
}
