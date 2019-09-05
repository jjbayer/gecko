cmake_minimum_required(VERSION 3.10)
project(compiled_language)

add_library(compiled_language
    ast.cpp
    compiler.cpp
    lookup.cpp
    exceptions.cpp
    executor.cpp
    instructions.cpp
    printvisitor.cpp
)

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

enable_testing()
add_executable(test_instructions test_instructions.cpp)
target_link_libraries(test_instructions compiled_language Boost::unit_test_framework)
add_test(test_instructions test_instructions)

add_executable(test_compiler test_compiler.cpp)
target_link_libraries(test_compiler compiled_language Boost::unit_test_framework)
add_test(test_compiler test_compiler)
