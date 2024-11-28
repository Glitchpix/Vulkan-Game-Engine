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
add_subdirectory(app)
add_subdirectory(tests)
add_custom_target(CopyLibs ALL
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:App> $<TARGET_RUNTIME_DLLS:App>
  DEPENDS App Engine_lib
  COMMAND_EXPAND_LISTS
)