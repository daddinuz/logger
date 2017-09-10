/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   September 02, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#ifndef __TRAITS_H__
#define __TRAITS_H__

#define TRAITS_VERSION "2.1.0"

#if defined(__GNUC__) || defined(__clang__)
#   define __TRAITS_FORMAT_ATTRIBUTE(x, y)      __attribute__((__format__(__printf__, x, y)))
#else
#   define __TRAITS_FORMAT_ATTRIBUTE(x, y)
#endif

#define __TRAITS_IMPL_CAT(x, ...)               x ## __VA_ARGS__
#define __TRAITS_CAT(x, ...)                    __TRAITS_IMPL_CAT(x, __VA_ARGS__)

#define __TRAITS_IMPL_TO_STRING(x)              #x
#define __TRAITS_TO_STRING(x)                   __TRAITS_IMPL_TO_STRING(x)

#define __TRAITS_IMPL_MORE_THAN_ONE(/**/\
        _1, _2, _3, _4, _5, _6, _7, _8, \
        _9, _a, _b, _c, _d, _e, _f, _g, \
        _h, _i, _j, _k, _l, _m, _n, _o, \
        _p, _q, _r, _s, _t, _u, _v, _w, \
        N, ...)                                 N
#define __TRAITS_MORE_THAN_ONE(...)             __TRAITS_IMPL_MORE_THAN_ONE(                \
                                                        __VA_ARGS__, 1, 1, 1, 1, 1, 1, 1,   \
                                                        1, 1, 1, 1, 1, 1, 1, 1,             \
                                                        1, 1, 1, 1, 1, 1, 1, 1,             \
                                                        1, 1, 1, 1, 1, 1, 1, 1,             \
                                                        0, 0                                \
                                                    )

#define __TRAITS_IMPL_MORE_THAN_TWO(/**/\
        _1, _2, _3, _4, _5, _6, _7, _8, \
        _9, _a, _b, _c, _d, _e, _f, _g, \
        _h, _i, _j, _k, _l, _m, _n, _o, \
        _p, _q, _r, _s, _t, _u, _v, _w, \
        _x, N, ...)                             N
#define __TRAITS_MORE_THAN_TWO(...)             __TRAITS_IMPL_MORE_THAN_TWO(                \
                                                        __VA_ARGS__, 1, 1, 1, 1, 1, 1, 1,   \
                                                        1, 1, 1, 1, 1, 1, 1, 1,             \
                                                        1, 1, 1, 1, 1, 1, 1, 1,             \
                                                        1, 1, 1, 1, 1, 1, 1, 1,             \
                                                        0, 0, 0                             \
                                                    )

#define __TRAITS_STATIC_IF_0(x, y)              x
#define __TRAITS_STATIC_IF_1(x, y)              y
#define __TRAITS_STATIC_IF(c)                   __TRAITS_CAT(__TRAITS_STATIC_IF_, c)

#define __TRAITS_OVERLOAD_ONE(f0, f1, ...)      __TRAITS_STATIC_IF(__TRAITS_MORE_THAN_ONE(__VA_ARGS__))(f0, f1)(__VA_ARGS__)
#define __TRAITS_OVERLOAD_TWO(f0, f1, ...)      __TRAITS_STATIC_IF(__TRAITS_MORE_THAN_TWO(__VA_ARGS__))(f0, f1)(__VA_ARGS__)

/*
 * Helpers
 */
#define eq(e, a)                                ((e) == (a))
#define ne(e, a)                                ((e) != (a))
#define gt(e, a)                                ((e)  > (a))
#define ge(e, a)                                ((e) >= (a))
#define lt(e, a)                                ((e)  < (a))
#define le(e, a)                                ((e) <= (a))

#define as(T, x)                                ((T)(x))
#define eq_as(T, e, a)                          eq(as(T, e), as(T, a))
#define ne_as(T, e, a)                          ne(as(T, e), as(T, a))
#define gt_as(T, e, a)                          gt(as(T, e), as(T, a))
#define ge_as(T, e, a)                          ge(as(T, e), as(T, a))
#define lt_as(T, e, a)                          lt(as(T, e), as(T, a))
#define le_as(T, e, a)                          le(as(T, e), as(T, a))

/*
 * Assertions framework
 */
static void
__traits_assert(bool condition, size_t line, const char *file, const char *assertion, const char *message, ...)
__TRAITS_FORMAT_ATTRIBUTE(5, 6);

