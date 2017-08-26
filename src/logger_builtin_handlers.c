/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 04, 2017
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sds/sds.h"
#include "logger_err.h"
#include "logger_stream.h"
#include "logger_builtin_handlers.h"

static void invalidate_ptr(void **ref, void destructor()) {
    assert(ref);
    assert(*ref);
    assert(destructor);
    destructor(*ref);
    *ref = NULL;
}

/*
 * Console Handler
 */
static Logger_Err_T consoleHandlerPublishCallback(Logger_Handler_T handler, Logger_Record_T record) {
    assert(handler);
    assert(record);

    char *log = NULL;
    Logger_Err_T err = LOGGER_ERR_OK;
    FILE *file = Logger_Handler_getContext(handler);
    Logger_Formatter_T formatter = Logger_Handler_getFormatter(handler);

    do {
        log = Logger_Formatter_formatRecord(formatter, record);
        if (!log) {
            err = LOGGER_ERR_OUT_OF_MEMORY;
            break;
        }

        if (fprintf(file, "%s", log) < 0) {
            err = LOGGER_ERR_IO;
            break;
        }
    } while (false);

    Logger_Formatter_deleteFormattedRecord(formatter, log);
    Logger_Handler_flush(handler);
    return err;
}

static void consoleHandlerFlushCallback(Logger_Handler_T handler) {
    assert(handler);
    fflush(Logger_Handler_getContext(handler));
}

static void consoleHandlerCloseCallback(Logger_Handler_T handler) {
    assert(handler);
    (void) handler;
}

Logger_Handler_Result_T Logger_Handler_newConsoleHandler(
        Logger_Level_T level, Logger_Formatter_T formatter, Logger_OStream_T stream
) {
    assert(LOGGER_OSTREAM_STDERR == stream || LOGGER_OSTREAM_STDOUT == stream);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(formatter);
    Logger_Err_T err = LOGGER_ERR_OK;
    Logger_Handler_T self = NULL;

    do {
        self = Logger_Handler_new(consoleHandlerPublishCallback, consoleHandlerFlushCallback, consoleHandlerCloseCallback);
        if (!self) {
            err = LOGGER_ERR_OUT_OF_MEMORY;
            break;
        }

        Logger_Handler_setContext(self, LOGGER_OSTREAM_STDOUT == stream ? stdout : stderr);
        Logger_Handler_setLevel(self, level);
        Logger_Handler_setFormatter(self, formatter);
    } while (false);

    return (Logger_Handler_Result_T) {.err=err, .handler=self};
}

/*
 * File Handler
 */
static Logger_Err_T fileHandlerPublishCallback(Logger_Handler_T handler, Logger_Record_T record) {
    assert(handler);
    assert(record);

    char *log = NULL;
    Logger_Err_T err = LOGGER_ERR_OK;
    FILE *file = Logger_Handler_getContext(handler);
    Logger_Formatter_T formatter = Logger_Handler_getFormatter(handler);

    do {
        log = Logger_Formatter_formatRecord(formatter, record);
        if (!log) {
            err = LOGGER_ERR_OUT_OF_MEMORY;
            break;
        }

        if (fprintf(file, "%s", log) < 0) {
            err = LOGGER_ERR_IO;
            break;
        }
    } while (false);

    Logger_Formatter_deleteFormattedRecord(formatter, log);
    Logger_Handler_flush(handler);
    return err;
}

static void fileHandlerFlushCallback(Logger_Handler_T handler) {
    assert(handler);
    fflush(Logger_Handler_getContext(handler));
}

static void fileHandlerCloseCallback(Logger_Handler_T handler) {
    assert(handler);
    fclose(Logger_Handler_getContext(handler));
}

Logger_Handler_Result_T Logger_Handler_newFileHandler(
        Logger_Level_T level, Logger_Formatter_T formatter, const char *filePath
) {
    assert(filePath);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(formatter);
    Logger_Err_T err = LOGGER_ERR_OK;
    Logger_Handler_T self = NULL;

    do {
        FILE *file = fopen(filePath, "w");
        if (!file) {
            err = Logger_Err_fromErrno(errno);
            break;
        }

        self = Logger_Handler_new(fileHandlerPublishCallback, fileHandlerFlushCallback, fileHandlerCloseCallback);
        if (!self) {
            fclose(file);
            err = LOGGER_ERR_OUT_OF_MEMORY;
            break;
        }

        Logger_Handler_setContext(self, file);
        Logger_Handler_setLevel(self, level);
        Logger_Handler_setFormatter(self, formatter);
    } while (false);

    return (Logger_Handler_Result_T) {.err=err, .handler=self};
}

