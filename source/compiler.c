#include "compiler/compiler.h"
#include "trace.h"

int compile_file(const char *filename, const char *out_filename, int flags)
{
    FW_LOG_ENTERED_FUNCTION();
    // struct compile_process *cp = compile_process_create(filename, out_filename, flags);
    // if (cp == NULL)
    // {
    //     return COMPILER_FAILED_WITH_ERRORS;
    // }

    // int status = compile_process_run(cp);
    // compile_process_destroy(cp);

    // return status;
    printf("Compiling file %s to %s with flags %d\n", filename, out_filename, flags);
    return 0;
}