static void
__traits_assert(bool condition, size_t line, const char *file, const char *assertion, const char *message, ...) {
    va_list args;
    if (!condition) {
        fprintf(stderr, "At %s:%zu\nAssertion `%s` failed.\n", file, line, assertion);
        va_start(args, message);
        vfprintf(stderr, message, args);
        va_end(args);
        exit(1);
    }
}

#define __traits_helper_x(A, x, ...)            __traits_assert((x), __LINE__, __FILE__, A, __VA_ARGS__)
#define __traits_helper_0(A, x)                 __traits_helper_x(A, x, NULL)
#define __traits_helper_1(A, x, ...)            __traits_helper_x(A, x, __VA_ARGS__)
#define __traits_helper(A, ...)                 __TRAITS_STATIC_IF(__TRAITS_MORE_THAN_ONE(__VA_ARGS__))(__traits_helper_0, __traits_helper_1)(A, __VA_ARGS__)

/*
 * Basic
 */
#define __traits_assert_x(x, ...)               do { const bool __traits_x = (x); __traits_helper(__TRAITS_TO_STRING(x), eq(true, __traits_x), __VA_ARGS__); } while(false)
#define __traits_assert_0(x)                    __traits_assert_x(x, NULL)
#define __traits_assert_1(x, ...)               __traits_assert_x(x, __VA_ARGS__)
#define traits_assert(...)                      __TRAITS_OVERLOAD_ONE(__traits_assert_0, __traits_assert_1, __VA_ARGS__)

/*
 * Boolean
 */
#define __assert_true_x(x, ...)                 do { const bool __traits_x = (x); __traits_helper(__TRAITS_TO_STRING(x), eq(true, __traits_x), __VA_ARGS__); } while(false)
#define __assert_true_0(x)                      __assert_true_x(x, "Expected to be true.\n")
#define __assert_true_1(x, ...)                 __assert_true_x(x, __VA_ARGS__)
#define assert_true(...)                        __TRAITS_OVERLOAD_ONE(__assert_true_0, __assert_true_1, __VA_ARGS__)

#define __assert_false_x(x, ...)                do { const bool __traits_x = (x); __traits_helper(__TRAITS_TO_STRING(x), eq(false, __traits_x), __VA_ARGS__); } while(false)
#define __assert_false_0(x)                     __assert_false_x(x, "Expected to be false.\n")
#define __assert_false_1(x, ...)                __assert_false_x(x, __VA_ARGS__)
#define assert_false(...)                       __TRAITS_OVERLOAD_ONE(__assert_false_0, __assert_false_1, __VA_ARGS__)

/*
 * Numerical
 */
#define __assert_equal_x(e, a, ...)             __traits_helper(__TRAITS_TO_STRING(eq(e, a)), eq(e, a), __VA_ARGS__)
#define __assert_equal_0(e, a)                  __assert_equal_x(e, a, "Expected to be equal.\n")
#define __assert_equal_1(e, a, ...)             __assert_equal_x(e, a, __VA_ARGS__)
#define assert_equal(...)                       __TRAITS_OVERLOAD_TWO(__assert_equal_0, __assert_equal_1, __VA_ARGS__)

#define __assert_not_equal_x(e, a, ...)         __traits_helper(__TRAITS_TO_STRING(ne(e, a)), ne(e, a), __VA_ARGS__)
#define __assert_not_equal_0(e, a)              __assert_not_equal_x(e, a, "Expected to be not equal.\n")
#define __assert_not_equal_1(e, a, ...)         __assert_not_equal_x(e, a, __VA_ARGS__)
#define assert_not_equal(...)                   __TRAITS_OVERLOAD_TWO(__assert_not_equal_0, __assert_not_equal_1, __VA_ARGS__)

#define __assert_greater_x(e, a, ...)           __traits_helper(__TRAITS_TO_STRING(gt(e, a)), gt(e, a), __VA_ARGS__)
#define __assert_greater_0(e, a)                __assert_greater_x(e, a, "Expected to be greater.\n")
#define __assert_greater_1(e, a, ...)           __assert_greater_x(e, a, __VA_ARGS__)
#define assert_greater(...)                     __TRAITS_OVERLOAD_TWO(__assert_greater_0, __assert_greater_1, __VA_ARGS__)

