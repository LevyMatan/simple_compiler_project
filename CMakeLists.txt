cmake_minimum_required(VERSION 3.27)

# ---- Project ----

project(
  SimpleCompilerProject
  VERSION 1.0
  LANGUAGES C CXX
)
message("MAIN PROJECT CMAKE IS CALLED")
message("Source directory: ${PROJECT_SOURCE_DIR}")
message("Binary directory: ${PROJECT_BINARY_DIR}")

# Set the values for the placeholders
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)

# Configure the generation of version.h
set(SIMPLECOMPILERPROJECT_VERSION "${PROJECT_VERSION}")

configure_file(
  "${PROJECT_SOURCE_DIR}/version.h.in"
  "${PROJECT_SOURCE_DIR}/include/simple_compiler_project/version.h"
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

include(cmake/CPM.cmake)

# PackageProject.cmake will be used to make our target installable
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.8.0")

CPMAddPackage(
  NAME fmt
  GIT_TAG 9.1.0
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES" # create an installable target
)

# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c")

# ---- Create library ----

# Note: for header-only libraries change all PUBLIC flags to INTERFACE and create an interface
# target: add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME} ${headers} ${sources})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

# Link dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

# print the cmake build type:
message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_compile_definitions("DEBUG_ENABLED")
endif()

if(MSVC)
  target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

# the location where the project's version header will be placed should match the project's regular
# header paths
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
  DEPENDENCIES "fmt 9.1.0"
)

# ---- Enum Generator Tool ----

# Find the Python interpreter
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Define the output file that the script will generate
set(GENERATED_FILE "${CMAKE_CURRENT_SOURCE_DIR}/source/utils/generated_enum_dict.txt")

# Define the source files that the script depends on
file(GLOB_RECURSE DEPENDS_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c")
file(GLOB_RECURSE DEPENDS_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Debug: print all the dependencies
message("DEPENDS_FILES: ${DEPENDS_FILES}")

# Add a custom command that runs the Python script
add_custom_command(
  OUTPUT ${GENERATED_FILE}
  COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/generate_enum_dict.py --OutputFile
          ${GENERATED_FILE}
  COMMENT "Generating source code with Python script"
  VERBATIM
  DEPENDS ${DEPENDS_FILES}
)

# Add a custom target that depends on the generated file
add_custom_target(generate_code ALL DEPENDS ${GENERATED_FILE})

# Make the main target depend on the custom target
add_dependencies(${PROJECT_NAME} generate_code)

# ---- Create trace db target ----
# Create an custom targets
add_custom_target(
  trace_db COMMAND python ${CMAKE_SOURCE_DIR}/../trace_db.py ${CMAKE_SOURCE_DIR}/../source
                   --output_dir ${CMAKE_SOURCE_DIR}/../
)
