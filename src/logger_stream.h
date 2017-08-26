/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 26, 2017 
 */

#ifndef LOGGER_LOGGER_STREAM_INCLUDED
#define LOGGER_LOGGER_STREAM_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef enum Logger_OStream_T {
    LOGGER_OSTREAM_STDERR,
    LOGGER_OSTREAM_STDOUT,
} Logger_OStream_T;

typedef enum Logger_IStream_T {
    LOGGER_ISTREAM_STDIN,
} Logger_IStream_T;

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_LOGGER_STREAM_INCLUDED */
