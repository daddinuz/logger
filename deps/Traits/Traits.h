/*
 *  C Header File
 *
 *  Author: Davide Di Carlo
 *  Date:   April 2, 2017
 *  email:  daddinuz@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef __TRAITS_H__
#define __TRAITS_H__

#define TRAITS_VERSION "1.1.0"

/*
 * Useful macros
 */
#define __traits_stringify(x)   #x
#define traits_stringify(x)     __traits_stringify(x)

/*
 * Assertions framework
 */
#define __traits_assert(_Expression, ...)                                                       \
    do {                                                                                        \
        if (!(_Expression)) {                                                                   \
            fprintf(stderr, "At " __FILE__ ":" traits_stringify(__LINE__) "\n" __VA_ARGS__);    \
            abort();                                                                            \
        }                                                                                       \
    } while (false)

/*
 * Boolean
 */
#define assert(_Expression)                             __traits_assert((_Expression), "expected %s", traits_stringify(_Expression))
#define assert_true(_Expression)                        __traits_assert(true == (_Expression), "expected to be true")
#define assert_false(_Expression)                       __traits_assert(false == (_Expression), "expected to be false")

/*
 * Numerical
 */
#define assert_equal(_Expected, _Actual)                __traits_assert((_Expected) == (_Actual), "expected %s to be equal to %s", traits_stringify(_Actual), traits_stringify(_Expected))
#define assert_not_equal(_Expected, _Actual)            __traits_assert((_Expected) != (_Actual), "expected %s to not be equal to %s", traits_stringify(_Actual), traits_stringify(_Expected))
#define assert_less_equal(_Expected, _Actual)           __traits_assert((_Expected) <= (_Actual), "expected %s to be less equal than %s", traits_stringify(_Actual), traits_stringify(_Expected))
#define assert_greater_equal(_Expected, _Actual)        __traits_assert((_Expected) >= (_Actual), "expected %s to be greater equal than %s", traits_stringify(_Actual), traits_stringify(_Expected))
#define assert_less(_Expected, _Actual)                 __traits_assert((_Expected) < (_Actual), "expected %s to be less than %s", traits_stringify(_Actual), traits_stringify(_Expected))
#define assert_greater(_Expected, _Actual)              __traits_assert((_Expected) > (_Actual), "expected %s to be greater than %s", traits_stringify(_Actual), traits_stringify(_Expected))

/*
 * Pointer
 */
#define assert_null(_Pointer)                           __traits_assert(NULL == (_Pointer), "expected %s to be NULL", traits_stringify(_Pointer))
#define assert_not_null(_Pointer)                       __traits_assert(NULL != (_Pointer), "expected %s to not be NULL", traits_stringify(_Pointer))

/*
 * String
 */
#define assert_string_equal(_Expected, _Actual)         __traits_assert(0 == strcmp((_Expected), (_Actual)), "expected \"%s\" to be equal to \"%s\"", _Actual, _Expected)
#define assert_string_not_equal(_Expected, _Actual)     __traits_assert(0 != strcmp((_Expected), (_Actual)), "expected \"%s\" to not be equal to \"%s\"", _Actual, _Expected)

/*
 * Test Framework
 */
extern void traits_setup(void);                                     /* Must be implemented by the user */
extern void traits_teardown(void);                                  /* Must be implemented by the user */

#define TRAITS(_Case)                                               \
    void test_##_Case(void)

#define traits_run(_Case)                                           \
    do {                                                            \
        printf("Running: " traits_stringify(_Case) "... ");         \
        traits_setup();                                             \
        test_##_Case();                                             \
        traits_teardown();                                          \
        puts("done");                                               \
    } while (false)

#define traits_skip(_Case)                                          \
    do {                                                            \
        puts("Skipping: " traits_stringify(_Case) "... done");      \
    } while (false)

#endif /* __TRAITS_H__ */
