add_executable(Test src/entry.cpp src/game.hpp)
target_link_libraries(Test PRIVATE Engine_lib)
target_include_directories(Test PRIVATE ${CMAKE_SOURCE_DIR}/engine/src)