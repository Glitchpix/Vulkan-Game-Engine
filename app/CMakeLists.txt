add_executable(App src/entry.cpp src/game.hpp)
target_link_libraries(App PRIVATE Engine_lib)
target_include_directories(App PRIVATE ${CMAKE_SOURCE_DIR}/engine/src)