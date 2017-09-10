/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   September 02, 2017 
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include "traits-unit.h"

#if defined(__GNUC__) || defined(__clang__)
#   define TRAITS_UNIT_ATTRIBUTE_FORMAT(x, y)           __attribute__((__format__(__printf__, x, y)))
#   define TRAITS_UNIT_ATTRIBUTE_FORMAT_NORETURN(x, y)  __attribute__((__format__(__printf__, x, y), __noreturn__))
#else
#   define TRAITS_UNIT_ATTRIBUTE_FORMAT(x, y)
#   define TRAITS_UNIT_ATTRIBUTE_FORMAT_NORETURN(x, y)
#endif

/*
 * Some constants
 */
#define TRAITS_UNIT_OUTPUT_STREAM                       stdout
#define TRAITS_UNIT_BUFFER_CAPACITY                     (2 << 8)
#define TRAITS_UNIT_INDENTATION_STEP                    2
#define TRAITS_UNIT_INDENTATION_START                   0

/*
 * Forward declare traits subject (this should come from the test file)
 */
extern traits_unit_subject_t traits_unit_subject;

/*
 * Implements default fixtures from header file
 */
SetupDefine(DefaultSetup) {
    return NULL;
}

TeardownDefine(DefaultTeardown) {
    (void) traits_context;
}

FixtureDefine(DefaultFixture, DefaultSetup, DefaultTeardown);

/*
 * Internal declarations
 */
static traits_unit_feature_t *global_feature = NULL;
static void *global_context = NULL;

static void *traits_unit_shared_malloc(size_t size);
static void traits_unit_shared_free(void *address, size_t size);

typedef struct traits_unit_buffer_t {
    size_t _index;
    size_t _capacity;
    char *_content;
} traits_unit_buffer_t;

static traits_unit_buffer_t *traits_unit_buffer_new(size_t capacity);

static void traits_unit_buffer_read(traits_unit_buffer_t *buffer, int fd);

static char *traits_unit_buffer_get(traits_unit_buffer_t *buffer);

static void traits_unit_buffer_clear(traits_unit_buffer_t *buffer);

static void traits_unit_buffer_delete(traits_unit_buffer_t **buffer);

static void _traits_unit_panic(size_t line, const char *file, const char *fmt, ...)
TRAITS_UNIT_ATTRIBUTE_FORMAT_NORETURN(3, 4);

static void traits_unit_print(size_t indentation_level, const char *fmt, ...)
TRAITS_UNIT_ATTRIBUTE_FORMAT(2, 3);

static void _traits_unit_teardown(void);

static void traits_unit_teardown_on_exit(traits_unit_feature_t *feature, void *context);

typedef struct traits_unit_trait_result_t {
    size_t succeed;
    size_t skipped;
    size_t failed;
    size_t todo;
    size_t all;
} traits_unit_trait_result_t;

static traits_unit_trait_result_t
traits_unit_run_trait(size_t indentation_level, traits_unit_trait_t *trait, traits_unit_buffer_t *buffer);

typedef enum traits_unit_feature_result_t {
    TRAITS_UNIT_FEATURE_RESULT_SUCCEED,
    TRAITS_UNIT_FEATURE_RESULT_SKIPPED,
    TRAITS_UNIT_FEATURE_RESULT_FAILED,
    TRAITS_UNIT_FEATURE_RESULT_TODO,
} traits_unit_feature_result_t;

static int _traits_unit_fork_and_run_feature(traits_unit_feature_t *feature, traits_unit_buffer_t *buffer);

static traits_unit_feature_result_t
traits_unit_run_feature(size_t indentation_level, traits_unit_feature_t *feature, traits_unit_buffer_t *buffer);

static void
traits_unit_report(size_t indentation_level, size_t succeed, size_t skipped, size_t failed, size_t todo, size_t all);

/*
 * Macro helpers
 */
#define traits_unit_panic(fmt, ...)                 _traits_unit_panic(__LINE__, __FILE__, fmt, __VA_ARGS__)
#define traits_unit_newline()                       traits_unit_print(0, "\n")

