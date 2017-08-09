/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 08, 2017 
 */

#include "Traits/Traits.h"
#include "logger_string.h"

/*
 *
 */
TRAITS(CanCreateALoggerString) {
    Logger_String_T sut = Logger_String_new("Hello World!\n");
    assert_not_null(sut);
    assert_string_equal("Hello World!\n", sut);

    Logger_String_delete(&sut);
    assert_null(sut);

    sut = Logger_String_from("Hello %s%c\n", "World", '!');
    assert_not_null(sut);
    assert_string_equal("Hello World!\n", sut);

    Logger_String_delete(&sut);
    assert_null(sut);
}

/*
 *
 */
int main() {
    traits_run(CanCreateALoggerString);
    return 0;
}

/*
 *
 */
void traits_setup(void) {
    /* Nothing to do */
}

void traits_teardown(void) {
    /* Nothing to do */
}
