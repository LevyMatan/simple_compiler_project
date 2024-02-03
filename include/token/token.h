#ifndef TOKEN_H
#define TOKEN_H

#include "common.h"
#include <stdbool.h>

enum token_type
{
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


typedef struct token_s
{
    int type;
    int flags;
    pos_t s_pos;
    union
    {
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

/**
 * @brief 
 * 
 * @return token_t* 
 */
token_t *read_next_token(void);

/**
 * @brief 
 * 
 * @param p_token 
 * @return token_t* 
 */
token_t *token_create(token_t *p_token);

/**
 * @brief 
 * 
 * @return token_t* 
 */
static token_t *handle_whitespace(void);

#endif // TOKEN_H
