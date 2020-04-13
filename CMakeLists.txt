cmake_minimum_required(VERSION 3.0.2)
project(gecko)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)
add_subdirectory(app)

enable_testing()
add_subdirectory(tests)



