/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 18, 2017
 */

#include <stdlib.h>
#include "logger_builtin_loggers.h"

/*
 *
 */
int main() {
    Logger_T gLogger = Logger_newRotatingFileLogger("RotatingFileLogger", LOGGER_LEVEL_DEBUG, "example_rotating_file_logger.log", 256);

    Logger_logDebug(gLogger, "%s", "Debug log message");
    Logger_logNotice(gLogger, "%s", "Notice log message");
    Logger_logInfo(gLogger, "%s", "Info log message");
    Logger_logWarning(gLogger, "%s", "Warning log message");
    Logger_logError(gLogger, "%s", "Error log message");
    Logger_logFatal(gLogger, "%s", "Fatal log message");

    Logger_deepDelete(&gLogger);
    return EXIT_SUCCESS;
}
