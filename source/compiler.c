#include "compiler/compiler.h"

#include <stdarg.h>
#include <stdlib.h>

#include "trace.h"

lex_process_function_t compiler_lex_functions = {
    .next_char = compile_process_next_char,
    .peek_char = compile_process_peek_char,
    .push_char = compile_process_push_char,
};

void compiler_error(compile_process_t *p_process, const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "%s:%d:%d: error: ", p_process->cfile.abs_path, p_process->s_pos.line,
            p_process->s_pos.col);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(-1);
}

void compiler_warning(compile_process_t *p_process, const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "%s:%d:%d: warnining: ", p_process->cfile.abs_path, p_process->s_pos.line,
            p_process->s_pos.col);
    vfprintf(stderr, format, args);
    va_end(args);
}

int compile_file(const char *filename, const char *out_filename, int flags) {
    compile_process_t *p_process = compile_process_create(filename, out_filename, flags);
    if (!p_process) {
        FW_LOG_ERROR("Failed to create compile process!\n");
        return COMPILER_FAILED_WITH_ERRORS;
    }

    debug_print_compile_process(p_process);
    // Preform Lexical Analysis
    lex_process_t *p_lex_process = lex_process_create(p_process, &compiler_lex_functions, NULL);
    if (!p_lex_process) {
        FW_LOG_ERROR("Failed to create lex process!\n");
        return COMPILER_FAILED_WITH_ERRORS;
    }

    if (LEXICAL_ANALSYS_ALL_OK != lex(p_lex_process)) {
        FW_LOG_ERROR("Failed to lex file!\n");
        return COMPILER_FAILED_WITH_ERRORS;
    }

    p_process->p_s_token_vec = p_lex_process->p_s_token_vec;
    // Preform parsing

    // Preform code generation

    return COMPILER_FILE_COMPILED_OK;
}
