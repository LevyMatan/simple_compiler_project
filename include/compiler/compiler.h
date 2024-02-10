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

typedef enum parser_status {
    PARSE_ALL_OK,
    PARSE_FAILED_WITH_ERRORS,
} parser_status_e;

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

    // A vector of tokens from lexical analysis.
    struct vector *p_s_token_vec;

    //
    struct vector *p_node_vec;
    struct vector *p_node_tree_vec;

    FILE *ofile;
} compile_process_t;

typedef enum node_type {
    NODE_TYPE_TOKEN,
    NODE_TYPE_EXPRESSION,
    NODE_TYPE_EXPRESSION_PARANTHESES,
    NODE_TYPE_NUMBER,
    NODE_TYPE_STRING,
    NODE_TYPE_IDENTIFIER,
    NODE_TYPE_OPERATOR,
    NODE_TYPE_KEYWORD,
    NODE_TYPE_VARIABLE,
    NODE_TYPE_VARIABLE_LIST,
    NODE_TYPE_FUNCTION,
    NODE_TYPE_BODY,

    NODE_TYPE_STATEMENT_RETURN,
    NODE_TYPE_STATEMENT_IF,
    NODE_TYPE_STATEMENT_ELSE,
    NODE_TYPE_STATEMENT_WHILE,
    NODE_TYPE_STATEMENT_DO_WHILE,
    NODE_TYPE_STATEMENT_FOR,
    NODE_TYPE_STATEMENT_BREAK,
    NODE_TYPE_STATEMENT_CONTINUE,
    NODE_TYPE_STATEMENT_SWITCH,
    NODE_TYPE_STATEMENT_CASE,
    NODE_TYPE_STATEMENT_DEFAULT,
    NODE_TYPE_STATEMENT_GOTO,

    NODE_TYPE_UNARY,
    NODE_TYPE_TENARY,
    NODE_TYPE_LABEL,
    NODE_TYPE_STRUCT,
    NODE_TYPE_ENUM,
    NODE_TYPE_UNION,
    NODE_TYPE_TYPEDEF,
    NODE_TYPE_INCLUDE,
    NODE_TYPE_DEFINE,
    NODE_TYPE_MACRO,
    NODE_TYPE_BRACKET,
    NODE_TYPE_CAST,
    NODE_TYPE_BLANK,

} node_type_e;

typedef struct node_s {
    node_type_e type;
    int flags;

    pos_t s_pos;

    struct node_binded {
        // Pointer to our body node
        struct node_s *p_node;

        // Pointer to the function this node is in.
        struct node_s *p_function;
    } binded;

    union {
        char cval;
        const char *sval;
        unsigned int ival;
        unsigned long lval;
        unsigned long long llval;
    };

} node_t;

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
int parse(compile_process_t *p_process);
void debug_print_compile_process(compile_process_t *p_process);
void debug_print_pos_struct(pos_t *p_pos);

/* Node Functions*/
void node_set_vector(struct vector *p_node_vec, struct vector *p_node_vec_root);
void node_push(node_t *p_node);
node_t *node_peek_or_null();
node_t *node_peek();
node_t *node_pop();
node_t *node_create(node_t *p_node);

/* Token Functions*/
bool token_is_nl_or_comment_or_nl_seperator(const token_t *p_token);
bool token_is_symbol(const token_t *p_token, char symbol);

#endif  // MATAN_COMPILER_H

#ifdef __cplusplus
}
#endif
