cmake_minimum_required (VERSION 3.8)
include(CMakePrintHelpers)
project("VulkanEngine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(engine)
add_subdirectory(tests)
enable_testing()
add_test(NAME MyTest COMMAND Test)