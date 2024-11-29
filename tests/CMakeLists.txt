enable_testing()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
# Include sources directly from engine to avoid exporting all symbols from the DLL.
# Unfortunately this means we are testing a different compiled version, maybe there's a better way.
add_executable(Tests src/doctest_main.cpp ${CMAKE_SOURCE_DIR}/engine/src/math/vec.cpp)
target_include_directories(Tests PRIVATE ${CMAKE_SOURCE_DIR}/tests/src)
target_include_directories(Tests PRIVATE ${CMAKE_SOURCE_DIR}/engine/src)

include(CTest)
include(doctest)
doctest_discover_tests(Tests)