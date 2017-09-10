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
#include "logger_record.h"

/*
 * Define context
 */
typedef struct Context_T {
    Logger_Record_T sut;
    Logger_String_T MESSAGE;
    char *LOGGER_NAME;
    char *FUNCTION;
    char *FILE;
    size_t LINE;
    time_t TIMESTAMP;
    Logger_Level_T LEVEL;
} *Context_T;

/*
 * Declare setups
 */
SetupDeclare(SetupLoggerRecord);

/*
 * Declare teardowns
 */
TeardownDeclare(TeardownLoggerRecord);

/*
 * Declare fixtures
 */
FixtureDeclare(FixtureLoggerRecord);

/*
 * Declare features
 */
FeatureDefine(NewAndDelete);
FeatureDefine(Getters);
FeatureDefine(Setters);

/*
 * Describe the test case
 */
Describe("LoggerRecord",
         Trait(
                 "Basic",
                 Run(NewAndDelete, FixtureLoggerRecord),
                 Run(Getters, FixtureLoggerRecord),
                 Run(Setters, FixtureLoggerRecord)
         )
)

/*
 * Define setups
 */
SetupDefine(SetupLoggerRecord) {
    Context_T context = malloc(sizeof(*context));
    assert_not_null(context);
    context->MESSAGE = Logger_String_new("EXPECTED_MESSAGE");
    assert_not_null(context->MESSAGE);
    context->LOGGER_NAME = strdup("EXPECTED_LOGGER_NAME");
    assert_not_null(context->LOGGER_NAME);
    context->FUNCTION = strdup("EXPECTED_FUNCTION");
    assert_not_null(context->FUNCTION);
    context->FILE = strdup("EXPECTED_FILE");
    assert_not_null(context->FILE);
    context->LINE = 1;
    context->TIMESTAMP = 0;
    context->LEVEL = LOGGER_LEVEL_NOTICE;
    context->sut = Logger_Record_new(
            context->LOGGER_NAME, context->LEVEL, context->FILE, context->LINE,
            context->FUNCTION, context->TIMESTAMP, context->MESSAGE
    );
    assert_not_null(context->sut);
    return context;
}

/*
 * Define teardowns
 */
TeardownDefine(TeardownLoggerRecord) {
    assert_not_null(traits_context);
    Context_T context = traits_context;

    Logger_Record_delete(&context->sut);
    assert_null(context->sut);

    assert_not_null(context->MESSAGE);
    Logger_String_delete(&context->MESSAGE);
    free(context->LOGGER_NAME);
    free(context->FUNCTION);
    free(context->FILE);
    free(context);
}

/*
 * Define fixtures
 */
FixtureDefine(FixtureLoggerRecord, SetupLoggerRecord, TeardownLoggerRecord);

/*
 * Define features
 */
FeatureDefine(NewAndDelete) {
    (void) traits_context;
    /* Nothing to do */
}

FeatureDefine(Getters) {
    Context_T context = traits_context;
    Logger_Record_T sut = context->sut;

    assert_string_equal(context->MESSAGE, Logger_Record_getMessage(sut));
    assert_string_equal(context->LOGGER_NAME, Logger_Record_getLoggerName(sut));
    assert_string_equal(context->FUNCTION, Logger_Record_getFunction(sut));
    assert_string_equal(context->FILE, Logger_Record_getFile(sut));
    assert_equal(context->LINE, Logger_Record_getLine(sut));
    assert_equal(context->TIMESTAMP, Logger_Record_getTimestamp(sut));
    assert_equal(context->LEVEL, Logger_Record_getLevel(sut));
}

FeatureDefine(Setters) {
    Context_T context = traits_context;
    Logger_Record_T sut = context->sut;

    Logger_Record_setMessage(sut, "MyAmazingMessage");
    assert_string_equal("MyAmazingMessage", Logger_Record_getMessage(sut));

    Logger_Record_setLoggerName(sut, "MyAmazingLoggerName");
    assert_string_equal("MyAmazingLoggerName", Logger_Record_getLoggerName(sut));

    Logger_Record_setFunction(sut, "MyAmazingFunction");
    assert_string_equal("MyAmazingFunction", Logger_Record_getFunction(sut));

    Logger_Record_setFile(sut, "MyAmazingFile");
    assert_string_equal("MyAmazingFile", Logger_Record_getFile(sut));

    Logger_Record_setLine(sut, 1234);
    assert_equal(1234, Logger_Record_getLine(sut));

    Logger_Record_setTimestamp(sut, 1234);
    assert_equal(1234, Logger_Record_getTimestamp(sut));

    for (Logger_Level_T level = LOGGER_LEVEL_DEBUG; level <= LOGGER_LEVEL_FATAL; level++) {
        Logger_Record_setLevel(sut, level);
        assert_equal(level, Logger_Record_getLevel(sut));
    }
}
