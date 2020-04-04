

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

add_executable(test_tokenizer test_tokenizer.cpp)
target_link_libraries(test_tokenizer gecko Boost::unit_test_framework)
add_test(test_tokenizer test_tokenizer)

add_executable(test_parser test_parser.cpp)
target_link_libraries(test_parser gecko Boost::unit_test_framework)
add_test(test_parser test_parser)

add_executable(test_compiler test_compiler.cpp)
target_link_libraries(test_compiler gecko Boost::unit_test_framework)
add_test(test_compiler test_compiler)

# Test full Gecko programs
add_executable(test_full test_full.cpp)
target_link_libraries(test_full gecko Boost::unit_test_framework)
add_test(test_full test_full)
