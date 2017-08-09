/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 04, 2017 
 */

#ifndef LOGGER_LOGGER_BUILTIN_FORMATTERS_INCLUDED
#define LOGGER_LOGGER_BUILTIN_FORMATTERS_INCLUDED

#include "logger_formatter.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates and initializes a pre-defined Logger_Formatter_T.
 *
 * Checked runtime errors:
 *  - In case of OOM this function will return NULL and errno will be set to ENOMEM.
 *
 * @return A new instance of a pre-defined Logger_Formatter_T.
 */
extern Logger_Formatter_T Logger_Formatter_newSimpleFormatter(void);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_BUILTIN_FORMATTERS_INCLUDED */
