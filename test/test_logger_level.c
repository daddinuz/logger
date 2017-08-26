/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 08, 2017 
 */

#include "Traits/Traits.h"
#include "logger_level.h"

/*
 *
 */
TRAITS(CanGetALoggerLevelName) {
    const char *const EXPECTED[] = {"DEBUG", "NOTICE", "INFO", "WARNING", "ERROR", "FATAL"};
    for (size_t i = 0, size = sizeof(EXPECTED) / sizeof(EXPECTED[0]); i < size; i++) {
        assert_string_equal(EXPECTED[i], Logger_Level_getName(i));
    }
}

/*
 *
 */
int main() {
    traits_run(CanGetALoggerLevelName);
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
