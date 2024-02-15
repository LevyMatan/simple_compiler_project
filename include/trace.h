#ifndef TRACE_H
#define TRACE_H
/**
 * @file trace.h
 * @author Matan Levy (levymatanlevy@gmail.com)
 * @brief Supply developer with extensive logging control.
 * - Control the level of logging.
 * - Control in realtime of which function will print logs
 * - Control group of functions that will print logs by file name
 * @version 0.1
 * @date 2024-02-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdbool.h>
#include <stdio.h>

#include "uthash.h"
#include "utils/generated_enum_dict.h"

#define VAR_TO_IDX(x) (typename_to_idx(typename(x)))
#define ENUM_STRING(x) g_a_getters[VAR_TO_IDX(x)](&(x))

/**
 * @brief
 *
 */
typedef enum trace_level {
    TRACE_LEVEL_DEBUG,
    TRACE_LEVEL_INFO,
    TRACE_LEVEL_WARNING,
    TRACE_LEVEL_ERROR
} trace_level_e;

/**
 * @brief
 *
 */
typedef enum trace_status {
    TRACE_STATUS_OK,
    TRACE_STATUS_FAILED_TO_READ_CONF_FILE,
    TRACE_STATUS_FAILED_TO_CONVERT_FUNC_STRING,
    TRACE_STATUS_GENERAL_FAIL,
    TRACE_STATUS_DEBUG_DISABLED,
} trace_status_e;

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
void fw_log(trace_level_e level, const char *file, const char *func, int line, const char *fmt,
            ...);

/**
 * @brief
 *
 * @param status
 */
void handle_tracer_status(trace_status_e status);

/**
 * @brief
 *
 * @param func
 * @return true
 * @return false
 */
bool is_function_enabled(const char *func);

/**
 * @brief Convert a type name to index, in-order to reach the correct getter function
 *
 * @param type_name
 * @return int
 */
int typename_to_idx(const char *type_name);

typedef struct func_node {
    char func_name[1000];
    char file_name[1000];
    bool is_enabled;
    int idx;
    UT_hash_handle hh;
} func_node_t;

/**
 * @brief
 *
 * @param conf_file_path
 * @return trace_status_e
 */
trace_status_e init_tracer(const char *conf_file_path);

/**
 * @brief remove the path to the repo and show only the relative file from the workspace
 *
 * @param path
 * @return const char*
 */
const char *strip_path(const char *path);
#define __FILENAME__ (strip_path(__FILE__))

#ifdef DEBUG_ENABLED
#    define FW_LOG_DEBUG(fmt, ...)                                                               \
        {                                                                                        \
            if (is_function_enabled(__func__))                                                   \
                fw_log(TRACE_LEVEL_DEBUG, __FILENAME__, __func__, __LINE__, fmt, ##__VA_ARGS__); \
        }
#else
#    define FW_LOG_DEBUG(fmt, ...)
#endif

#define FW_LOG_INFO(fmt, ...) \
    fw_log(TRACE_LEVEL_INFO, __FILENAME__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define FW_LOG_ERROR(fmt, ...) \
    fw_log(TRACE_LEVEL_ERROR, __FILENAME__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define FW_LOG_WARN(fmt, ...) \
    fw_log(TRACE_LEVEL_WARN, __FILENAME__, __func__, __LINE__, fmt, ##__VA_ARGS__)

#define FW_LOG_ENTERED_FUNCTION() FW_LOG_DEBUG("Entered function: %s\n", __func__)

#endif  // TRACE_H