/*
 * Rotating File Handler
 */
typedef struct rotatingFileHandlerContext {
    size_t BYTES_BEFORE_ROTATION;
    size_t bytesWritten;
    size_t rotationCounter;
    const char *filePath;
    FILE *file;
} *rotatingFileHandlerContext;

static Logger_Err_T rotatingFileHandlerPublishCallback(Logger_Handler_T handler, Logger_Record_T record) {
    assert(handler);
    assert(record);

    char *log = NULL;
    sds realFilePath = NULL;
    Logger_Err_T err = LOGGER_ERR_OK;
    Logger_Formatter_T formatter = Logger_Handler_getFormatter(handler);
    rotatingFileHandlerContext context = Logger_Handler_getContext(handler);

    do {
        log = Logger_Formatter_formatRecord(formatter, record);
        if (!log) {
            err = LOGGER_ERR_OUT_OF_MEMORY;
            break;
        }

        if (context->bytesWritten >= context->BYTES_BEFORE_ROTATION) { /* rotate */
            Logger_Handler_flush(handler);
            context->rotationCounter++;

            realFilePath = sdsempty();
            if (!realFilePath) {
                err = LOGGER_ERR_OUT_OF_MEMORY;
                break;
            }
            realFilePath = sdscatprintf(realFilePath, "%s.%zu", context->filePath, context->rotationCounter);
            if (!realFilePath) {
                err = LOGGER_ERR_OUT_OF_MEMORY;
                break;
            }

            FILE *newFile = fopen(realFilePath, "w");
            invalidate_ptr((void **) &realFilePath, sdsfree);
            if (!newFile) {
                err = Logger_Err_fromErrno(errno);
                break;
            }

            fclose(context->file);
            context->file = newFile;
            context->bytesWritten = 0;
        }

        const int bytesWritten = fprintf(context->file, "%s", log);
        if (bytesWritten < 0) {
            err = LOGGER_ERR_IO;
            break;
        }
        context->bytesWritten += bytesWritten;
    } while (false);

    Logger_Formatter_deleteFormattedRecord(formatter, log);
    sdsfree(realFilePath);
    Logger_Handler_flush(handler);
    return err;
}

static void rotatingFileHandlerFlushCallback(Logger_Handler_T handler) {
    assert(handler);
    rotatingFileHandlerContext context = Logger_Handler_getContext(handler);
    fflush(context->file);
}

static void rotatingFileHandlerCloseCallback(Logger_Handler_T handler) {
    assert(handler);
    rotatingFileHandlerContext context = Logger_Handler_getContext(handler);
    fclose(context->file);
    free(context);
}

Logger_Handler_Result_T Logger_Handler_newRotatingFileHandler(
        Logger_Level_T level, Logger_Formatter_T formatter, const char *filePath, size_t bytesBeforeRotation
) {
    assert(filePath);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(formatter);
    FILE *file = NULL;
    sds realFilePath = NULL;
    Logger_Handler_T self = NULL;
    Logger_Err_T err = LOGGER_ERR_OK;
    rotatingFileHandlerContext context = NULL;
    const size_t ROTATION_COUNTER = 0;

    realFilePath = sdsempty();
    if (!realFilePath) {
        err = LOGGER_ERR_OUT_OF_MEMORY;
        goto cleanup;
    }

    realFilePath = sdscatprintf(realFilePath, "%s.%zu", filePath, ROTATION_COUNTER);
    if (!realFilePath) {
        err = LOGGER_ERR_OUT_OF_MEMORY;
        goto cleanup;
    }

    file = fopen(realFilePath, "w");
    if (!file) {
        err = Logger_Err_fromErrno(errno);
        goto cleanup;
    }

    context = malloc(sizeof(*context));
    if (!context) {
        err = LOGGER_ERR_OUT_OF_MEMORY;
        goto cleanup;
    }
    context->file = file;
    context->filePath = filePath;
    context->bytesWritten = 0;
    context->rotationCounter = ROTATION_COUNTER;
    context->BYTES_BEFORE_ROTATION = bytesBeforeRotation;

    self = Logger_Handler_new(rotatingFileHandlerPublishCallback, rotatingFileHandlerFlushCallback, rotatingFileHandlerCloseCallback);
    if (!self) {
        err = LOGGER_ERR_OUT_OF_MEMORY;
        goto cleanup;
    }
    Logger_Handler_setLevel(self, level);
    Logger_Handler_setContext(self, context);
    Logger_Handler_setFormatter(self, formatter);

    exit:
    {
        sdsfree(realFilePath);  /* this needs to be freed in any case */
        return (Logger_Handler_Result_T) {.err=err, .handler=self};
    }
    cleanup:
    {
        if (file) {
            fclose(file);
        }
        free(context);
        goto exit;
    }
}

