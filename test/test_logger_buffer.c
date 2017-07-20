/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   July 20, 2017 
 */

#include "sds/sds.h"
#include "logger.h"
#include "Traits/Traits.h"

TRAITS(CanCreateANewLoggerBuffer) {
    sds data = sdsnew("Hello World!\n");

    Logger_Buffer_T sut = Logger_Buffer_new(sdslen(data), data);
    assert_not_null(sut);

    assert_equal(sdslen(data), sut->size);
    assert_string_equal(data, sut->content);

    Logger_Buffer_delete(&sut);
    assert_null(sut);
    assert_not_null(data);

    sdsfree(data);
}

/*
 *
 */
int main() {
    traits_run(CanCreateANewLoggerBuffer);
    return 0;
}

void traits_setup(void) {
    /* Nothing to do */
}

void traits_teardown(void) {
    /* Nothing to do */
}
