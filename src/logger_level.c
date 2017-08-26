/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 07, 2017 
 */

#include <assert.h>
#include <stdbool.h>
#include "logger_level.h"

static const char *LOGGER_LEVEL_DEBUG_STR = "DEBUG";
static const char *LOGGER_LEVEL_NOTICE_STR = "NOTICE";
static const char *LOGGER_LEVEL_INFO_STR = "INFO";
static const char *LOGGER_LEVEL_WARNING_STR = "WARNING";
static const char *LOGGER_LEVEL_ERROR_STR = "ERROR";
static const char *LOGGER_LEVEL_FATAL_STR = "FATAL";

const char *Logger_Level_getName(Logger_Level_T level) {
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
            assert(false);
    };
}
