/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   July 19, 2017 
 */

#include <stdlib.h>
#include "logger.h"

/*
 *
 */
static Logger_T gLogger = NULL;
static Logger_Formatter_T gFormatter = NULL;
static Logger_Handler_T gStdoutHandler = NULL, gStderrHandler = NULL, gFileHandler = NULL, gRotatingHandler = NULL;
static void initializeLogging(void);
static void terminateLogging(void);

/*
 *
 */
int main() {
    atexit(terminateLogging);
    initializeLogging();

    Logger_logDebug(gLogger, "%s\n", "This should not appear");
    Logger_logNotice(gLogger, "%s\n", "This should appear only on stdout");
    Logger_logInfo(gLogger, "%s\n", "This should appear on stdout and on example.log.0");
    Logger_logWarning(gLogger, "%s\n", "This should also appear to example.log");
    Logger_logError(gLogger, "%s\n", "Same for this");
    Logger_logFatal(gLogger, "%s\n", "And this");

    return EXIT_SUCCESS;
}

/*
 *
 */
void initializeLogging(void) {
    const char *filePath = "example.log";
    /*
     * Setup formatter
     */
    gFormatter = Logger_Formatter_newSimpleFormatter();

    /*
     * Setup handlers
     */
    gStdoutHandler = Logger_Handler_newConsoleHandler(LOGGER_CONSOLE_STREAM_STDOUT, LOGGER_LEVEL_DEBUG, gFormatter);
    gStderrHandler = Logger_Handler_newConsoleHandler(LOGGER_CONSOLE_STREAM_STDERR, LOGGER_LEVEL_ERROR, gFormatter);
    gFileHandler = Logger_Handler_newFileHandler(filePath, LOGGER_LEVEL_WARNING, gFormatter);
    gRotatingHandler = Logger_Handler_newRotatingHandler(filePath, LOGGER_LEVEL_INFO, gFormatter, 256);

    /*
     * Setup logger
     */
    gLogger = Logger_new("Example Logger", LOGGER_LEVEL_NOTICE);
    Logger_addHandler(gLogger, gStdoutHandler);
    Logger_addHandler(gLogger, gStderrHandler);
    Logger_addHandler(gLogger, gFileHandler);
    Logger_addHandler(gLogger, gRotatingHandler);
}

void terminateLogging(void) {
    /*
     * Terminate logger
     */
    Logger_delete(&gLogger);

    /*
     * Terminate handlers
     */
    Logger_Handler_delete(&gStderrHandler);
    Logger_Handler_delete(&gStdoutHandler);
    Logger_Handler_delete(&gFileHandler);
    Logger_Handler_delete(&gRotatingHandler);

    /*
     * Terminate formatter
     */
    Logger_Formatter_delete(&gFormatter);
}
