/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 09, 2017 
 */

#include <errno.h>
#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>
#include "sds/sds.h"
#include "logger_string.h"

Logger_String_T Logger_String_new(const char *str) {
    assert(str);
    sds self = sdsnew(str);
    if (!self) {
        errno = ENOMEM;
    }
    return self;
}

Logger_String_T Logger_String_from(const char *fmt, ...) {
    assert(fmt);
    va_list args;
    va_start(args, fmt);
    sds self = sdscatvprintf(sdsempty(), fmt, args);
    if (!self) {
        errno = ENOMEM;
    }
    va_end(args);
    return self;
}

void Logger_String_delete(Logger_String_T *ref) {
    assert(ref);
    assert(*ref);
    sds self = (sds) *ref;
    sdsfree(self);
    *ref = NULL;
}
