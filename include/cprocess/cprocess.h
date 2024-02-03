#ifndef CPROCESS_H
#define CPROCESS_H
#include <stdio.h>

enum
{
    COMPILER_FILE_COMPILED_OK,
    COMPILER_FAILED_WITH_ERRORS,
};

typedef struct compile_process
{
    // The flags in regards to how this file should be compiled.
    int flags;

    struct compile_process_input_file
    {
        FILE *fp;
        const char *abs_path;

    } cfile;

    FILE *ofile;

    int pid;
    int status;
}compile_process_t;

compile_process_t *compile_process_create(const char *filename, const char *out_filename, int flags);

#endif // CPROCESS_H