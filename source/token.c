#include <string.h>

#include "compiler/compiler.h"

bool token_is_keyword(const token_t *p_token, const char *p_keyword) {
    if (p_token->type != TOKEN_TYPE_KEYWORD) {
        return false;
    }
    return strcmp(p_token->sval, p_keyword) == 0;
}

bool token_is_symbol(const token_t *p_token, char symbol) {
    return ((p_token->type == TOKEN_TYPE_SYMBOL) && (p_token->cval == symbol));
}

bool token_is_nl_or_comment_or_nl_seperator(const token_t *p_token) {
    return p_token->type == TOKEN_TYPE_NEWLINE || p_token->type == TOKEN_TYPE_COMMENT
           || token_is_symbol(p_token, '\\');
}