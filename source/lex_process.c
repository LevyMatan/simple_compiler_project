#include <stdlib.h>

#include "compiler/compiler.h"
#include "trace.h"
#include "utils/vector.h"

void debug_print_compile_process(compile_process_t *p_process) {
    FW_LOG_DEBUG("p_process->cfile.fp = %p\n", p_process->cfile.fp);
    FW_LOG_DEBUG("p_process->cfile.abs_path = %s\n", p_process->cfile.abs_path);
    FW_LOG_DEBUG("p_process->s_pos = ", p_process->s_pos);
    debug_print_pos_struct(&(p_process->s_pos));
}

lex_process_t *lex_process_create(compile_process_t *p_s_compiler,
                                  lex_process_function_t *p_lex_function, void *p_private) {
    debug_print_compile_process(p_s_compiler);
    lex_process_t *p_lex_process = calloc(1, sizeof(lex_process_t));
    p_lex_process->p_s_compiler = p_s_compiler;
    p_lex_process->function = p_lex_function;
    p_lex_process->p_s_token_vec = vector_create(sizeof(token_t));
    p_lex_process->p_private = p_private;
    p_lex_process->s_pos.line = 1;
    p_lex_process->s_pos.col = 0;
    return p_lex_process;
}

void lex_process_free(lex_process_t *p_lex_process) {
    vector_free(p_lex_process->p_s_token_vec);
    free(p_lex_process);
}

void *lex_process_private(lex_process_t *p_lex_process) { return p_lex_process->p_private; }

struct vector *lex_process_token_vec(lex_process_t *p_lex_process) {
    return p_lex_process->p_s_token_vec;
}
