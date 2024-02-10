#include <string.h>

#include "utils/generated_enum_dict.h"

char* trace_level_dict[4] = {
    "TRACE_LEVEL_DEBUG",
    "TRACE_LEVEL_INFO",
    "TRACE_LEVEL_WARNING",
    "TRACE_LEVEL_ERROR",
};

char* parser_status_dict[2] = {
    "PARSE_ALL_OK",
    "PARSE_FAILED_WITH_ERRORS",
};

char* LEXICAL_ANALSYS_STATAUS_dict[2] = {
    "LEXICAL_ANALSYS_ALL_OK",
    "LEXICAL_ANALSYS_FAILED_WITH_ERRORS",
};

char* token_type_dict[9] = {
    "TOKEN_TYPE_IDENTIFIER", "TOKEN_TYPE_KEYWORD", "TOKEN_TYPE_OPERATOR",
    "TOKEN_TYPE_SYMBOL",     "TOKEN_TYPE_NUMBER",  "TOKEN_TYPE_STRING",
    "TOKEN_TYPE_COMMENT",    "TOKEN_TYPE_NEWLINE", "TOKEN_TYPE_EOF",
};

char* number_type_dict[4] = {
    "NUMBER_TYPE_INT",
    "NUMBER_TYPE_LONG",
    "NUMBER_TYPE_LONG_LONG",
    "NUMBER_TYPE_FLOAT",
};

char* node_type_dict[37] = {
    "NODE_TYPE_TOKEN",
    "NODE_TYPE_EXPRESSION",
    "NODE_TYPE_EXPRESSION_PARANTHESES",
    "NODE_TYPE_NUMBER",
    "NODE_TYPE_STRING",
    "NODE_TYPE_IDENTIFIER",
    "NODE_TYPE_OPERATOR",
    "NODE_TYPE_KEYWORD",
    "NODE_TYPE_VARIABLE",
    "NODE_TYPE_VARIABLE_LIST",
    "NODE_TYPE_FUNCTION",
    "NODE_TYPE_BODY",
    "NODE_TYPE_STATEMENT_RETURN",
    "NODE_TYPE_STATEMENT_IF",
    "NODE_TYPE_STATEMENT_ELSE",
    "NODE_TYPE_STATEMENT_WHILE",
    "NODE_TYPE_STATEMENT_DO_WHILE",
    "NODE_TYPE_STATEMENT_FOR",
    "NODE_TYPE_STATEMENT_BREAK",
    "NODE_TYPE_STATEMENT_CONTINUE",
    "NODE_TYPE_STATEMENT_SWITCH",
    "NODE_TYPE_STATEMENT_CASE",
    "NODE_TYPE_STATEMENT_DEFAULT",
    "NODE_TYPE_STATEMENT_GOTO",
    "NODE_TYPE_UNARY",
    "NODE_TYPE_TENARY",
    "NODE_TYPE_LABEL",
    "NODE_TYPE_STRUCT",
    "NODE_TYPE_ENUM",
    "NODE_TYPE_UNION",
    "NODE_TYPE_TYPEDEF",
    "NODE_TYPE_INCLUDE",
    "NODE_TYPE_DEFINE",
    "NODE_TYPE_MACRO",
    "NODE_TYPE_BRACKET",
    "NODE_TYPE_CAST",
    "NODE_TYPE_BLANK",
};
