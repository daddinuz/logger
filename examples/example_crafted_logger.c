/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   July 19, 2017 
 */

#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include "logger_builtin_handlers.h"
#include "logger_builtin_formatters.h"

/*
 *
 */
static Logger_T gLogger = NULL;

static void initializeLogging(void);
static void terminateLogging(void);

/*
 *
 */
int main() {
    initializeLogging();
    atexit(terminateLogging);

    Logger_logDebug(gLogger, "%s", "This should not appear");
    Logger_logNotice(gLogger, "%s", "This should appear on stdout and memory_file_handler.log");
    Logger_logInfo(gLogger, "%s", "This should also appear to rotating_file_handler.log.0");
    Logger_logWarning(gLogger, "%s", "This should also appear to file_handler.log");
    Logger_logError(gLogger, "%s", "Same for this");
    Logger_logFatal(gLogger, "%s", "And this");

    return EXIT_SUCCESS;
}

/*
 *
 */
void initializeLogging(void) {
    /*
     * Setup logger
     */
    gLogger = Logger_new("CraftedLogger", LOGGER_LEVEL_NOTICE);
    if (!gLogger) {
        fprintf(stderr, "At %s:%d\n%s\n", __FILE__, __LINE__, Logger_Err_gerString(LOGGER_ERR_OUT_OF_MEMORY));
        exit(EXIT_FAILURE);
    }

    /*
     * Setup formatter
     */
    Logger_Formatter_T formatter = Logger_Formatter_newSimpleFormatter();
    if (!formatter) {
        fprintf(stderr, "At %s:%d\n%s\n", __FILE__, __LINE__, Logger_Err_gerString(LOGGER_ERR_OUT_OF_MEMORY));
        exit(EXIT_FAILURE);
    }

    /*
     * Setup handlers
     */
    Logger_Handler_Result_T handlers[] = {
            Logger_Handler_newConsoleHandler(LOGGER_LEVEL_DEBUG, formatter, LOGGER_OSTREAM_STDOUT),
            Logger_Handler_newConsoleHandler(LOGGER_LEVEL_ERROR, formatter, LOGGER_OSTREAM_STDERR),
            Logger_Handler_newFileHandler(LOGGER_LEVEL_WARNING, formatter, "file_handler.log"),
            Logger_Handler_newRotatingFileHandler(LOGGER_LEVEL_INFO, formatter, "rotating_file_handler.log", 256),
            Logger_Handler_newMemoryFileHandler(LOGGER_LEVEL_DEBUG, formatter, "memory_file_handler.log", 512),
    };
    for (size_t i = 0; i < sizeof(handlers) / sizeof(handlers[0]); i++) {
        const Logger_Handler_Result_T r = handlers[i];
        if (LOGGER_ERR_OK != r.err) {
            fprintf(stderr, "At %s:%d\n%s\n", __FILE__, __LINE__, Logger_Err_gerString(r.err));
            exit(EXIT_FAILURE);
        }
        Logger_addHandler(gLogger, r.handler);
    }
}

void terminateLogging(void) {
    bool formatterNotYetDeleted = true;
    for (Logger_Handler_T handler = Logger_popHandler(gLogger); handler; handler = Logger_popHandler(gLogger)) {
        if (formatterNotYetDeleted) {
            /*
             * Terminate formatter
             */
            Logger_Formatter_T formatter = Logger_Handler_getFormatter(handler);
            Logger_Formatter_delete(&formatter);
            formatterNotYetDeleted = false;
        }

        /*
         * Terminate handlers
         */
        Logger_Handler_delete(&handler);
    }

    /*
     * Terminate logger
     */
    Logger_delete(&gLogger);
}
