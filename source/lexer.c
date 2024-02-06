#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "compiler/compiler.h"
#include "trace.h"
#include "utils/buffer.h"
#include "utils/vector.h"

#define LEX_GETC_IF(buffer_, c_, exp_) \
    char c_ = peekc();                 \
    for (; exp_; c_ = peekc()) {       \
        buffer_write(buffer_, c_);     \
        nextc();                       \
    }

#define IS_NUMERIC(c) (((c) >= '0') && ((c) <= '9'))

#define S_EQ(a, b) (a && b && (0 == strcmp(a, b)))
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

#define OPERATOR_CASE_EXCLUDING_DIVISION \
    case '+':                            \
    case '-':                            \
    case '*':                            \
    case '<':                            \
    case '>':                            \
    case '%':                            \
    case '=':                            \
    case '!':                            \
    case '&':                            \
    case '^':                            \
    case '~':                            \
    case '(':                            \
    case '[':                            \
    case ',':                            \
    case '.':                            \
    case '?':                            \
    case '|'

#define SYMBOL_CASE \
    case ';':       \
    case ':':       \
    case '{':       \
    case '}':       \
    case ']':       \
    case ')':       \
    case '\\':      \
    case '#'

lex_process_t *g_p_lex_process;
static token_t tmp_token;

static pos_t lex_file_position(void) { return g_p_lex_process->s_pos; }

static char peekc(void) {
    FW_LOG_ENTERED_FUNCTION();
    return g_p_lex_process->function->peek_char(g_p_lex_process);
}

static char nextc(void) {
    char c = g_p_lex_process->function->next_char(g_p_lex_process);
    if (c == '\n') {
        g_p_lex_process->s_pos.line++;
        g_p_lex_process->s_pos.col = 0;
    } else {
        g_p_lex_process->s_pos.col++;
    }
    return c;
}

static void pushc(char c) { g_p_lex_process->function->push_char(g_p_lex_process, c); }

static char peek_next_c(void) {
    char current_char = nextc();
    char next_char = peekc();
    pushc(current_char);
    return next_char;
}
static void lex_new_expression(void) {
    FW_LOG_ENTERED_FUNCTION();
    g_p_lex_process->current_expression_count++;
    if (1 == g_p_lex_process->current_expression_count) {
        // This means we are starting a new expression.
        g_p_lex_process->p_parantheses_buffer = buffer_create();
    }
}

void debug_log_token(token_t *p_s_token) {
    FW_LOG_DEBUG("A new Token: {\n");
    FW_LOG_DEBUG("  .type = %d\n", p_s_token->type);
    if (p_s_token->type == TOKEN_TYPE_NUMBER) {
        FW_LOG_DEBUG("  .llval = %llu\n", p_s_token->llval);
    }
    else if (p_s_token->type == TOKEN_TYPE_STRING)
    {
        FW_LOG_DEBUG("  .sval = %s\n", p_s_token->sval);
    }
    else if (p_s_token->type == TOKEN_TYPE_OPERATOR)
    {
        FW_LOG_DEBUG("  .sval = %s\n", p_s_token->sval);
    }
    else if (p_s_token->type == TOKEN_TYPE_SYMBOL)
    {
        FW_LOG_DEBUG("  .cval = %c\n", p_s_token->cval);
    }
    else if (p_s_token->type == TOKEN_TYPE_NEWLINE)
    {
        FW_LOG_DEBUG("  .cval = NEW_LINE\n");
    }
    else if (p_s_token->type == TOKEN_TYPE_COMMENT)
    {
        FW_LOG_DEBUG("  .sval = %s\n", p_s_token->sval);
    }
    else if (p_s_token->type == TOKEN_TYPE_IDENTIFIER)
    {
        FW_LOG_DEBUG("  .sval = %s\n", p_s_token->sval);
    }
    FW_LOG_DEBUG("  .whitespace = %d\n", p_s_token->whitespace);
    if (p_s_token->between_brackets){
        FW_LOG_DEBUG("  .between_brackets = %s\n", p_s_token->between_brackets);
    }
    FW_LOG_DEBUG("  .s_pos = ");
    debug_print_pos_struct(&p_s_token->s_pos);
    FW_LOG_DEBUG("}\n");
}
bool lex_is_in_an_expression(void) { return g_p_lex_process->current_expression_count > 0; }
token_t *token_create(token_t *p_token) {
    memcpy(&tmp_token, p_token, sizeof(token_t));
    tmp_token.s_pos = lex_file_position();

    // Log token information
    debug_log_token(&tmp_token);
    
    return &tmp_token;
}

