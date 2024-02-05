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
#pragma once

#include <stdio.h>

/**
 * @brief
 *
 */
enum trace_level { TRACE_LEVEL_DEBUG, TRACE_LEVEL_INFO, TRACE_LEVEL_WARNING, TRACE_LEVEL_ERROR };

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
void fw_log(enum trace_level level, const char *file, const char *func, int line, const char *fmt,
            ...);

/**
 * @brief remove the path to the repo and show only the relative file from the workspace
 *
 * @param path
 * @return const char*
 */
const char *strip_path(const char *path);
#define __FILENAME__ (strip_path(__FILE__))

#ifdef DEBUG_ENABLED
#  define FW_LOG_DEBUG(fmt, ...) \
    fw_log(TRACE_LEVEL_DEBUG, __FILENAME__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#else
#  define FW_LOG_DEBUG(fmt, ...)
#endif

#define FW_LOG_INFO(fmt, ...) \
  fw_log(TRACE_LEVEL_INFO, __FILENAME__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define FW_LOG_ERROR(fmt, ...) \
  fw_log(TRACE_LEVEL_ERROR, __FILENAME__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define FW_LOG_WARN(fmt, ...) \
  fw_log(TRACE_LEVEL_WARN, __FILENAME__, __func__, __LINE__, fmt, ##__VA_ARGS__)

#define FW_LOG_ENTERED_FUNCTION() FW_LOG_DEBUG("Entered function\n")

#endif  // TRACE_H
