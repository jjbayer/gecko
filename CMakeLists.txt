cmake_minimum_required(VERSION 3.10)
project(compiled_language)

add_library(compiled_language
    ast.cpp
    builtins.cpp
    compiler.cpp
    lookup.cpp
    exceptions.cpp
    executor.cpp
    instructions.cpp
    parser.cpp
    printvisitor.cpp
    statemachine.cpp
    tokenizer.cpp
)
target_compile_options(compiled_language PRIVATE -Wall)

add_executable(gecko gecko.cpp)
target_link_libraries(gecko compiled_language)

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

add_executable(test_compiler test_compiler.cpp)
target_link_libraries(test_compiler compiled_language Boost::unit_test_framework)
add_test(test_compiler test_compiler)

add_executable(test_tokenizer test_tokenizer.cpp)
target_link_libraries(test_tokenizer compiled_language Boost::unit_test_framework)
add_test(test_tokenizer test_tokenizer)
