#include "trace.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/**
 * @brief 
 * 
 * @param level 
 * @param file 
 * @param func 
 * @param line 
 * @param fmt 
 * @param ... 
 */
void fw_log(enum trace_level level, const char *file, const char *func, int line, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    switch (level)
    {
    case TRACE_LEVEL_DEBUG:
        printf("[DEBUG] ");
        break;
    case TRACE_LEVEL_INFO:
        printf("[INFO] ");
        break;
    case TRACE_LEVEL_WARNING:
        printf("[WARNING] ");
        break;
    case TRACE_LEVEL_ERROR:
        printf("[ERROR] ");
        break;
    default:
        printf("[UNKNOWN] ");
        break;
    }
    printf("%s:%d:%s: ", file, line, func);
    vprintf(fmt, args);
    va_end(args);
}

/**
 * @brief remove the path to the repo and show only the relative file from the workspace
 * 
 * @param path 
 * @return const char* 
 */
const char* strip_path(const char* path) {
    const char* repo_name = "/simple_compiler_project/";
    const char* found = strstr(path, repo_name);
    if (found) {
        return found + strlen(repo_name);
    }
    return path;
}
