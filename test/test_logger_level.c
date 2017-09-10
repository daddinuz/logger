/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 08, 2017 
 */


#include "traits/traits.h"
#include "traits-unit/traits-unit.h"
#include "logger_level.h"

/*
 * Declare features
 */
FeatureDeclare(GetLoggerLevelName);

/*
 * Describe the test case
 */
Describe("LoggerLevel",
         Trait(
                 "Basic",
                 Run(GetLoggerLevelName)
         )
)

/*
 * Define features
 */
FeatureDefine(GetLoggerLevelName) {
    (void) traits_context;
    const char *const EXPECTED[] = {"DEBUG", "NOTICE", "INFO", "WARNING", "ERROR", "FATAL"};
    for (size_t i = 0, size = sizeof(EXPECTED) / sizeof(EXPECTED[0]); i < size; i++) {
        assert_string_equal(EXPECTED[i], Logger_Level_getName(i));
    }
}
