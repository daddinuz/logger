/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 08, 2017
 */

#include "traits/traits.h"
#include "traits-unit/traits-unit.h"
#include "logger_builtin_formatters.h"
#include "logger_builtin_handlers.h"
#include "logger.h"

/*
 * Define context
 */
typedef struct Context_T {
    char *NAME;
    Logger_Level_T LEVEL;
    Logger_Formatter_T FORMATTER;
    Logger_Handler_T HANDLER1;
    Logger_Handler_T HANDLER2;
    Logger_T sut;
} *Context_T;

/*
 * Declare setups
 */
SetupDeclare(SetupLogger);

/*
 * Declare teardowns
 */
TeardownDeclare(TeardownLogger);

/*
 * Declare fixtures
 */
FixtureDeclare(FixtureLogger);

/*
 * Declare features
 */
FeatureDeclare(NewAndDelete);
FeatureDeclare(Getters);
FeatureDeclare(Setters);
FeatureDeclare(ManageHandlers);

/*
 * Describe the test case
 */
Describe("Logger",
         Trait(
                 "Basic",
                 Run(NewAndDelete, FixtureLogger),
                 Run(Getters, FixtureLogger),
                 Run(Setters, FixtureLogger),
                 Run(ManageHandlers, FixtureLogger)
         )
)

/*
 * Define setups
 */
SetupDefine(SetupLogger) {
    Context_T context = malloc(sizeof(*context));
    assert_not_null(context);

    context->NAME = strdup("EXPECTED_LOGGER_NAME");
    assert_not_null(context->NAME);

    context->LEVEL = LOGGER_LEVEL_DEBUG;

    context->FORMATTER = Logger_Formatter_newSimpleFormatter();
    assert_not_null(context->FORMATTER);

    context->HANDLER1 = Logger_Handler_newConsoleHandler(LOGGER_LEVEL_DEBUG, context->FORMATTER, LOGGER_OSTREAM_STDOUT).handler;
    assert_not_null(context->HANDLER1);

    context->HANDLER2 = Logger_Handler_newConsoleHandler(LOGGER_LEVEL_ERROR, context->FORMATTER, LOGGER_OSTREAM_STDERR).handler;
    assert_not_null(context->HANDLER2);

    context->sut = Logger_new(context->NAME, context->LEVEL);
    assert_not_null(context->sut);

    return context;
}

/*
 * Define teardowns
 */
TeardownDefine(TeardownLogger) {
    assert_not_null(traits_context);
    Context_T context = traits_context;
    assert_not_null(context->sut);
    assert_not_null(context->NAME);
    assert_not_null(context->FORMATTER);
    assert_not_null(context->HANDLER1);
    assert_not_null(context->HANDLER2);

    Logger_Handler_delete(&context->HANDLER2);
    assert_null(context->HANDLER2);

    Logger_Handler_delete(&context->HANDLER1);
    assert_null(context->HANDLER1);

    Logger_Formatter_delete(&context->FORMATTER);
    assert_null(context->FORMATTER);

    free(context->NAME);

    Logger_delete(&context->sut);
    assert_null(context->sut);

    free(context);
}

/*
 * Define fixtures
 */
FixtureDefine(FixtureLogger, SetupLogger, TeardownLogger);

/*
 * Define features
 */
FeatureDefine(NewAndDelete) {
    (void) traits_context;
    /* Nothing to do */
}

FeatureDefine(Getters) {
    Context_T context = traits_context;
    Logger_T sut = context->sut;

    assert_string_equal(context->NAME, Logger_getName(sut));
    assert_equal(context->LEVEL, Logger_getLevel(sut));
    for (Logger_Level_T probeLevel = LOGGER_LEVEL_DEBUG; probeLevel <= LOGGER_LEVEL_FATAL; probeLevel++) {
        assert_equal(probeLevel >= context->LEVEL, Logger_isLoggable(sut, probeLevel));
    }
}

FeatureDefine(Setters) {
    Context_T context = traits_context;
    Logger_T sut = context->sut;

    Logger_setName(sut, "NEW_EXPECTED_NAME");
    assert_string_equal("NEW_EXPECTED_NAME", Logger_getName(sut));

    for (Logger_Level_T EXPECTED_LEVEL = LOGGER_LEVEL_DEBUG; EXPECTED_LEVEL <= LOGGER_LEVEL_FATAL; EXPECTED_LEVEL++) {
        Logger_setLevel(sut, EXPECTED_LEVEL);
        assert_equal(EXPECTED_LEVEL, Logger_getLevel(sut));
        for (Logger_Level_T probeLevel = LOGGER_LEVEL_DEBUG; probeLevel <= LOGGER_LEVEL_FATAL; probeLevel++) {
            assert_equal(probeLevel >= EXPECTED_LEVEL, Logger_isLoggable(sut, probeLevel));
        }
    }
}

FeatureDefine(ManageHandlers) {
    Context_T context = traits_context;
    Logger_T sut = context->sut;

    assert_null(Logger_popHandler(sut));

    Logger_addHandler(sut, context->HANDLER1);
    assert_equal(context->HANDLER1, Logger_popHandler(sut));
    assert_not_null(context->HANDLER1);
    assert_null(Logger_popHandler(sut));

    Logger_addHandler(sut, context->HANDLER1);
    Logger_addHandler(sut, context->HANDLER2);
    assert_equal(context->HANDLER2, Logger_popHandler(sut));
    assert_not_null(context->HANDLER2);
    assert_equal(context->HANDLER1, Logger_popHandler(sut));
    assert_not_null(context->HANDLER1);
    assert_null(Logger_popHandler(sut));

    Logger_addHandler(sut, context->HANDLER1);
    assert_equal(context->HANDLER1, Logger_removeHandler(sut, context->HANDLER1));
    assert_not_null(context->HANDLER1);
    assert_null(Logger_removeHandler(sut, context->HANDLER1));

    Logger_addHandler(sut, context->HANDLER1);
    Logger_addHandler(sut, context->HANDLER2);
    assert_equal(context->HANDLER1, Logger_removeHandler(sut, context->HANDLER1));
    assert_not_null(context->HANDLER1);
    assert_equal(context->HANDLER2, Logger_removeHandler(sut, context->HANDLER2));
    assert_not_null(context->HANDLER2);
    assert_null(Logger_removeHandler(sut, context->HANDLER1));
    assert_null(Logger_removeHandler(sut, context->HANDLER2));
}
