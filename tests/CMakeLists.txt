add_executable(Test src/entry.cpp src/game.hpp)
target_link_libraries(Test PRIVATE Engine_lib)
target_include_directories(Test PRIVATE ${CMAKE_SOURCE_DIR}/engine/src)
add_custom_command(TARGET Test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:Test> $<TARGET_RUNTIME_DLLS:Test>
  COMMAND_EXPAND_LISTS
)