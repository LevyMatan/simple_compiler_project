#include <doctest/doctest.h>

#include <iostream>
#include <string>

#include "compiler/compiler.h"
#include "simple_compiler_project/version.h"
#include "trace.h"
#include "utils/vector.h"

TEST_CASE("token::token_is_keyword") {
    token_t token;
    token.type = TOKEN_TYPE_KEYWORD;
    token.sval = "keyword";

    CHECK(token_is_keyword(&token, "keyword") == true);
    CHECK(token_is_keyword(&token, "notkeyword") == false);

    token.type = TOKEN_TYPE_SYMBOL;
    CHECK(token_is_keyword(&token, "keyword") == false);
}

TEST_CASE("token::token_is_symbol") {
    token_t token;
    token.type = TOKEN_TYPE_SYMBOL;
    token.cval = '+';

    CHECK(token_is_symbol(&token, '+') == true);
    CHECK(token_is_symbol(&token, '-') == false);

    token.type = TOKEN_TYPE_KEYWORD;
    CHECK(token_is_symbol(&token, '+') == false);
}

TEST_CASE("token::token_is_nl_or_comment_or_nl_seperator") {
    token_t token;

    token.type = TOKEN_TYPE_NEWLINE;
    CHECK(token_is_nl_or_comment_or_nl_seperator(&token) == true);

    token.type = TOKEN_TYPE_COMMENT;
    CHECK(token_is_nl_or_comment_or_nl_seperator(&token) == true);

    token.type = TOKEN_TYPE_SYMBOL;
    token.cval = '\\';
    CHECK(token_is_nl_or_comment_or_nl_seperator(&token) == true);

    token.cval = '+';
    CHECK(token_is_nl_or_comment_or_nl_seperator(&token) == false);

    token.type = TOKEN_TYPE_KEYWORD;
    CHECK(token_is_nl_or_comment_or_nl_seperator(&token) == false);
}

TEST_CASE("identifier_is_keyword") {
    CHECK(identifier_is_keyword("if") == true);
    CHECK(identifier_is_keyword("else") == true);
    CHECK(identifier_is_keyword("while") == true);
    CHECK(identifier_is_keyword("for") == true);
    CHECK(identifier_is_keyword("do") == true);
    CHECK(identifier_is_keyword("return") == true);
    CHECK(identifier_is_keyword("break") == true);
    CHECK(identifier_is_keyword("continue") == true);
    CHECK(identifier_is_keyword("switch") == true);
    CHECK(identifier_is_keyword("case") == true);
    CHECK(identifier_is_keyword("default") == true);
    CHECK(identifier_is_keyword("goto") == true);
    CHECK(identifier_is_keyword("typedef") == true);
    CHECK(identifier_is_keyword("struct") == true);
    CHECK(identifier_is_keyword("enum") == true);
    CHECK(identifier_is_keyword("union") == true);
    CHECK(identifier_is_keyword("const") == true);
    CHECK(identifier_is_keyword("volatile") == true);
    CHECK(identifier_is_keyword("extern") == true);
    CHECK(identifier_is_keyword("static") == true);
    CHECK(identifier_is_keyword("register") == true);
    CHECK(identifier_is_keyword("auto") == true);
    CHECK(identifier_is_keyword("void") == true);
    CHECK(identifier_is_keyword("char") == true);
    CHECK(identifier_is_keyword("short") == true);
    CHECK(identifier_is_keyword("int") == true);
    CHECK(identifier_is_keyword("long") == true);
    CHECK(identifier_is_keyword("float") == true);
    CHECK(identifier_is_keyword("double") == true);
    CHECK(identifier_is_keyword("signed") == true);
    CHECK(identifier_is_keyword("unsigned") == true);
    CHECK(identifier_is_keyword("sizeof") == true);
    CHECK(identifier_is_keyword("alignof") == true);
    CHECK(identifier_is_keyword("offsetof") == true);
    CHECK(identifier_is_keyword("asm") == true);
    CHECK(identifier_is_keyword("inline") == true);
    CHECK(identifier_is_keyword("restrict") == true);
    CHECK(identifier_is_keyword("bool") == true);
    CHECK(identifier_is_keyword("true") == true);
    CHECK(identifier_is_keyword("false") == true);
    CHECK(identifier_is_keyword("NULL") == true);
    CHECK(identifier_is_keyword("va_list") == true);
    CHECK(identifier_is_keyword("va_start") == true);
    CHECK(identifier_is_keyword("va_end") == true);
    CHECK(identifier_is_keyword("va_arg") == true);
    CHECK(identifier_is_keyword("va_copy") == true);

    CHECK(identifier_is_keyword("notakeyword") == false);
    CHECK(identifier_is_keyword("also_not_a_keyword") == false);
}

TEST_CASE("op_treated_as_one") {
    CHECK(op_treated_as_one('*') == true);
    CHECK(op_treated_as_one('%') == true);
    CHECK(op_treated_as_one('!') == true);
    CHECK(op_treated_as_one('^') == true);
    CHECK(op_treated_as_one('~') == true);
    CHECK(op_treated_as_one('(') == true);
    CHECK(op_treated_as_one('[') == true);
    CHECK(op_treated_as_one(',') == true);
    CHECK(op_treated_as_one('.') == true);
    CHECK(op_treated_as_one('?') == true);
    CHECK(op_treated_as_one('a') == false);
}

TEST_CASE("is_single_operator") {
    CHECK(is_single_operator('+') == true);
    CHECK(is_single_operator('-') == true);
    CHECK(is_single_operator('/') == true);
    CHECK(is_single_operator('*') == true);
    CHECK(is_single_operator('=') == true);
    CHECK(is_single_operator('>') == true);
    CHECK(is_single_operator('<') == true);
    CHECK(is_single_operator('&') == true);
    CHECK(is_single_operator('|') == true);
    CHECK(is_single_operator('%') == true);
    CHECK(is_single_operator('^') == true);
    CHECK(is_single_operator('~') == true);
    CHECK(is_single_operator('!') == true);
    CHECK(is_single_operator('[') == true);
    CHECK(is_single_operator('(') == true);
    CHECK(is_single_operator(',') == true);
    CHECK(is_single_operator('.') == true);
    CHECK(is_single_operator('?') == true);
    CHECK(is_single_operator('a') == false);
}

TEST_CASE("op_valid") {
    CHECK(op_valid("++") == true);
    CHECK(op_valid("--") == true);
    CHECK(op_valid("+=") == true);
    CHECK(op_valid("-=") == true);
    CHECK(op_valid("*=") == true);
    CHECK(op_valid("/=") == true);
    CHECK(op_valid("%=") == true);
    CHECK(op_valid("&=") == true);
    CHECK(op_valid("|=") == true);
    CHECK(op_valid("^=") == true);
    CHECK(op_valid("<<") == true);
    CHECK(op_valid(">>") == true);
    CHECK(op_valid("&&") == true);
    CHECK(op_valid("||") == true);
    CHECK(op_valid("==") == true);
    CHECK(op_valid("!=") == true);
    CHECK(op_valid("<=") == true);
    CHECK(op_valid(">=") == true);
    CHECK(op_valid("->") == true);
    CHECK(op_valid("...") == true);
    CHECK(op_valid("invalid") == false);
}