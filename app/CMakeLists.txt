

add_executable(gecko-bin gecko.cpp)

target_link_libraries(gecko-bin gecko)
target_compile_options(gecko-bin PRIVATE -Wall)

set_target_properties(gecko-bin PROPERTIES OUTPUT_NAME gecko)