/*
 * Implements main
 */
int main(int argc, char *argv[]) {
    bool loaded = true;
    traits_unit_buffer_t *buffer = NULL;
    traits_unit_trait_t *traits_list[__TRAITS_UNIT_MAX_TRAITS] = {0};
    size_t counter_succeed = 0, counter_skipped = 0, counter_failed = 0, counter_todo = 0, counter_all = 0;
    size_t indentation_level = TRAITS_UNIT_INDENTATION_START;

    /* Load traits_list */
    if (argc > 1) {
        /* Specific traits must be run */
        if (argc > __TRAITS_UNIT_MAX_TRAITS) {
            /* Too many traits has been specified, not able to load traits_list */
            loaded = false;
            traits_unit_print(indentation_level, "Too many traits specified\n");
        } else {
            /* Search for the specified traits and load them into traits_list */
            size_t index = 0;
            traits_unit_trait_t *trait = NULL;
            for (int x = 1; x < argc; x++) {
                bool found = false;
                for (size_t y = 0;
                     (trait = &traits_unit_subject.traits[y]) && trait->trait_name && trait->features; y++) {
                    if (0 == strcmp(trait->trait_name, argv[x])) {
                        found = true;
                        traits_list[index++] = trait;
                    }
                }
                if (!found) {
                    /* No such trait found, not able to load traits_list */
                    loaded = false;
                    traits_unit_print(indentation_level, "Unknown trait: `%s`\n", argv[x]);
                    break;
                }
            }
        }
    } else {
        /* Load all traits present in traits_subject */
        for (size_t i = 0; i < __TRAITS_UNIT_MAX_TRAITS; i++) {
            traits_list[i] = &traits_unit_subject.traits[i];
        }
    }

    if (loaded) {
        /* Run features of traits in traits_list */
        traits_unit_trait_t *trait = NULL;
        buffer = traits_unit_buffer_new(TRAITS_UNIT_BUFFER_CAPACITY);
        traits_unit_print(indentation_level, "Describing: %s\n", traits_unit_subject.subject);
        indentation_level += TRAITS_UNIT_INDENTATION_STEP;
        for (size_t i = 0; (trait = traits_list[i]) && trait->trait_name && trait->features; i++) {
            traits_unit_trait_result_t trait_result = traits_unit_run_trait(indentation_level, trait, buffer);
            counter_succeed += trait_result.succeed;
            counter_skipped += trait_result.skipped;
            counter_failed += trait_result.failed;
            counter_todo += trait_result.todo;
            counter_all += trait_result.all;
        }
        indentation_level -= TRAITS_UNIT_INDENTATION_STEP;
        traits_unit_report(indentation_level, counter_succeed, counter_skipped, counter_failed, counter_todo, counter_all);
        traits_unit_buffer_delete(&buffer);
    }

    return (loaded && (0 == counter_failed)) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*
 * Internal implementations
 */
void *traits_unit_shared_malloc(size_t size) {
    /* Our memory will be readable and writable */
    int protection = PROT_READ | PROT_WRITE;

    /*
     * The buffer will be shared (meaning other processes can access it), but
     * anonymous (meaning third-party processes cannot obtain an address for it),
     * so only this process and its children will be able to use it.
     */
    int visibility = MAP_ANONYMOUS | MAP_SHARED;

    /* Perform memory mapping */
    void *memory = mmap(NULL, size, protection, visibility, 0, 0);
    if (!memory) {
        traits_unit_panic("%s\n", "Unable to map memory");
    }

    return memory;
}

void traits_unit_shared_free(void *address, size_t size) {
    if (0 != munmap(address, size)) {
        traits_unit_panic("%s\n", "Unable to unmap memory");
    }
}

traits_unit_buffer_t *traits_unit_buffer_new(size_t capacity) {
    traits_unit_buffer_t *self = traits_unit_shared_malloc(sizeof(*self));
    if (!self) {
        traits_unit_panic("%s\n", "Out of memory.");
    }
    self->_content = traits_unit_shared_malloc(capacity + 1);
    if (!self->_content) {
        traits_unit_panic("%s\n", "Out of memory.");
    }
    self->_index = 0;
    self->_capacity = capacity;
    self->_content[capacity] = 0;
    return self;
}

void traits_unit_buffer_read(traits_unit_buffer_t *buffer, int fd) {
    assert(buffer);
    FILE *stream = fdopen(fd, "r");

    /* Open fd */
    if (!stream) {
        traits_unit_panic("%s\n", "Unable to open file.");
    }

    /* Read from fd and write to buffer */
    buffer->_index += fread(
            buffer->_content + buffer->_index,
            sizeof(buffer->_content[0]),
            buffer->_capacity - buffer->_index,
            stream
    );

    /* Close the stream */
    fclose(stream);
}

char *traits_unit_buffer_get(traits_unit_buffer_t *buffer) {
    assert(buffer);
    return buffer->_content;
}

void traits_unit_buffer_clear(traits_unit_buffer_t *buffer) {
    assert(buffer);
    buffer->_index = 0;
    buffer->_content[0] = 0;
}

void traits_unit_buffer_delete(traits_unit_buffer_t **buffer) {
    assert(buffer && *buffer);
    traits_unit_shared_free((*buffer)->_content, (*buffer)->_capacity + 1);
    traits_unit_shared_free(*buffer, sizeof(*buffer));
    *buffer = NULL;
}

void _traits_unit_panic(size_t line, const char *file, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "\nAt %s:%zu\nError: %s\n", file, line, strerror(errno));
    vfprintf(stderr, fmt, args);
    fputs("", stderr);
    va_end(args);
    exit(-1);
}

