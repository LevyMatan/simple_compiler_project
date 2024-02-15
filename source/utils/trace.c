#include "trace.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *g_a_log_level[] = {"[DEBUG]", "[INFO]", "[WARNING]", "[ERROR]"};
func_node_t *g_p_func_node_table = NULL;

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
const char *strip_path(const char *path) {
    const char *repo_name = "/simple_compiler_project/";
    const char *found = strstr(path, repo_name);
    if (found) {
        return found + strlen(repo_name);
    }
    return path;
}

void add_function(char *func_name, int idx, bool is_enabled) {
    func_node_t *s;

    HASH_FIND_STR(g_p_func_node_table, func_name, s);  // Check if entry already exists
    if (s == NULL) {
        s = (func_node_t *)malloc(sizeof(func_node_t));
        strcpy(s->func_name, func_name);
        s->idx = idx;
        s->is_enabled = is_enabled;
        HASH_ADD_STR(g_p_func_node_table, func_name, s);  // Add it
    }
}

int get_index(char *func_name) {
    func_node_t *s;

    HASH_FIND_STR(g_p_func_node_table, func_name, s);  // Look it up in the hash table
    if (s == NULL) {
        return -1;  // Not found
    }
    return s->idx;
}

bool is_function_enabled(const char *func) {
    func_node_t *s;
    HASH_FIND_STR(g_p_func_node_table, func, s);
    if (s == NULL) {
        return false;
    }
    return s->is_enabled;
}

trace_status_e load_function_table(const char *csv_filename) {
    FILE *file = fopen(csv_filename, "r");
    if (!file) {
        printf("Could not open file %s\n", csv_filename);
        return TRACE_STATUS_FAILED_TO_READ_CONF_FILE;
    }

    char line[1024];
    bool is_header = true;
    while (fgets(line, 1024, file)) {
        if (__builtin_expect(is_header, 0)) {  // Skip the header line
            is_header = false;
            continue;
        }
        char *tmp = strdup(line);
        char *index_str = strtok(tmp, ",");
        char *func_name = strtok(NULL, ",");
        char *file = strtok(NULL, ",");  // We're not using this in the current function
        char *is_enabled_str = strtok(NULL, ",");
        int index = atoi(index_str);
        int is_enabled_int = atoi(is_enabled_str);
        bool is_enabled = is_enabled_int == 1 ? true : false;
        add_function(func_name, index, is_enabled);
        free(tmp);
    }
    fclose(file);
    return TRACE_STATUS_OK;
}

/**
 * @brief
 *
 * @param conf_file_path
 * @return trace_status_e
 */
trace_status_e init_tracer(const char *conf_file_path) {
#ifdef DEBUG_ENABLED
    trace_status_e e_status = load_function_table(conf_file_path);
    return e_status;
#else
    return TRACE_STATUS_DEBUG_DISABLED;
#endif
}

void handle_tracer_status(trace_status_e status) {
    switch (status) {
        case TRACE_STATUS_OK:
            break;
        case TRACE_STATUS_FAILED_TO_READ_CONF_FILE:
            printf("Failed to read conf file\n");
            break;
        case TRACE_STATUS_FAILED_TO_CONVERT_FUNC_STRING:
            printf("Failed to convert function string\n");
            break;
        case TRACE_STATUS_GENERAL_FAIL:
            printf("General failure\n");
            break;
        case TRACE_STATUS_DEBUG_DISABLED:
            printf("Debug is disabled\n");
            break;
        default:
            printf("Unknown error\n");
            break;
    }
}