#define __assert_greater_equal_x(e, a, ...)     __traits_helper(__TRAITS_TO_STRING(ge(e, a)), ge(e, a), __VA_ARGS__)
#define __assert_greater_equal_0(e, a)          __assert_greater_equal_x(e, a, "Expected to be greater or equal.\n")
#define __assert_greater_equal_1(e, a, ...)     __assert_greater_equal_x(e, a, __VA_ARGS__)
#define assert_greater_equal(...)               __TRAITS_OVERLOAD_TWO(__assert_greater_equal_0, __assert_greater_equal_1, __VA_ARGS__)

#define __assert_less_x(e, a, ...)              __traits_helper(__TRAITS_TO_STRING(lt(e, a)), lt(e, a), __VA_ARGS__)
#define __assert_less_0(e, a)                   __assert_less_x(e, a, "Expected to be less.\n")
#define __assert_less_1(e, a, ...)              __assert_less_x(e, a, __VA_ARGS__)
#define assert_less(...)                        __TRAITS_OVERLOAD_TWO(__assert_less_0, __assert_less_1, __VA_ARGS__)

#define __assert_less_equal_x(e, a, ...)        __traits_helper(__TRAITS_TO_STRING(le(e, a)), le(e, a), __VA_ARGS__)
#define __assert_less_equal_0(e, a)             __assert_less_equal_x(e, a, "Expected to be less or equal.\n")
#define __assert_less_equal_1(e, a, ...)        __assert_less_equal_x(e, a, __VA_ARGS__)
#define assert_less_equal(...)                  __TRAITS_OVERLOAD_TWO(__assert_less_equal_0, __assert_less_equal_1, __VA_ARGS__)

/*
 * Pointer
 */
#define __assert_null_x(x, ...)                 do { const void *__traits_x = (x); __traits_helper(__TRAITS_TO_STRING(x), eq(NULL, __traits_x), __VA_ARGS__); } while(false)
#define __assert_null_0(x)                      __assert_null_x(x, "Expected to be null.\n")
#define __assert_null_1(x, ...)                 __assert_null_x(x, __VA_ARGS__)
#define assert_null(...)                        __TRAITS_OVERLOAD_ONE(__assert_null_0, __assert_null_1, __VA_ARGS__)

#define __assert_not_null_x(x, ...)             do { const void *__traits_x = (x); __traits_helper(__TRAITS_TO_STRING(x), ne(NULL, __traits_x), __VA_ARGS__); } while(false)
#define __assert_not_null_0(x)                  __assert_not_null_x(x, "Expected to be not null.\n")
#define __assert_not_null_1(x, ...)             __assert_not_null_x(x, __VA_ARGS__)
#define assert_not_null(...)                    __TRAITS_OVERLOAD_ONE(__assert_not_null_0, __assert_not_null_1, __VA_ARGS__)

/*
 * String
 */
#define __assert_string_equal_x(e, a, ...)      do { const char *const __trait_e = (e), *const __trait_a = (a); __traits_helper(__TRAITS_TO_STRING(eq(0, strcmp(e, a))), eq(0, strcmp(__trait_e, __trait_a)), __VA_ARGS__); } while(false)
#define __assert_string_equal_0(e, a)           __assert_string_equal_x(e, a, "Expected to be equal.\n")
#define __assert_string_equal_1(e, a, ...)      __assert_string_equal_x(e, a, __VA_ARGS__)
#define assert_string_equal(...)                __TRAITS_OVERLOAD_TWO(__assert_string_equal_0, __assert_string_equal_1, __VA_ARGS__)

#define __assert_string_not_equal_x(e, a, ...)  do { const char *const __trait_e = (e), *const __trait_a = (a); __traits_helper(__TRAITS_TO_STRING(ne(0, strcmp(e, a))), ne(0, strcmp(__trait_e, __trait_a)), __VA_ARGS__); } while(false)
#define __assert_string_not_equal_0(e, a)       __assert_string_not_equal_x(e, a, "Expected to be not equal.\n")
#define __assert_string_not_equal_1(e, a, ...)  __assert_string_not_equal_x(e, a, __VA_ARGS__)
#define assert_string_not_equal(...)            __TRAITS_OVERLOAD_TWO(__assert_string_not_equal_0, __assert_string_not_equal_1, __VA_ARGS__)

#endif /* __TRAITS_H__ */