/*
 * Memory File Handler
 */
typedef struct memoryFileHandlerContext {
    size_t BYTES_BEFORE_WRITE;
    size_t bytesStored;
    FILE *file;
} *memoryFileHandlerContext;

static Logger_Err_T memoryFileHandlerPublishCallback(Logger_Handler_T handler, Logger_Record_T record) {
    assert(handler);
    assert(record);

    char *log = NULL;
    Logger_Err_T err = LOGGER_ERR_OK;
    Logger_Formatter_T formatter = Logger_Handler_getFormatter(handler);
    memoryFileHandlerContext context = Logger_Handler_getContext(handler);

    do {
        log = Logger_Formatter_formatRecord(formatter, record);
        if (!log) {
            err = LOGGER_ERR_OUT_OF_MEMORY;
            break;
        }

        if (context->bytesStored >= context->BYTES_BEFORE_WRITE) {
            fflush(context->file);
        }

        const int bytesStored = fprintf(context->file, "%s", log);
        if (bytesStored < 0) {
            err = LOGGER_ERR_IO;
            break;
        }
        context->bytesStored += bytesStored;
    } while (false);

    Logger_Formatter_deleteFormattedRecord(formatter, log);
    return err;
}

static void memoryFileHandlerFlushCallback(Logger_Handler_T handler) {
    assert(handler);
    memoryFileHandlerContext context = Logger_Handler_getContext(handler);
    fflush(context->file);
}

static void memoryFileHandlerCloseCallback(Logger_Handler_T handler) {
    assert(handler);
    memoryFileHandlerContext context = Logger_Handler_getContext(handler);
    fclose(context->file);
    free(context);
}

/* TODO: use a custom buffer */
Logger_Handler_Result_T Logger_Handler_newMemoryFileHandler(
        Logger_Level_T level, Logger_Formatter_T formatter, const char *filePath, size_t bytesBeforeWrite
) {
    assert(filePath);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(formatter);
    FILE *file = NULL;
    Logger_Handler_T self = NULL;
    Logger_Err_T err = LOGGER_ERR_OK;
    memoryFileHandlerContext context = NULL;

    file = fopen(filePath, "w");
    if (!file) {
        err = Logger_Err_fromErrno(errno);
        goto cleanup;
    }

    context = malloc(sizeof(*context));
    if (!context) {
        err = LOGGER_ERR_OUT_OF_MEMORY;
        goto cleanup;
    }
    context->file = file;
    context->bytesStored = 0;
    context->BYTES_BEFORE_WRITE = bytesBeforeWrite;

    self = Logger_Handler_new(memoryFileHandlerPublishCallback, memoryFileHandlerFlushCallback, memoryFileHandlerCloseCallback);
    if (!self) {
        err = LOGGER_ERR_OUT_OF_MEMORY;
        goto cleanup;
    }
    Logger_Handler_setLevel(self, level);
    Logger_Handler_setContext(self, context);
    Logger_Handler_setFormatter(self, formatter);

    exit:
    {
        return (Logger_Handler_Result_T) {.err=err, .handler=self};
    }
    cleanup:
    {
        if (file) {
            fclose(file);
        }
        free(context);
        goto exit;
    }
}
