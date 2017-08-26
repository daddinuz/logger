/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 09, 2017 
 */

#ifndef LOGGER_LOGGER_STRING_INCLUDED
#define LOGGER_LOGGER_STRING_INCLUDED

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Logger_String_T is an helper to build heap allocated strings.
 */
typedef const char *Logger_String_T;

/**
 * Construct a Logger_String_T.
 *
 * Checked runtime errors:
 *  - @param str must not be NULL.
 *
 * @param str The content of the string.
 * @return A new instance of Logger_String_T.
 */
extern Logger_String_T Logger_String_new(const char *str);

/**
 * Construct a Logger_String_T from a printf-like format.
 *
 * Checked runtime errors:
 *  - @param fmt must not be NULL.
 *
 * @param fmt The printf-like fmt string.
 * @param ...
 * @return A new instance of Logger_String_T.
 */
extern Logger_String_T Logger_String_from(const char *fmt, ...);

/**
 * Construct a Logger_String_T from a printf-like format.
 *
 * Checked runtime errors:
 *  - @param fmt must not be NULL.
 *  - @param args must not be NULL.
 *
 * @param fmt The printf-like fmt string.
 * @param args The arguments list.
 * @return A new instance of Logger_String_T.
 */
extern Logger_String_T Logger_String_fromArgumentsList(const char *fmt, va_list args);

/**
 * Destruct a Logger_String_T.
 *
 * Checked runtime errors:
 *  - @param ref must be a valid reference to a Logger_String_T instance.
 *
 * @param ref The reference to the Logger_String_T instance.
 */
extern void Logger_String_delete(Logger_String_T *ref);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_STRING_INCLUDED */
