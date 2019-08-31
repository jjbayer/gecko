project(compiled_language)

add_library(compiled_language
    instructions.cpp
)

add_executable(test_instructions test_instructions.cpp)

target_link_libraries(test_instructions compiled_language)
