#include <doctest/doctest.h>

#include <iostream>
#include <string>

#include "compiler/compiler.h"
#include "simple_compiler_project/version.h"
#include "trace.h"
#include "utils/vector.h"

extern lex_process_function_t compiler_lex_functions;
extern lex_process_t *g_p_lex_process;

TEST_CASE("lexer::lex") {
    /**
     * 1. Create a new file to test our lexer on
     * 2. Write the test string '324+32 ++ (3+2) "Hello World!"' to the file
     * 3. Create a new compile_process_t object
     * 4. Create a new lex_process_t object
     * 5. Call lex() on the lex_process_t object
     * 6. Check the tokens
     */
    FILE *p_file = fopen("test_lexer.c", "w");
    REQUIRE(p_file != nullptr);
    fprintf(p_file, "324+32 ++ (3+2) \"Hello World!\"");
    fclose(p_file);

    compile_process_t *p_process = compile_process_create("test_lexer.c", NULL, 0);
    REQUIRE(p_process != nullptr);

    // Preform Lexical Analysis
    lex_process_t *p_lex_process = lex_process_create(p_process, &compiler_lex_functions, NULL);
    REQUIRE(p_lex_process != nullptr);

    CHECK(LEXICAL_ANALSYS_ALL_OK == lex(p_lex_process));

    // Check the tokens
    vector *p_s_token_vec = p_lex_process->p_s_token_vec;
    // Iterate over the tokens using vector methods
    token_t *p_token = (token_t *)vector_at(p_s_token_vec, 0);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_NUMBER);
    CHECK(p_token->whitespace == false);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 1);
    CHECK(p_token->s_pos.col == 3);
    CHECK(p_token->ival == 324);

    p_token = (token_t *)vector_at(p_s_token_vec, 1);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_OPERATOR);
    CHECK(p_token->whitespace == false);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 1);
    CHECK(p_token->s_pos.col == 4);
    CHECK_EQ(std::string(p_token->sval), std::string("+"));

    p_token = (token_t *)vector_at(p_s_token_vec, 2);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_NUMBER);
    CHECK(p_token->whitespace == true);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 1);
    CHECK(p_token->s_pos.col == 6);
    CHECK(p_token->ival == 32);

    p_token = (token_t *)vector_at(p_s_token_vec, 3);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_OPERATOR);
    CHECK(p_token->whitespace == true);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 1);
    CHECK(p_token->s_pos.col == 9);
    CHECK_EQ(std::string(p_token->sval), std::string("++"));

    p_token = (token_t *)vector_at(p_s_token_vec, 4);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_OPERATOR);
    CHECK(p_token->whitespace == false);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 1);
    CHECK(p_token->s_pos.col == 11);
    CHECK_EQ(std::string(p_token->sval), std::string("("));

    p_token = (token_t *)vector_at(p_s_token_vec, 5);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_NUMBER);
    CHECK(p_token->whitespace == false);
    if (p_token->between_brackets)
        CHECK_EQ(std::string(p_token->between_brackets), std::string("3+2)"));
    CHECK(p_token->s_pos.line == 1);
    CHECK(p_token->s_pos.col == 12);
    CHECK(p_token->ival == 3);

    p_token = (token_t *)vector_at(p_s_token_vec, 6);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_OPERATOR);
    CHECK(p_token->whitespace == false);
    if (p_token->between_brackets)
        CHECK_EQ(std::string(p_token->between_brackets), std::string("3+2)"));
    CHECK(p_token->s_pos.line == 1);
    CHECK(p_token->s_pos.col == 13);
    CHECK_EQ(std::string(p_token->sval), std::string("+"));

    p_token = (token_t *)vector_at(p_s_token_vec, 7);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_NUMBER);
    CHECK(p_token->whitespace == false);
    if (p_token->between_brackets)
        CHECK_EQ(std::string(p_token->between_brackets), std::string("3+2)"));
    CHECK(p_token->s_pos.line == 1);
    CHECK(p_token->s_pos.col == 14);
    CHECK(p_token->ival == 2);

    p_token = (token_t *)vector_at(p_s_token_vec, 8);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_SYMBOL);
    CHECK(p_token->whitespace == true);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 1);
    CHECK(p_token->s_pos.col == 15);
    CHECK_EQ(p_token->cval, ')');

    p_token = (token_t *)vector_at(p_s_token_vec, 9);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_STRING);
    CHECK(p_token->whitespace == false);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 1);
    CHECK(p_token->s_pos.col == 30);
    CHECK_EQ(std::string(p_token->sval), std::string("Hello World!"));

    // Clean up
    remove("test_lexer.c");
}

