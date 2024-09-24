cmake_minimum_required (VERSION 3.8)
include(CMakePrintHelpers)
project("VulkanEngine")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

add_subdirectory(engine)
add_subdirectory(tests)
enable_testing()
add_test(NAME MyTest COMMAND Test)
add_custom_target(CopyLibs ALL
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:Test> $<TARGET_RUNTIME_DLLS:Test>
  DEPENDS Test Engine_lib
  COMMAND_EXPAND_LISTS
)