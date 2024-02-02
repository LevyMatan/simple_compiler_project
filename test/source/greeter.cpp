#include <doctest/doctest.h>
#include "simple_compiler_project/version.h"
#include <string>

TEST_CASE("Greeter version") {
  static_assert(std::string_view(SIMPLECOMPILERPROJECT_VERSION) == std::string_view("1.0"));
  CHECK(std::string(SIMPLECOMPILERPROJECT_VERSION) == std::string("1.0"));
}
