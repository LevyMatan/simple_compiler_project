#ifndef COMMON_H
#define COMMON_H

typedef struct pos_s
{
    int line;
    int col;
    const char *filename;
} pos_t;

#endif // COMMON_H