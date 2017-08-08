/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 07, 2017 
 */

#include <stdlib.h>
#include <assert.h>
#include "logger_level.h"

static const char *LOGGER_LEVEL_STR_DEBUG = "DEBUG";
static const char *LOGGER_LEVEL_STR_NOTICE = "NOTICE";
static const char *LOGGER_LEVEL_STR_INFO = "INFO";
static const char *LOGGER_LEVEL_STR_WARNING = "WARNING";
static const char *LOGGER_LEVEL_STR_ERROR = "ERROR";
static const char *LOGGER_LEVEL_STR_FATAL = "FATAL";

const char *Logger_Level_getName(Logger_Level_T level) {
    assert(LOGGER_LEVEL_DEBUG <= level && level <= LOGGER_LEVEL_FATAL);
    switch (level) {
        case LOGGER_LEVEL_DEBUG:
            return LOGGER_LEVEL_STR_DEBUG;
        case LOGGER_LEVEL_NOTICE:
            return LOGGER_LEVEL_STR_NOTICE;
        case LOGGER_LEVEL_INFO:
            return LOGGER_LEVEL_STR_INFO;
        case LOGGER_LEVEL_WARNING:
            return LOGGER_LEVEL_STR_WARNING;
        case LOGGER_LEVEL_ERROR:
            return LOGGER_LEVEL_STR_ERROR;
        case LOGGER_LEVEL_FATAL:
            return LOGGER_LEVEL_STR_FATAL;
        default:
            abort();
    };
}
