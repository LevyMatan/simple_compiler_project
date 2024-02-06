#include "trace.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

const char* g_a_log_level[] = {"[DEBUG]", "[INFO]", "[WARNING]", "[ERROR]"};

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
void fw_log(trace_level_e level, const char* file, const char* func, int line, const char* fmt,
            ...) {
    va_list args;
    va_start(args, fmt);
    printf("%s ", g_a_log_level[level]);
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