static token_t *lexer_last_token(void) {
    FW_LOG_ENTERED_FUNCTION();
    return vector_back_or_null(g_p_lex_process->p_s_token_vec);
}

static token_t *handle_whitespace(void) {
    FW_LOG_ENTERED_FUNCTION();
    token_t *p_s_token = lexer_last_token();
    if (p_s_token) {
        p_s_token->whitespace = true;
    }
    nextc();
    return read_next_token();
}

const char *read_number_str(void) {
    FW_LOG_ENTERED_FUNCTION();
    struct buffer *p_buffer = buffer_create();
    LEX_GETC_IF(p_buffer, c, IS_NUMERIC(c));

    buffer_write(p_buffer, '\0');
    printf("Closed buffer\n");
    return buffer_ptr(p_buffer);
}

unsigned long long read_number(void) {
    FW_LOG_ENTERED_FUNCTION();
    const char *p_str = read_number_str();
    unsigned long long value = atoll(p_str);
    printf("value = %llu\n", value);
    return value;
}

token_t *token_make_number_by_value(unsigned long long value) {
    FW_LOG_ENTERED_FUNCTION();
    token_t p_s_token = {
        .type = TOKEN_TYPE_NUMBER,
        .llval = value,
    };
    return token_create(&p_s_token);
}

token_t *token_make_number(void) {
    FW_LOG_ENTERED_FUNCTION();
    return token_make_number_by_value(read_number());
}

