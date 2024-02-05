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
  CHECK(p_token->between_brackets == nullptr);
  CHECK(p_token->s_pos.line == 1);
  CHECK(p_token->s_pos.col == 12);
  CHECK(p_token->ival == 3);

  p_token = (token_t *)vector_at(p_s_token_vec, 6);
  CHECK(p_token != nullptr);
  CHECK(p_token->type == TOKEN_TYPE_OPERATOR);
  CHECK(p_token->whitespace == false);
  CHECK(p_token->between_brackets == nullptr);
  CHECK(p_token->s_pos.line == 1);
  CHECK(p_token->s_pos.col == 13);
  CHECK_EQ(std::string(p_token->sval), std::string("+"));

  p_token = (token_t *)vector_at(p_s_token_vec, 7);
  CHECK(p_token != nullptr);
  CHECK(p_token->type == TOKEN_TYPE_NUMBER);
  CHECK(p_token->whitespace == false);
  CHECK(p_token->between_brackets == nullptr);
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