void traits_unit_print(size_t indentation_level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(TRAITS_UNIT_OUTPUT_STREAM, "%*s", (int) indentation_level, "");
    vfprintf(TRAITS_UNIT_OUTPUT_STREAM, fmt, args);
    va_end(args);
}

void _traits_unit_teardown(void) {
    assert(global_feature);
    global_feature->fixture->teardown(global_context);
    global_context = NULL;
    global_feature = NULL;
}

void traits_unit_teardown_on_exit(traits_unit_feature_t *feature, void *context) {
    assert(feature);
    assert(NULL == global_feature);
    assert(NULL == global_context);
    global_feature = feature;
    global_context = context;
    atexit(_traits_unit_teardown);
}

traits_unit_trait_result_t
traits_unit_run_trait(size_t indentation_level, traits_unit_trait_t *trait, traits_unit_buffer_t *buffer) {

    traits_unit_trait_result_t trait_result = {0};
    traits_unit_feature_t *feature = NULL;
    traits_unit_print(indentation_level, "Trait: %s\n", trait->trait_name);
    indentation_level += TRAITS_UNIT_INDENTATION_STEP;
    for (size_t i = 0; (feature = &trait->features[i]) && feature->feature && feature->feature_name; i++) {
        traits_unit_feature_result_t feature_result = traits_unit_run_feature(indentation_level, feature, buffer);
        switch (feature_result) {
            case TRAITS_UNIT_FEATURE_RESULT_SUCCEED: {
                trait_result.succeed++;
                break;
            }
            case TRAITS_UNIT_FEATURE_RESULT_SKIPPED: {
                trait_result.skipped++;
                break;
            }
            case TRAITS_UNIT_FEATURE_RESULT_FAILED: {
                trait_result.failed++;
                break;
            }
            case TRAITS_UNIT_FEATURE_RESULT_TODO: {
                trait_result.todo++;
                break;
            }
            default: {
                traits_unit_panic("Unexpected traits_unit_feature_result_t value: %d\n", feature_result);
            }
        }
        trait_result.all++;
    }
    return trait_result;
}

