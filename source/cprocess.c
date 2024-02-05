#include <stdio.h>
#include <stdlib.h>

#include "compiler/compiler.h"
#include "trace.h"

compile_process_t *compile_process_create(const char *filename, const char *out_filename,
                                          int flags) {
  FILE *file = fopen(filename, "r");

  if (!file) {
    FW_LOG_ERROR("File %s not found!\n", filename);
    return NULL;
  }

  FILE *out_file = NULL;
  if (out_filename) {
    out_file = fopen(out_filename, "w");
    if (!out_file) {
      FW_LOG_ERROR("File %s not found!\n", out_filename);
      return NULL;
    }
  }

  compile_process_t *p_process = calloc(1, sizeof(compile_process_t));
  p_process->cfile.fp = file;
  p_process->cfile.abs_path = filename;
  p_process->ofile = out_file;
  p_process->flags = flags;
  p_process->s_pos.line = 1;
  p_process->s_pos.col = 0;

  return p_process;
}

char compile_process_next_char(lex_process_t *p_lex_process) {
  compile_process_t *compiler = p_lex_process->p_s_compiler;
  compiler->s_pos.col++;
  char c = getc(compiler->cfile.fp);
  if (c == '\n') {
    compiler->s_pos.line++;
    compiler->s_pos.col = 1;
  }

  return c;
}

char compile_process_peek_char(lex_process_t *p_lex_process) {
  FW_LOG_ENTERED_FUNCTION();
  FW_LOG_DEBUG("p_lex_process = %p\n", p_lex_process);
  compile_process_t *compiler = p_lex_process->p_s_compiler;
  FW_LOG_DEBUG("compiler = %p\n", compiler);
  FW_LOG_DEBUG("compiler->cfile.fp = %p\n", compiler->cfile.fp);
  char c = getc(compiler->cfile.fp);
  FW_LOG_DEBUG("c = %c\n", c);
  ungetc(c, compiler->cfile.fp);
  FW_LOG_DEBUG("Done ungetc\n");
  return c;
}

void compile_process_push_char(lex_process_t *p_lex_process, char c) {
  compile_process_t *compiler = p_lex_process->p_s_compiler;
  ungetc(c, compiler->cfile.fp);
}