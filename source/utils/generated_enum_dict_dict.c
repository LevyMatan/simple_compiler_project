#include <string.h>

#include "utils/generated_enum_dict.h"

char* trace_level_dict[4] = {
    "TRACE_LEVEL_DEBUG",
    "TRACE_LEVEL_INFO",
    "TRACE_LEVEL_WARNING",
    "TRACE_LEVEL_ERROR",
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
