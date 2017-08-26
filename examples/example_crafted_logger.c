/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   July 19, 2017 
 */

#include <stdlib.h>
#include "logger.h"
#include "logger_builtin_handlers.h"
#include "logger_builtin_formatters.h"

/*
 *
 */
static Logger_T gLogger = NULL;
static Logger_Formatter_T gFormatter = NULL;
static Logger_Handler_T gStdoutHandler = NULL;
static Logger_Handler_T gStderrHandler = NULL;
static Logger_Handler_T gFileHandler = NULL;
static Logger_Handler_T gRotatingFileHandler = NULL;
static Logger_Handler_T gMemoryFileHandler = NULL;

static void initializeLogging(void);
static void terminateLogging(void);

/*
 *
 */
int main() {
    atexit(terminateLogging);
    initializeLogging();

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
     * Setup formatter
     */
    gFormatter = Logger_Formatter_newSimpleFormatter();

    /*
     * Setup handlers
     */
    gStdoutHandler = Logger_Handler_newConsoleHandler(LOGGER_LEVEL_DEBUG, gFormatter, LOGGER_OSTREAM_STDOUT);
    gStderrHandler = Logger_Handler_newConsoleHandler(LOGGER_LEVEL_ERROR, gFormatter, LOGGER_OSTREAM_STDERR);
    gFileHandler = Logger_Handler_newFileHandler(LOGGER_LEVEL_WARNING, gFormatter, "file_handler.log");
    gRotatingFileHandler = Logger_Handler_newRotatingFileHandler(LOGGER_LEVEL_INFO, gFormatter, "rotating_file_handler.log", 256);
    gMemoryFileHandler = Logger_Handler_newMemoryFileHandler(LOGGER_LEVEL_DEBUG, gFormatter, "memory_file_handler.log", 512);

    /*
     * Setup logger
     */
    gLogger = Logger_new("CraftedLogger", LOGGER_LEVEL_NOTICE);
    Logger_addHandler(gLogger, gStdoutHandler);
    Logger_addHandler(gLogger, gStderrHandler);
    Logger_addHandler(gLogger, gFileHandler);
    Logger_addHandler(gLogger, gRotatingFileHandler);
    Logger_addHandler(gLogger, gMemoryFileHandler);
}

void terminateLogging(void) {
    /*
     * Terminate logger
     */
    Logger_delete(&gLogger);

    /*
     * Terminate handlers
     */
    Logger_Handler_delete(&gStdoutHandler);
    Logger_Handler_delete(&gStderrHandler);
    Logger_Handler_delete(&gFileHandler);
    Logger_Handler_delete(&gRotatingFileHandler);
    Logger_Handler_delete(&gMemoryFileHandler);

    /*
     * Terminate formatter
     */
    Logger_Formatter_delete(&gFormatter);
}
