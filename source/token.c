#include "compiler/compiler.h"
#include <string.h>

bool token_is_keyword(const token_t *p_token, const char *p_keyword)
{
    if (p_token->type != TOKEN_TYPE_KEYWORD)
    {
        return false;
    }
    return strcmp(p_token->sval, p_keyword) == 0;
}