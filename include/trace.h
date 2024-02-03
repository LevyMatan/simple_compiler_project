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
#include <stdio.h>

/**
 * @brief 
 * 
 */
enum trace_level
{
    TRACE_LEVEL_DEBUG,
    TRACE_LEVEL_INFO
};

void set_trace_level(enum trace_level level);
void trace(enum trace_level level, const char *fmt, ...);

#define TRACE(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)

#endif // TRACE_H