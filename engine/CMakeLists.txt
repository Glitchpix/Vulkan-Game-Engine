add_library(Engine_lib SHARED src/core/logger.cpp src/core/logger.hpp
                              src/core/asserts.hpp src/defines.hpp 
                              src/platform/platform.hpp src/platform/platform_win32.cpp
                              src/core/application.hpp src/core/application.cpp
                              src/entry.hpp src/game_types.hpp
                              src/core/e_memory.hpp src/core/e_memory.cpp
                              src/core/event.hpp src/core/event.cpp
                              src/core/input.hpp src/core/input.cpp
                              src/core/clock.hpp src/core/clock.cpp
                              src/renderer/renderer.hpp
                              src/renderer/vulkan/vulkan_backend.hpp src/renderer/vulkan/vulkan_backend.cpp)
target_include_directories(Engine_lib PRIVATE src/)
find_package(Vulkan REQUIRED)
target_include_directories(Engine_lib PRIVATE ${Vulkan_INCLUDE_DIR})