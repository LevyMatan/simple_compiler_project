#include "compiler/compiler.h"
#include "utils/vector.h"
#include "utils/buffer.h"
#include "trace.h"
#include <string.h>
#include <assert.h>

#define LEX_GETC_IF(buffer_, c_, exp_)          \
    for (char c_ = peekc(); exp_; c_ = peekc()) \
    {                                           \
        buffer_write(buffer_, c_);              \
        nextc();                                \
    }

#define IS_NUMERIC(c) (((c) >= '0') && ((c) <= '9'))

/**
 * @brief use in a switch statement to match all numeric characters.
 *
 */
#define NUMERIC_CASE \
    case '0':        \
    case '1':        \
    case '2':        \
    case '3':        \
    case '4':        \
    case '5':        \
    case '6':        \
    case '7':        \
    case '8':        \
    case '9'

static lex_process_t *g_p_lex_process;
static token_t tmp_token;

static pos_t lex_file_position(void)
{
    return g_p_lex_process->s_pos;
}

static char peekc(void)
{
    FW_LOG_ENTERED_FUNCTION();
    return g_p_lex_process->function->peek_char(g_p_lex_process);
}

static char nextc(void)
{
    char c = g_p_lex_process->function->next_char(g_p_lex_process);
    if (c == '\n')
    {
        g_p_lex_process->s_pos.line++;
        g_p_lex_process->s_pos.col = 0;
    }
    else
    {
        g_p_lex_process->s_pos.col++;
    }
    return c;
}

static void pushc(char c)
{
    g_p_lex_process->function->push_char(g_p_lex_process, c);
}

token_t *token_create(token_t *p_token)
{
    memcpy(&tmp_token, p_token, sizeof(token_t));
    printf("tmp_token.type = %d\n", tmp_token.type);
    tmp_token.s_pos = lex_file_position();
    printf("tmp_token.s_pos.line = %d\n", tmp_token.s_pos.line);
    return &tmp_token;
}

static token_t *lexer_last_token(void)
{
    FW_LOG_ENTERED_FUNCTION();
    return vector_back_or_null(g_p_lex_process->p_s_token_vec);
}

static token_t *handle_whitespace(void)
{
    FW_LOG_ENTERED_FUNCTION();
    token_t *p_s_token = lexer_last_token();
    if (p_s_token)
    {
        p_s_token->whitespace = true;
    }
    nextc();
    return read_next_token();
}

const char *read_number_str(void)
{
    FW_LOG_ENTERED_FUNCTION();
    const char *p_str = NULL;
    struct buffer *p_buffer = buffer_create();
    LEX_GETC_IF(p_buffer, c, IS_NUMERIC(c));

    buffer_write(p_buffer, '\0');
    printf("Closed buffer\n");
    return buffer_ptr(p_buffer);
}

unsigned long long read_number(void)
{
    FW_LOG_ENTERED_FUNCTION();
    const char *p_str = read_number_str();
    unsigned long long value = atoll(p_str);
    printf("value = %llu\n", value);
    return value;
}

token_t *token_make_number_by_value(unsigned long long value)
{
    FW_LOG_ENTERED_FUNCTION();
    token_t p_s_token = {
        .type = TOKEN_TYPE_NUMBER,
        .llval = value,
    };
    return token_create(&p_s_token);
}

token_t *token_make_number(void)
{
    FW_LOG_ENTERED_FUNCTION();
    return token_make_number_by_value(read_number());
}

static token_t *token_make_string(const char start_delimiter, const char end_delimiter)
{
    FW_LOG_ENTERED_FUNCTION();
    struct buffer *p_buffer = buffer_create();
    assert(nextc() == start_delimiter);
    char c = nextc();
    while (c != end_delimiter && c != EOF)
    {
        if (c == '\\')
        {
            // Handle escape sequence
            c = nextc();
            switch (c)
            {
            case 'n':
                buffer_write(p_buffer, '\n');
                break;
            case 't':
                buffer_write(p_buffer, '\t');
                break;
            case 'r':
                buffer_write(p_buffer, '\r');
                break;
            case '0':
                buffer_write(p_buffer, '\0');
                break;
            case '\\':
                buffer_write(p_buffer, '\\');
                break;
            case '\'':
                buffer_write(p_buffer, '\'');
                break;
            case '\"':
                buffer_write(p_buffer, '\"');
                break;
            default:
                compiler_error(g_p_lex_process->p_s_compiler, "Unknown escape sequence '\\%c'\n", c);
                break;
            }
        }

        buffer_write(p_buffer, c);
        c = nextc();
    }
    buffer_write(p_buffer, '\0');

    token_t s_token = {
        .type = TOKEN_TYPE_STRING,
        .sval = buffer_ptr(p_buffer),
    };
    return token_create(&s_token);

}

token_t *read_next_token(void)
{
    token_t *p_s_token = NULL;
    char c = peekc();
    printf("I am read_next_token! with c = %c\n", c);
    switch (c)
    {
    NUMERIC_CASE:
        p_s_token = token_make_number();
        printf("p_s_token->type = %d\n", p_s_token->type);
        break;

    case '"' :
        p_s_token = token_make_string('"', '"');
        break;
    case ' ':
    case '\t':
        p_s_token = handle_whitespace();
        break;
    case EOF:
        // We have finished reading the file. do nothing
        break;
    default:
        compiler_error(g_p_lex_process->p_s_compiler, "Unexpected token '%c' \n", c);
        break;
    }
    return p_s_token;
}

int lex(lex_process_t *p_lex_process)
{
    p_lex_process->current_expression_count = 0;
    p_lex_process->p_parantheses_buffer = NULL;
    // printf("p_lex_process->p_s_compiler = %p\n", p_lex_process->p_s_compiler);
    p_lex_process->s_pos.filename = p_lex_process->p_s_compiler->cfile.abs_path;
    // printf("p_lex_process->s_pos.filename = %s\n", p_lex_process->s_pos.filename);
    g_p_lex_process = p_lex_process;

    token_t *p_s_token = read_next_token();
    printf("p_s_token = %p\n", p_s_token);
    printf("p_s_token->type = %d\n", p_s_token->type);
    while (p_s_token)
    {
        vector_push(p_lex_process->p_s_token_vec, p_s_token);
        p_s_token = read_next_token();
    }
    printf("I am at the end of lex!\n");
    return LEXICAL_ANALSYS_ALL_OK;
}