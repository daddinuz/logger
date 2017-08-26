/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 09, 2017 
 */

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sds/sds.h"
#include "logger_string.h"

static sds format(const char *fmt, va_list args) {
    assert(fmt);
    assert(args);
    sds self = NULL;
    do {
        self = sdsempty();
        if (!self) {
            break;
        }
        self = sdscatvprintf(self, fmt, args);
        if (!self) {
            sdsfree(self);
            break;
        }
    } while (false);
    return self;
}

Logger_String_T Logger_String_new(const char *str) {
    assert(str);
    return sdsnew(str);
}

Logger_String_T Logger_String_from(const char *fmt, ...) {
    assert(fmt);
    va_list args;
    va_start(args, fmt);
    sds self = format(fmt, args);
    va_end(args);
    return self;
}

Logger_String_T Logger_String_fromArgumentsList(const char *fmt, va_list args) {
    assert(fmt);
    assert(args);
    return format(fmt, args);
}

void Logger_String_delete(Logger_String_T *ref) {
    assert(ref);
    assert(*ref);
    sds self = (sds) *ref;
    sdsfree(self);
    *ref = NULL;
}
