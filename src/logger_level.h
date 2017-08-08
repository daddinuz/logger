/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 07, 2017 
 */

#ifndef LOGGER_LOGGER_LEVEL_INCLUDED
#define LOGGER_LOGGER_LEVEL_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Logger_Level_T structs are used to control logging output.
 * The logging levels are ordered and are specified by ordered integers.
 * Enabling logging at a given level also enables logging at all higher levels.
 */
typedef enum Logger_Level_T {
    LOGGER_LEVEL_DEBUG = 0,
    LOGGER_LEVEL_NOTICE,
    LOGGER_LEVEL_INFO,
    LOGGER_LEVEL_WARNING,
    LOGGER_LEVEL_ERROR,
    LOGGER_LEVEL_FATAL
} Logger_Level_T;

/**
 * Get the string representation of a Logger_Level_T.
 *
 * Checked runtime errors:
 *  - @param level must be in range LOGGER_LEVEL_DEBUG - LOGGER_LEVEL_FATAL.
 *
 * @param level The logging level.
 * @return The string representation of the Logger_Level_T.
 */
extern const char *Logger_Level_getName(Logger_Level_T level);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_LEVEL_INCLUDED */
