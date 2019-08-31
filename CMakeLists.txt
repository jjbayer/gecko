project(compiled_language)

add_library(compiled_language
    ast.cpp
    printvisitor.cpp
    instructions.cpp
)

add_executable(test_instructions test_instructions.cpp)
target_link_libraries(test_instructions compiled_language)

add_executable(test_compiler test_compiler.cpp)
target_link_libraries(test_compiler compiled_language)
