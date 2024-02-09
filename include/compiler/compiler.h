#ifdef __cplusplus
extern "C" {
#endif

#ifndef MATAN_COMPILER_H
#    define MATAN_COMPILER_H
#    include <stdbool.h>
#    include <stdio.h>

typedef struct pos_s {
    int line;
    int col;
    const char *filename;
} pos_t;

typedef enum LEXICAL_ANALSYS_STATAUS {
    LEXICAL_ANALSYS_ALL_OK,
    LEXICAL_ANALSYS_FAILED_WITH_ERRORS,
} LEXICAL_ANALSYS_STATAUS_E;

typedef enum token_type {
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_SYMBOL,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_COMMENT,
    TOKEN_TYPE_NEWLINE,
    TOKEN_TYPE_EOF,
} token_type_e;

typedef struct token_s {
    token_type_e type;
    int flags;
    pos_t s_pos;
    union {
        char cval;
        const char *sval;
        unsigned int ival;
        unsigned long lval;
        unsigned long long llval;
        void *any;
    };

    /*
    True if there is a whitespace between the token and the next token.
    i.e.
    */
    bool whitespace;

    /**
     * All token inside a bracket will hold the point to the start of the bracket.
     */
    const char *between_brackets;

} token_t;

typedef enum number_type {
    NUMBER_TYPE_INT,
    NUMBER_TYPE_LONG,
    NUMBER_TYPE_LONG_LONG,
    NUMBER_TYPE_FLOAT,
} number_type_e;

enum {
    COMPILER_FILE_COMPILED_OK,
    COMPILER_FAILED_WITH_ERRORS,
};
typedef struct compile_process_s {
    // The flags in regards to how this file should be compiled.
    int flags;

    pos_t s_pos;
    struct compile_process_input_file {
        FILE *fp;
        const char *abs_path;

    } cfile;

    struct vector *p_s_token_vec;

    FILE *ofile;
} compile_process_t;

typedef struct lex_process_s lex_process_t;

typedef char (*LEX_PROCESS_NEXT_CHAR)(lex_process_t *p_lex_process);
typedef char (*LEX_PROCESS_PEEK_CHAR)(lex_process_t *p_lex_process);
typedef void (*LEX_PROCESS_PUSH_CHAR)(lex_process_t *p_lex_process, char c);
typedef struct lex_process_function_s {
    LEX_PROCESS_NEXT_CHAR next_char;
    LEX_PROCESS_PEEK_CHAR peek_char;
    LEX_PROCESS_PUSH_CHAR push_char;

} lex_process_function_t;

char compile_process_next_char(lex_process_t *p_lex_process);

char compile_process_peek_char(lex_process_t *p_lex_process);

void compile_process_push_char(lex_process_t *p_lex_process, char c);
token_t *read_next_token(void);
struct lex_process_s {
    pos_t s_pos;
    struct vector *p_s_token_vec;
    compile_process_t *p_s_compiler;

    /**
     * How many characters are in the current expression.
     */
    int current_expression_count;

    struct buffer *p_parantheses_buffer;
    lex_process_function_t *function;

    /**
     * Private data that the lexer does not understand.
     * But the pereson using the lexer does understand.
     */
    void *p_private;
};

bool lex_is_in_an_expression(void);
int compile_file(const char *filename, const char *out_filename, int flags);
compile_process_t *compile_process_create(const char *filename, const char *out_filename,
                                          int flags);

bool token_is_keyword(const token_t *p_token, const char *p_keyword);
void compiler_error(compile_process_t *p_process, const char *format, ...);
static token_t *token_make_symbol(void);
void compiler_warning(compile_process_t *p_process, const char *format, ...);

lex_process_t *lex_process_create(compile_process_t *p_s_compiler,
                                  lex_process_function_t *p_s_lex_functions, void *p_private);
void lex_process_free(lex_process_t *p_lex_process);
void *lex_process_private(lex_process_t *p_lex_process);
struct vector *lex_process_token_vec(lex_process_t *p_lex_process);
int lex(lex_process_t *p_lex_process);

/**
 * @brief Create a token vector from a string, using the lexer.
 *
 */
lex_process_t *tokens_build_for_string(compile_process_t *p_compiler, const char *str);

void debug_print_compile_process(compile_process_t *p_process);
void debug_print_pos_struct(pos_t *p_pos);
#endif  // MATAN_COMPILER_H

#ifdef __cplusplus
}
#endif