TEST_CASE("lexer::lex::comment") {
    /**
     * 1. Create a new file to test our lexer on
     * 2. Write the test string '324+32 ++ (3+2) "Hello World!' to the file
     * 3. Create a new compile_process_t object
     * 4. Create a new lex_process_t object
     * 5. Call lex() on the lex_process_t object
     * 6. Check the tokens
     */
    FILE *p_file = fopen("test_lexer.c", "w");
    REQUIRE(p_file != nullptr);
    fprintf(p_file, "/**\n");
    fprintf(p_file, " * @brief Multi line comment for testing\n");
    fprintf(p_file, " * \n");
    fprintf(p_file, " * @return int \n");
    fprintf(p_file, " */\n");
    fprintf(p_file, "int main(void)\n");
    fprintf(p_file, "{\n");
    fprintf(p_file, "    long a = 87L; // This is a long \n");
    fprintf(p_file, "    int b = 0x3; // This is a hex\n");
    fprintf(p_file, "    int c = 0b101; // This is a binary\n");
    fprintf(p_file, "    int d = (a + b) * c;\n");
    fprintf(p_file, "    return d;\n");
    fprintf(p_file, "}\n");
    fclose(p_file);

    compile_process_t *p_process = compile_process_create("test_lexer.c", NULL, 0);
    REQUIRE(p_process != nullptr);

    // Preform Lexical Analysis
    lex_process_t *p_lex_process = lex_process_create(p_process, &compiler_lex_functions, NULL);
    REQUIRE(p_lex_process != nullptr);

    CHECK(LEXICAL_ANALSYS_ALL_OK == lex(p_lex_process));

    // Check the tokens
    vector *p_s_token_vec = p_lex_process->p_s_token_vec;
    // Iterate over the tokens using vector methods
    token_t *p_token = (token_t *)vector_at(p_s_token_vec, 0);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_COMMENT);
    CHECK(p_token->whitespace == false);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 5);
    CHECK(p_token->s_pos.col == 4);
    // Compare the string, indicate the first char that is not equal
    int i = 0;
    std::string expected = "**\n * @brief Multi line comment for testing\n * \n * @return int \n ";
    for (int i = 0; i < expected.length(); i++) {
        char expected_char = expected[i];
        char actual_char = p_token->sval[i];
        printf("index: %d, expected_char: %c, actual_char: %c\n", i, expected_char, actual_char);
        CHECK_EQ(expected_char, actual_char);
    }
    CHECK_EQ(std::string(p_token->sval),
             std::string("**\n * @brief Multi line comment for testing\n * \n * @return int \n "));

    p_token = (token_t *)vector_at(p_s_token_vec, 1);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_NEWLINE);
    CHECK(p_token->whitespace == false);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 6);
    CHECK(p_token->s_pos.col == 0);

    p_token = (token_t *)vector_at(p_s_token_vec, 2);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_KEYWORD);
    CHECK(p_token->whitespace == true);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 6);
    CHECK(p_token->s_pos.col == 3);
    CHECK_EQ(std::string(p_token->sval), std::string("int"));

    p_token = (token_t *)vector_at(p_s_token_vec, 3);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_IDENTIFIER);
    CHECK(p_token->whitespace == false);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 6);
    CHECK(p_token->s_pos.col == 8);
    CHECK_EQ(std::string(p_token->sval), std::string("main"));

    p_token = (token_t *)vector_at(p_s_token_vec, 4);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_OPERATOR);
    CHECK(p_token->whitespace == false);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 6);
    CHECK(p_token->s_pos.col == 9);
    CHECK_EQ(std::string(p_token->sval), "(");

    p_token = (token_t *)vector_at(p_s_token_vec, 5);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_KEYWORD);
    CHECK(p_token->whitespace == false);
    if (p_token->between_brackets)
        CHECK_EQ(std::string(p_token->between_brackets), std::string("void)"));
    CHECK(p_token->s_pos.line == 6);
    CHECK(p_token->s_pos.col == 13);
    CHECK_EQ(std::string(p_token->sval), std::string("void"));

    p_token = (token_t *)vector_at(p_s_token_vec, 6);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_SYMBOL);
    CHECK(p_token->whitespace == false);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 6);
    CHECK(p_token->s_pos.col == 14);
    CHECK_EQ(p_token->cval, ')');

    p_token = (token_t *)vector_at(p_s_token_vec, 7);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_NEWLINE);
    CHECK(p_token->whitespace == false);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 7);
    CHECK(p_token->s_pos.col == 0);

    p_token = (token_t *)vector_at(p_s_token_vec, 8);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_SYMBOL);
    CHECK(p_token->whitespace == false);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 7);
    CHECK(p_token->s_pos.col == 1);
    CHECK_EQ(p_token->cval, '{');

    p_token = (token_t *)vector_at(p_s_token_vec, 9);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_NEWLINE);
    CHECK(p_token->whitespace == true);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 8);
    CHECK(p_token->s_pos.col == 0);

    p_token = (token_t *)vector_at(p_s_token_vec, 10);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_KEYWORD);
    CHECK(p_token->whitespace == true);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 8);
    CHECK(p_token->s_pos.col == 8);
    CHECK_EQ(std::string(p_token->sval), std::string("long"));

    p_token = (token_t *)vector_at(p_s_token_vec, 11);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_IDENTIFIER);
    CHECK(p_token->whitespace == true);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 8);
    CHECK(p_token->s_pos.col == 10);
    CHECK_EQ(std::string(p_token->sval), std::string("a"));

    p_token = (token_t *)vector_at(p_s_token_vec, 12);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_OPERATOR);
    CHECK(p_token->whitespace == true);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 8);
    CHECK(p_token->s_pos.col == 12);
    CHECK_EQ(std::string(p_token->sval), std::string("="));

    p_token = (token_t *)vector_at(p_s_token_vec, 13);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_NUMBER);
    CHECK(p_token->whitespace == true);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 8);
    CHECK(p_token->s_pos.col == 17);
    CHECK(p_token->lval == 87L);

    p_token = (token_t *)vector_at(p_s_token_vec, 14);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_COMMENT);
    CHECK(p_token->whitespace == false);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 8);
    CHECK(p_token->s_pos.col == 36);
    CHECK_EQ(std::string(p_token->sval), std::string("/ This is a long "));

    p_token = (token_t *)vector_at(p_s_token_vec, 15);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_NEWLINE);
    CHECK(p_token->whitespace == true);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 9);
    CHECK(p_token->s_pos.col == 0);

    p_token = (token_t *)vector_at(p_s_token_vec, 16);
    CHECK(p_token != nullptr);
    CHECK(p_token->type == TOKEN_TYPE_KEYWORD);
    CHECK(p_token->whitespace == true);
    CHECK(p_token->between_brackets == nullptr);
    CHECK(p_token->s_pos.line == 9);
    CHECK(p_token->s_pos.col == 7);
    CHECK_EQ(std::string(p_token->sval), std::string("int"));

    // Clean up
    remove("test_lexer.c");
}