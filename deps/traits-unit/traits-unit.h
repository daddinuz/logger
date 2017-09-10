/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   September 02, 2017 
 */

#include <stddef.h>

#ifndef __TRAITS_UNIT_H__
#define __TRAITS_UNIT_H__

#define TRAITS_UNIT                         "1.0.1"

#ifdef __cplusplus
extern "C" {
#endif

#define __TRAITS_UNIT_IMPL_CAT(x, ...)      x ## __VA_ARGS__
#define __TRAITS_UNIT_CAT(x, ...)           __TRAITS_UNIT_IMPL_CAT(x, __VA_ARGS__)

#define __TRAITS_UNIT_IMPL_TO_STRING(x)     #x
#define __TRAITS_UNIT_TO_STRING(x)          __TRAITS_UNIT_IMPL_TO_STRING(x)

#define __TRAITS_UNIT_SETUP_ID(Name)        __TRAITS_UNIT_CAT(traits_unit_user_setup_, Name)
#define __TRAITS_UNIT_TEARDOWN_ID(Name)     __TRAITS_UNIT_CAT(traits_unit_user_teardown_, Name)
#define __TRAITS_UNIT_FIXTURE_ID(Name)      __TRAITS_UNIT_CAT(traits_unit_user_fixture_, Name)
#define __TRAITS_UNIT_FEATURE_ID(Name)      __TRAITS_UNIT_CAT(traits_unit_user_feature_, Name)

#define __TRAITS_UNIT_MAX_TRAITS            (32 + 1)
#define __TRAITS_UNIT_MAX_FEATURES          (128 + 1)


typedef void *traits_unit_setup_fn(void);
typedef void traits_unit_teardown_fn(void *);

typedef struct traits_unit_fixture_t {
    traits_unit_setup_fn *setup;
    traits_unit_teardown_fn *teardown;
} traits_unit_fixture_t;

typedef void traits_unit_feature_fn(void *traits_context);

typedef enum traits_unit_action_t {
    TRAITS_UNIT_ACTION_RUN,
    TRAITS_UNIT_ACTION_SKIP,
    TRAITS_UNIT_ACTION_TODO
} traits_unit_action_t;

typedef struct traits_unit_feature_t {
    traits_unit_action_t action;
    const char *feature_name;
    traits_unit_fixture_t *fixture;
    traits_unit_feature_fn *feature;
} traits_unit_feature_t;

typedef struct traits_unit_trait_t {
    const char *trait_name;
    traits_unit_feature_t features[__TRAITS_UNIT_MAX_FEATURES];
} traits_unit_trait_t;

typedef struct traits_unit_subject_t {
    const char *subject;
    traits_unit_trait_t traits[__TRAITS_UNIT_MAX_TRAITS];
} traits_unit_subject_t;

#define SetupDeclare(Name)                      \
    extern void *__TRAITS_UNIT_SETUP_ID(Name)(void)

#define SetupDefine(Name)                       \
    void *__TRAITS_UNIT_SETUP_ID(Name)(void)

#define TeardownDeclare(Name)                   \
    extern void __TRAITS_UNIT_TEARDOWN_ID(Name)(void *traits_context)

#define TeardownDefine(Name)                    \
    void __TRAITS_UNIT_TEARDOWN_ID(Name)(void *traits_context)

#define FixtureDeclare(Name)                    \
    extern traits_unit_fixture_t __TRAITS_UNIT_FIXTURE_ID(Name)

#define FixtureDefine(Name, Setup, Teardown)    \
    traits_unit_fixture_t __TRAITS_UNIT_FIXTURE_ID(Name) = {.setup=__TRAITS_UNIT_SETUP_ID(Setup), .teardown=__TRAITS_UNIT_TEARDOWN_ID(Teardown)}

#define FeatureDeclare(Name)                    \
    extern void __TRAITS_UNIT_FEATURE_ID(Name)(void *traits_context)

#define FeatureDefine(Name)                     \
    void __TRAITS_UNIT_FEATURE_ID(Name)(void *traits_context)

#define Describe(Subject, ...)                  \
    traits_unit_subject_t traits_unit_subject = {.subject=(Subject), .traits={__VA_ARGS__, {0}}};

#define Trait(Name, ...)                        \
    {.trait_name=(Name), .features={__VA_ARGS__, {0}}}

#define Run(...)                                \
    __Run(__VA_ARGS__, DefaultFixture, DefaultFixture)

#define __Run(Name, Fixture, ...)               \
    {.feature_name=__TRAITS_UNIT_TO_STRING(Name), .feature=__TRAITS_UNIT_FEATURE_ID(Name), .fixture=&__TRAITS_UNIT_FIXTURE_ID(Fixture), .action=TRAITS_UNIT_ACTION_RUN}

#define Skip(...)                               \
    __Skip(__VA_ARGS__, DefaultFixture, DefaultFixture)

#define __Skip(Name, Fixture, ...)              \
    {.feature_name=__TRAITS_UNIT_TO_STRING(Name), .feature=__TRAITS_UNIT_FEATURE_ID(Name), .fixture=&__TRAITS_UNIT_FIXTURE_ID(Fixture), .action=TRAITS_UNIT_ACTION_SKIP}

#define Todo(...)                               \
    __Todo(__VA_ARGS__, DefaultFixture, DefaultFixture)

#define __Todo(Name, Fixture, ...)              \
    {.feature_name=__TRAITS_UNIT_TO_STRING(Name), .feature=__TRAITS_UNIT_FEATURE_ID(Name), .fixture=&__TRAITS_UNIT_FIXTURE_ID(Fixture), .action=TRAITS_UNIT_ACTION_TODO}

/*
 * Declare default fixtures
 */
SetupDeclare(DefaultSetup);
TeardownDeclare(DefaultTeardown);
FixtureDeclare(DefaultFixture);

/*
 * Declare main in order to force definition by traits-unit
 */
extern int main(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif /* __TRAITS_UNIT_H__ */
