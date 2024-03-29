#include <stdio.h>

#include "compiler/compiler.h"
#include "trace.h"

int main(void) {
    trace_status_e status = init_tracer("/workspaces/simple_compiler_project/trace_db.csv");
    handle_tracer_status(status);

    int res = compile_file("/workspaces/simple_compiler_project/input_files/test.c", NULL, 0);
    if (COMPILER_FILE_COMPILED_OK == res) {
        printf("File compiled successfully!\n");
    } else if (COMPILER_FAILED_WITH_ERRORS == res) {
        printf("File failed to compile!\n");
    } else {
        printf("Unknown error!\n");
    }

    // lex_process_t *p_lex_process = tokens_build_for_string("int main() { return 0; }");
    return 0;
}