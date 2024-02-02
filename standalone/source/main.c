// #include <simple_compiler_project/version.h>
#include <stdio.h>

#include "compiler/compiler.h"
#include "utils/vector.h"

int main(void) {
  int res = compile_file("./test.c", "./test", 0);
  if (COMPILER_FILE_COMPILED_OK == res) {
    printf("File compiled successfully!\n");
  } else if (COMPILER_FAILED_WITH_ERRORS == res) {
    printf("File failed to compile!\n");
  } else {
    printf("Unknown error!\n");
  }
  return 0;
}