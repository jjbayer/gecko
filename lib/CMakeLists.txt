

add_library(gecko
    common/exceptions.cpp
    common/object.cpp
    common/object.hpp
    common/utils.cpp

    compiler/compiler.cpp
    compiler/compiletimeobject.cpp
    compiler/lookup.cpp
    compiler/lookupkey.cpp
    compiler/objectprovider.cpp
    compiler/typecreator.cpp

    parser/ast.cpp
    parser/parser.cpp
    parser/printvisitor.cpp

    runtime/executor.cpp
    runtime/instructions.cpp
    runtime/instructions.hpp
    runtime/memorymanager.cpp
    runtime/objects/builtins.cpp
    runtime/objects/string.cpp
    runtime/objects/stdin.cpp
    runtime/objects/userfunction.cpp
    runtime/output.cpp

    tokenizer/statemachine.cpp
    tokenizer/tokenizer.cpp
)
target_include_directories(gecko PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(gecko INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")  # TODO: install

target_compile_options(gecko PRIVATE -Wall)
