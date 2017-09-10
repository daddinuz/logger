/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 08, 2017 
 */

#include "traits/traits.h"
#include "traits-unit/traits-unit.h"
#include "logger_string.h"

/*
 * Declare helpers
 */
static Logger_String_T Helper_LoggerStringFromArgumentsList(const char *fmt, ...);

/*
 * Declare features
 */
FeatureDeclare(NewAndDelete);
FeatureDeclare(NewFromFormatAndDelete);
FeatureDeclare(NewFromArgumentsListAndDelete);

/*
 * Describe the test case
 */
Describe("LoggerString",
         Trait(
                 "Basic",
                 Run(NewAndDelete),
                 Run(NewFromFormatAndDelete),
                 Run(NewFromArgumentsListAndDelete)
         )
)

/*
 * Define helpers
 */
Logger_String_T Helper_LoggerStringFromArgumentsList(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    Logger_String_T sut = Logger_String_fromArgumentsList(fmt, args);
    va_end(args);
    return sut;
}

/*
 * Define features
 */
FeatureDefine(NewAndDelete) {
    (void) traits_context;
    Logger_String_T sut = Logger_String_new("Hello World!\n");
    assert_not_null(sut);
    assert_string_equal("Hello World!\n", sut);
    Logger_String_delete(&sut);
    assert_null(sut);
}

FeatureDefine(NewFromFormatAndDelete) {
    (void) traits_context;
    Logger_String_T sut = Logger_String_from("Hello %s%c\n", "World", '!');
    assert_not_null(sut);
    assert_string_equal("Hello World!\n", sut);
    Logger_String_delete(&sut);
    assert_null(sut);
}

FeatureDefine(NewFromArgumentsListAndDelete) {
    (void) traits_context;
    Logger_String_T sut = Helper_LoggerStringFromArgumentsList("Hello %s%c\n", "World", '!');
    assert_not_null(sut);
    assert_string_equal("Hello World!\n", sut);
    Logger_String_delete(&sut);
    assert_null(sut);
}