int _traits_unit_fork_and_run_feature(traits_unit_feature_t *feature, traits_unit_buffer_t *buffer) {
    pid_t pid;
    int fd, pipe_fd[2], pid_status;

    /* Flush TRAITS_UNIT_OUTPUT_STREAM */
    fflush(TRAITS_UNIT_OUTPUT_STREAM);

    /* Open a pipe */
    if (pipe(pipe_fd) < 0) {
        traits_unit_panic("%s\n", "Unable to open pipe.");
    }

    /* Fork the process */
    if ((pid = fork()) < 0) {
        traits_unit_panic("%s\n", "Unable to fork process.");
    }

    /* We are in the child process */
    if (0 == pid) {
        /* Close read end of pipe */
        close(pipe_fd[0]);

        /* Get write end of pipe */
        fd = pipe_fd[1];

        /* Redirect STDERR to pipe*/
        dup2(fd, STDERR_FILENO);

        /* Setup context */
        void *context = feature->fixture->setup();

        /* Teardown context on exit */
        traits_unit_teardown_on_exit(feature, context);

        /* Run feature */
        feature->feature(context);

        /* Close fd */
        close(fd);

        /* Exit normally */
        exit(EXIT_SUCCESS);
    }

    /* We are in the parent process */

    /* Wait for children */
    wait(&pid_status);

    /* Close write end of pipe */
    close(pipe_fd[1]);

    /* Get read end of pipe */
    fd = pipe_fd[0];

    /* Redirect the children output to the global buffer */
    traits_unit_buffer_read(buffer, fd);

    /* Close fd */
    close(fd);

    /* Flush TRAITS_UNIT_OUTPUT_STREAM */
    fflush(TRAITS_UNIT_OUTPUT_STREAM);
    return pid_status;
}

traits_unit_feature_result_t
traits_unit_run_feature(size_t indentation_level, traits_unit_feature_t *feature, traits_unit_buffer_t *buffer) {

    traits_unit_feature_result_t result = TRAITS_UNIT_FEATURE_RESULT_FAILED;
    traits_unit_print(indentation_level, "Feature: %s... ", feature->feature_name);
    switch (feature->action) {
        case TRAITS_UNIT_ACTION_RUN: {
            traits_unit_buffer_clear(buffer);
            const int exit_status = _traits_unit_fork_and_run_feature(feature, buffer);
            if (EXIT_SUCCESS == exit_status) {
                result = TRAITS_UNIT_FEATURE_RESULT_SUCCEED;
                traits_unit_print(0, "succeed\n");
            } else {
                result = TRAITS_UNIT_FEATURE_RESULT_FAILED;
                if (!WIFEXITED(exit_status)) {
                    if (WIFSIGNALED(exit_status)) {
                        traits_unit_print(0, "(terminated by signal %d - %s) ", WTERMSIG(exit_status), strsignal(WTERMSIG(exit_status)));
                    } else {
                        traits_unit_print(0, "(terminated abnormally) ");
                    }
                }
                traits_unit_print(0, "failed\n\n%s\n", traits_unit_buffer_get(buffer));
            }
            break;
        }
        case TRAITS_UNIT_ACTION_SKIP: {
            result = TRAITS_UNIT_FEATURE_RESULT_SKIPPED;
            traits_unit_print(0, "skipped\n");
            break;
        }
        case TRAITS_UNIT_ACTION_TODO: {
            result = TRAITS_UNIT_FEATURE_RESULT_TODO;
            traits_unit_print(0, "todo\n");
            break;
        }
        default: {
            traits_unit_panic("Unexpected traits_unit_action_t value: %d\n", feature->action);
        }
    }
    return result;
}

void
traits_unit_report(size_t indentation_level, size_t succeed, size_t skipped, size_t failed, size_t todo, size_t all) {
    traits_unit_newline();
    traits_unit_print(indentation_level, "Succeed: %zu\n", succeed);
    traits_unit_print(indentation_level, "Skipped: %zu\n", skipped);
    traits_unit_print(indentation_level, " Failed: %zu\n", failed);
    traits_unit_print(indentation_level, "   Todo: %zu\n", todo);
    traits_unit_print(indentation_level, "    All: %zu\n", all);
}