static token_t *token_make_string(const char start_delimiter, const char end_delimiter) {
    FW_LOG_ENTERED_FUNCTION();
    struct buffer *p_buffer = buffer_create();
    assert(nextc() == start_delimiter);
    char c = nextc();
    while (c != end_delimiter && c != EOF) {
        if (c == '\\') {
            // Handle escape sequence
            c = nextc();
            switch (c) {
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
                    compiler_error(g_p_lex_process->p_s_compiler,
                                   "Unknown escape sequence '\\%c'\n", c);
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

static bool op_treated_as_one(const char op) {
    switch (op) {
        case '*':
        case '%':
        case '!':
        case '^':
        case '~':
        case '(':
        case '[':
        case ',':
        case '.':
        case '?':
            return true;
        default:
            return false;
    }
}

static bool is_single_operator(const char c) {
    switch (c) {
        case '+':
        case '-':
        case '/':
        case '*':
        case '=':
        case '>':
        case '<':
        case '&':
        case '|':
        case '%':
        case '^':
        case '~':
        case '!':
        case '[':
        case '(':
        case ',':
        case '.':
        case '?':
            return true;
        default:
            return false;
    }
}

static bool op_valid(const char *op) {
    return S_EQ(op, "++") || S_EQ(op, "--") || S_EQ(op, "+=") || S_EQ(op, "-=") || S_EQ(op, "*=")
           || S_EQ(op, "/=") || S_EQ(op, "%=") || S_EQ(op, "&=") || S_EQ(op, "|=") || S_EQ(op, "^=")
           || S_EQ(op, "<<") || S_EQ(op, ">>") || S_EQ(op, "&&") || S_EQ(op, "||") || S_EQ(op, "==")
           || S_EQ(op, "!=") || S_EQ(op, "<=") || S_EQ(op, ">=") || S_EQ(op, "->")
           || S_EQ(op, "...");
}

static void read_op_flush_back_keep_first(struct buffer *p_buffer) {
    FW_LOG_ENTERED_FUNCTION();
    int len = p_buffer->len;
    for (int i = len - 1; i >= 1; i--) {
        if (0x00 == p_buffer->data[i]) {
            continue;
        }
        pushc(p_buffer->data[i]);
    }
}

const char *read_op(void) {
    FW_LOG_ENTERED_FUNCTION();
    bool single_operator = true;
    char op = nextc();
    struct buffer *p_buffer = buffer_create();
    buffer_write(p_buffer, op);
    if (!op_treated_as_one(op)) {
        char c = peekc();
        if (is_single_operator(c)) {
            buffer_write(p_buffer, c);
            nextc();
            single_operator = false;
        }
    }
    buffer_write(p_buffer, 0x00);
    char *ptr = buffer_ptr(p_buffer);
    if (false == single_operator) {
        if (!op_valid(ptr)) {
            read_op_flush_back_keep_first(p_buffer);
            ptr[1] = 0x00;
        }
    }
    return buffer_ptr(p_buffer);
}

token_t *token_make_operator_or_string(void) {
    char op = peekc();
    if ('<' == op) {
        token_t *p_s_token = lexer_last_token();
        if (token_is_keyword(p_s_token, "include")) {
            return token_make_string('<', '>');
        }
    }

    token_t s_token = {
        .type = TOKEN_TYPE_OPERATOR,
        .sval = read_op(),
    };
    token_t *p_s_token = token_create(&s_token);
    if (op == '(') {
        lex_new_expression();
    }

    return p_s_token;
}

static void lex_finish_expression(void) {
    FW_LOG_ENTERED_FUNCTION();
    g_p_lex_process->current_expression_count--;
    if (g_p_lex_process->current_expression_count < 0) {
        compiler_error(g_p_lex_process->p_s_compiler, "Unmatched parantheses\n");
    }
}

static token_t *token_make_symbol(void) {
    FW_LOG_ENTERED_FUNCTION();
    char c = nextc();
    if (')' == c) {
        lex_finish_expression();
    }

    token_t s_token = {
        .type = TOKEN_TYPE_SYMBOL,
        .cval = c,
    };
    return token_create(&s_token);
}

// clang-format off
static bool identifier_is_keyword(const char *p_str) {
    // Verify if the string is a keyword
    return S_EQ(p_str, "if")       || S_EQ(p_str, "else")     || S_EQ(p_str, "while") || S_EQ(p_str, "for")
        || S_EQ(p_str, "do")       || S_EQ(p_str, "return")   || S_EQ(p_str, "break")
        || S_EQ(p_str, "continue") || S_EQ(p_str, "switch")   || S_EQ(p_str, "case")
        || S_EQ(p_str, "default")  || S_EQ(p_str, "goto")     || S_EQ(p_str, "typedef")
        || S_EQ(p_str, "struct")   || S_EQ(p_str, "enum")     || S_EQ(p_str, "union")
        || S_EQ(p_str, "const")    || S_EQ(p_str, "volatile") || S_EQ(p_str, "extern")
        || S_EQ(p_str, "static")   || S_EQ(p_str, "register") || S_EQ(p_str, "auto")
        || S_EQ(p_str, "void")     || S_EQ(p_str, "char")     || S_EQ(p_str, "short")
        || S_EQ(p_str, "int")      || S_EQ(p_str, "long")     || S_EQ(p_str, "float")
        || S_EQ(p_str, "double")   || S_EQ(p_str, "signed")   || S_EQ(p_str, "unsigned")
        || S_EQ(p_str, "sizeof")   || S_EQ(p_str, "alignof")  || S_EQ(p_str, "offsetof")
        || S_EQ(p_str, "asm")      || S_EQ(p_str, "inline")   || S_EQ(p_str, "restrict")
        || S_EQ(p_str, "bool")     || S_EQ(p_str, "true")     || S_EQ(p_str, "false")
        || S_EQ(p_str, "NULL")     || S_EQ(p_str, "va_list")  || S_EQ(p_str, "va_start")
        || S_EQ(p_str, "va_end")   || S_EQ(p_str, "va_arg")   || S_EQ(p_str, "va_copy");
}
// clang-format on

static token_t *token_make_identifier_or_keyword(void) {
    FW_LOG_ENTERED_FUNCTION();
    struct buffer *p_buffer = buffer_create();
    LEX_GETC_IF(p_buffer, c,
                (IS_NUMERIC(c) || ('_' == c) || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')));
    buffer_write(p_buffer, '\0');
    const char *p_str = buffer_ptr(p_buffer);
    token_t s_token = {
        .type = identifier_is_keyword(p_str) ? TOKEN_TYPE_KEYWORD : TOKEN_TYPE_IDENTIFIER,
        .sval = p_str,
    };
    return token_create(&s_token);
}

token_t *read_special_token(void) {
    char c = peekc();
    if (isalpha(c) || c == '_') {
        return token_make_identifier_or_keyword();
    }
    return NULL;
}

token_t *token_make_newline(void) {
    nextc();
    token_t s_token = {
        .type = TOKEN_TYPE_NEWLINE,
    };
    return token_create(&s_token);
}
token_t *token_make_one_line_comment(void) {
    FW_LOG_ENTERED_FUNCTION();
    struct buffer *p_buffer = buffer_create();
    LEX_GETC_IF(p_buffer, c, (c != '\n' && c != EOF));
    buffer_write(p_buffer, '\0');
    token_t s_token = {
        .type = TOKEN_TYPE_COMMENT,
        .sval = buffer_ptr(p_buffer),
    };
    return token_create(&s_token);
}

token_t *token_make_multi_line_comment(void) {
    FW_LOG_ENTERED_FUNCTION();
    struct buffer *p_buffer = buffer_create();
    LEX_GETC_IF(p_buffer, c, (c != EOF && !(c == '*' && peek_next_c() == '/')));
    if (EOF == c) {
        compiler_error(g_p_lex_process->p_s_compiler,
                       "Unexpected end of file, in the middle of multi-line comment\n");
    }
    nextc();
    nextc();
    buffer_write(p_buffer, '\0');
    token_t s_token = {
        .type = TOKEN_TYPE_COMMENT,
        .sval = buffer_ptr(p_buffer),
    };
    return token_create(&s_token);
}

token_t *token_make_comment(void) {
    FW_LOG_ENTERED_FUNCTION();
    // We have already read the first '/' character
    // but we need to consume it to dermine if it is a one line or multi line comment
    nextc();
    char c = peekc();
    if (c == '/') {
        return token_make_one_line_comment();
    }
    if (c == '*') {
        return token_make_multi_line_comment();
    }
    pushc('/');
    return token_make_operator_or_string();
}
token_t *read_next_token(void) {
    token_t *p_s_token = NULL;
    char c = peekc();
    switch (c) {
    NUMERIC_CASE:
        p_s_token = token_make_number();
        break;

    OPERATOR_CASE_EXCLUDING_DIVISION:
        p_s_token = token_make_operator_or_string();
        break;
    SYMBOL_CASE:
        p_s_token = token_make_symbol();
        break;
        case '"':
            p_s_token = token_make_string('"', '"');
            break;
        case ' ':
        case '\t':
            p_s_token = handle_whitespace();
            break;
        case '\n':
            p_s_token = token_make_newline();
            break;
        case '/':
            p_s_token = token_make_comment();
            break;
        case EOF:
            // We have finished reading the file. do nothing
            break;
        default:
            p_s_token = read_special_token();
            if (p_s_token == NULL) {
                compiler_error(g_p_lex_process->p_s_compiler, "Unexpected token '%c' \n", c);
            }
            break;
    }
    return p_s_token;
}

int lex(lex_process_t *p_lex_process) {
    p_lex_process->current_expression_count = 0;
    p_lex_process->p_parantheses_buffer = NULL;
    p_lex_process->s_pos.filename = p_lex_process->p_s_compiler->cfile.abs_path;
    g_p_lex_process = p_lex_process;

    token_t *p_s_token = read_next_token();
    while (p_s_token) {
        vector_push(p_lex_process->p_s_token_vec, p_s_token);
        p_s_token = read_next_token();
    }
    return LEXICAL_ANALSYS_ALL_OK;
}

void debug_print_pos_struct(pos_t *p_pos) {
    printf("{ .line = %d, .col = %d, .filename = %s }\n", p_pos->line, p_pos->col, p_pos->filename);
}
