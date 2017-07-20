/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   July 19, 2017 
 */

#include <stdlib.h>
#include <errno.h>
#include "sds/sds.h"
#include "logger.h"

/*
 *
 */
static Logger_T gLogger = NULL;
static Logger_Formatter_T gFormatter = NULL;
static Logger_Handler_T gStdoutHandler = NULL, gStderrHandler = NULL;

static char *formatMessageCallback(Logger_Message_T message);
static void deleteMessageCallback(char **self);

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
    Logger_logInfo(gLogger, "%s\n", "This should also appear only stdout");
    Logger_logWarning(gLogger, "%s\n", "Same for this");
    Logger_logError(gLogger, "%s\n", "This should appear both on stdout and stderr");
    Logger_logFatal(gLogger, "%s\n", "This should also appear both on stdout and stderr");

    return 0;
}

/*
 *
 */
char *formatMessageCallback(Logger_Message_T message) {
    char time_string[32] = "";
    time_t timestamp = Logger_Message_getTimestamp(message);
    strftime(time_string, sizeof(time_string) / sizeof(time_string[0]), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
    sds result = sdscatprintf(
            sdsempty(),
            "%s [%s] %s %s:%zu:%s\n%s",
            Logger_Message_getLoggerName(message),
            Logger_Level_getName(Logger_Message_getLevel(message)),
            time_string,
            Logger_Message_getFile(message),
            Logger_Message_getLine(message),
            Logger_Message_getFunction(message),
            Logger_Message_getContent(message)
    );
    if (!result) {
        errno = ENOMEM;
    }
    return result;
}

void deleteMessageCallback(char **self) {
    sdsfree(*self);
    *self = NULL;
}

void initializeLogging(void) {
    /*
     * Setup formatter
     */
    gFormatter = Logger_Formatter_new(formatMessageCallback, deleteMessageCallback);

    /*
     * Setup handlers
     */
    gStdoutHandler = Logger_Handler_newStreamHandler(gFormatter, LOGGER_LEVEL_DEBUG, stdout);
    gStderrHandler = Logger_Handler_newStreamHandler(gFormatter, LOGGER_LEVEL_ERROR, stderr);

    /*
     * Setup logger
     */
    gLogger = Logger_new("ExampleLogger", LOGGER_LEVEL_NOTICE);
    Logger_addHandler(gLogger, gStdoutHandler);
    Logger_addHandler(gLogger, gStderrHandler);
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

    /*
     * Terminate formatter
     */
    Logger_Formatter_delete(&gFormatter);
}
