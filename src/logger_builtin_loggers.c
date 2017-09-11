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

#define NEW_LOGGER_FUNCTION_TEMPLATE(__argName__, __argLevel__, __argFunction__, ...)   \
    Logger_Handler_Result_T result;                                                     \
    Logger_Err_T err = LOGGER_ERR_OK;                                                   \
    Logger_T self = NULL;                                                               \
    Logger_Formatter_T formatter = NULL;                                                \
                                                                                        \
    formatter = Logger_Formatter_newSimpleFormatter();                                  \
    if (!formatter) {                                                                   \
        err = LOGGER_ERR_OUT_OF_MEMORY;                                                 \
        goto cleanup;                                                                   \
    }                                                                                   \
                                                                                        \
    result = __argFunction__(__argLevel__, formatter, __VA_ARGS__);                     \
    if (LOGGER_ERR_OK != result.err) {                                                  \
        err = result.err;                                                               \
        goto cleanup;                                                                   \
    }                                                                                   \
                                                                                        \
    self = Logger_new(__argName__, __argLevel__);                                       \
    if (!self) {                                                                        \
        err = LOGGER_ERR_OUT_OF_MEMORY;                                                 \
        goto cleanup;                                                                   \
    }                                                                                   \
    Logger_addHandler(self, result.handler);                                            \
                                                                                        \
    exit: {                                                                             \
        return (Logger_Result_T) {.err=err, .logger=self};                              \
    }                                                                                   \
    cleanup: {                                                                          \
        if (formatter) {                                                                \
            Logger_Formatter_delete(&formatter);                                        \
        }                                                                               \
        if (result.handler) {                                                           \
            Logger_Handler_delete(&result.handler);                                     \
        }                                                                               \
        goto exit;                                                                      \
    }

Logger_Result_T Logger_newStdoutLogger(const char *name, Logger_Level_T level) {
    assert(name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);

    NEW_LOGGER_FUNCTION_TEMPLATE(name, level, Logger_Handler_newConsoleHandler, LOGGER_OSTREAM_STDOUT)
}

Logger_Result_T Logger_newStderrLogger(const char *name, Logger_Level_T level) {
    assert(name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);

    NEW_LOGGER_FUNCTION_TEMPLATE(name, level, Logger_Handler_newConsoleHandler, LOGGER_OSTREAM_STDERR)
}

Logger_Result_T Logger_newFileLogger(const char *name, Logger_Level_T level, const char *filePath) {
    assert(name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(filePath);

    NEW_LOGGER_FUNCTION_TEMPLATE(name, level, Logger_Handler_newFileHandler, filePath)
}

Logger_Result_T Logger_newRotatingFileLogger(
        const char *name, Logger_Level_T level, const char *filePath, size_t bytesBeforeRotation
) {
    assert(name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(filePath);

    NEW_LOGGER_FUNCTION_TEMPLATE(name, level, Logger_Handler_newRotatingFileHandler, filePath, bytesBeforeRotation)
}

Logger_Result_T Logger_newMemoryFileLogger(
        const char *name, Logger_Level_T level, const char *filePath, size_t bytesBeforeWrite
) {
    assert(name);
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    assert(filePath);

    NEW_LOGGER_FUNCTION_TEMPLATE(name, level, Logger_Handler_newMemoryFileHandler, filePath, bytesBeforeWrite)
}
