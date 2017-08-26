/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   August 25, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include "logger_builtin_loggers.h"

/*
 *
 */
int main() {
    Logger_T gLogger = NULL;
    {
        Logger_Result_T result = Logger_newMemoryFileLogger("FileLogger", LOGGER_LEVEL_DEBUG, "example_memory_file_logger.log", 256);
        if (LOGGER_ERR_OK != result.err) {
            fprintf(stderr, "At %s:%d\n%s\n", __FILE__, __LINE__, Logger_Err_gerString(result.err));
            exit(EXIT_FAILURE);
        }
        gLogger = result.logger;
    }

    Logger_logDebug(gLogger, "%s", "Debug log message");
    Logger_logNotice(gLogger, "%s", "Notice log message");
    Logger_logInfo(gLogger, "%s", "Info log message");
    Logger_logWarning(gLogger, "%s", "Warning log message");
    Logger_logError(gLogger, "%s", "Error log message");
    Logger_logFatal(gLogger, "%s", "Fatal log message");

    Logger_deepDelete(&gLogger);
    return EXIT_SUCCESS;
}
